#ifndef _MEMORY_DATA_LAYER_CPU_H__
#define _MEMORY_DATA_LAYER_CPU_H__

#include "HolidayBaseLayer.h"
#include <memory>
#include <cstring>

#include "orz/sync/shotgun.h"
#include "orz/tools/ctxmgr_lite.h"
#include "orz/tools/box.h"

template <class	T>
class HolidayMemoryDataLayerCPU :public HolidayBaseLayer<T>
{
public:
	HolidayMemoryDataLayerCPU(){};
	~HolidayMemoryDataLayerCPU(){};

	int Init(Holiday_LayerParameter& inputparam, HolidayNetResource<T> *pNetResource);
	int Process(std::vector<HolidayFeatureMap<T>*> input_data_map, std::vector<HolidayFeatureMap<T>*>& output_data_map);
	int CroppingImageWithMean(T *src, T *mean, T *dest, T scale, int channel, int image_height, int image_width, int dest_height, int dest_width);
	int CroppingImageNoMean(T *src, T *dest, T scale, int channel, int image_height, int image_width, int dest_height, int dest_width);
	int CroppingImageWithMeanValue(T *src, T *mean, T *dest, T scale, int channel, int image_height, int image_width, int dest_height, int dest_width);
	int ChannelSwap(T *data, int number, int channels, int height, int width);
public:

	T data_scale;
	int32_t m_mean_type;
	int m_dest_height;
	int m_dest_width;
	int crop_height;
	int crop_width;
	HolidayBlobCpu<T> m_mean_blob;

	std::vector<T> m_mean_value;

	std::vector<int> m_channel_swaps;
	size_t m_swap_space_size;
	std::shared_ptr<T> m_swap_space;

	// supporting prewhiten
	bool m_prewhiten = false;
};

enum HolidayMeanType
{
	HolidayNoMean = 0,
	HolidayMeanFile = 1,
	HolidayMeanValue = 2,
};

template <class T>
int HolidayMemoryDataLayerCPU<T>::CroppingImageWithMean(T *src, T *mean, T *dest, T scale, int channel, int image_height, int image_width, int dest_height, int dest_width)
{
	
	int dest_single_image_element = dest_width*dest_height;
	int src_single_image_element = image_height*image_width;

	auto gun = orz::ctx::lite::ptr<orz::Shotgun>();
	if (gun == nullptr || gun->size() <= 1)
	{
		for (int c = 0; c < channel; c++)
		{
			T * src_start = src + c*src_single_image_element + crop_height*image_width + crop_width;
			T * mean_start = mean + c*src_single_image_element + crop_height*image_width + crop_width;
			T * dest_start = dest + c*dest_single_image_element;
			for (int i = 0; i < dest_height; i++)
			{
				for (int j = 0; j < dest_width; j++)
				{
					T tmp_src_value = src_start[j];
					*dest_start = tmp_src_value - mean_start[j];
					*dest_start *= scale;
					dest_start++;
				}
				src_start += image_width;
				mean_start += image_width;
			}
		}
	}
	else
	{
		auto bins = orz::lsplit_bins(0, channel, gun->size());
		for (auto &bin : bins)
		{
			gun->fire([&, bin](int)
			{
				for (size_t c = bin.first; c < bin.second; c++)
				{
					T * src_start = src + c*src_single_image_element + crop_height*image_width + crop_width;
					T * mean_start = mean + c*src_single_image_element + crop_height*image_width + crop_width;
					T * dest_start = dest + c*dest_single_image_element;
					for (int i = 0; i < dest_height; i++)
					{
						for (int j = 0; j < dest_width; j++)
						{
							T tmp_src_value = src_start[j];
							*dest_start = tmp_src_value - mean_start[j];
							*dest_start *= scale;
							dest_start++;
						}
						src_start += image_width;
						mean_start += image_width;
					}
				}
			});
		}
		gun->join();
	}

	return 0;
}

template <class T>
int HolidayMemoryDataLayerCPU<T>::CroppingImageWithMeanValue(T *src, T *mean, T *dest, T scale, int channel, int image_height, int image_width, int dest_height, int dest_width)
{

	int dest_single_image_element = dest_height*dest_width;
	int src_single_image_element = image_height*image_width;

	auto gun = orz::ctx::lite::ptr<orz::Shotgun>();
	if (gun == nullptr || gun->size() <= 1)
	{
		for (int c = 0; c < channel; c++)
		{
			T * src_start = src + c*src_single_image_element + crop_height*image_width + crop_width;
			T * dest_start = dest + c*dest_single_image_element;
			for (int i = 0; i < dest_height; i++)
			{
				for (int j = 0; j < dest_width; j++)
				{
					T tmp_src_value = src_start[j];
					*dest_start = tmp_src_value - mean[c];
					*dest_start *= scale;
					dest_start++;
				}
				src_start += image_width;

			}
		}
	}
	else
	{
		auto bins = orz::lsplit_bins(0, channel, gun->size());
		for (auto &bin : bins)
		{
			gun->fire([&, bin](int)
			{
				for (size_t c = bin.first; c < bin.second; c++)
				{
					T * src_start = src + c*src_single_image_element + crop_height*image_width + crop_width;
					T * dest_start = dest + c*dest_single_image_element;
					for (int i = 0; i < dest_height; i++)
					{
						for (int j = 0; j < dest_width; j++)
						{
							T tmp_src_value = src_start[j];
							*dest_start = tmp_src_value - mean[c];
							*dest_start *= scale;
							dest_start++;
						}
						src_start += image_width;

					}
				}
			});
		}
		gun->join();
	}

	return 0;
}

template <class T>
static void CopyData(T *dst, const T*src, size_t _count)
{
#if _MSC_VER >= 1600
	memcpy_s(dst, _count * sizeof(T), src, _count * sizeof(T));
#else
	memcpy(dst, src, _count * sizeof(T));
#endif
}

template <class T>
int HolidayMemoryDataLayerCPU<T>::ChannelSwap(T* data, int number, int channels, int height, int width)
{
	if (m_channel_swaps.size() != channels) return 1;

	size_t data_size = number * channels * height * width;

	if (m_swap_space_size < data_size)
	{
		m_swap_space_size = data_size;
		m_swap_space.reset(new T[m_swap_space_size], std::default_delete<T[]>());
	}

	size_t block_size = height * width;
	size_t step = block_size * channels;


	auto gun = orz::ctx::lite::ptr<orz::Shotgun>();
	if (gun == nullptr || gun->size() <= 1)
	{
		for (int n = 0; n < number; ++n)
		{
			for (int c = 0; c < channels; ++c)
			{
				const T *src = &data[n * step + m_channel_swaps[c] * block_size];
				T *dst = &m_swap_space.get()[n * step + c * block_size];
				CopyData(dst, src, block_size);
			}
		}
	}
	else
	{
		for (int n = 0; n < number; ++n)
		{
			auto bins = orz::lsplit_bins(0, channels, gun->size());
			for (auto &bin : bins)
			{
				gun->fire([&, n, bin](int) {
					for (size_t c = bin.first; c < bin.second; ++c)
					{
						const T *src = &data[n * step + m_channel_swaps[c] * block_size];
						T *dst = &m_swap_space.get()[n * step + c * block_size];
						CopyData(dst, src, block_size);
					}
				});
			}
		}
		gun->join();
	}

	CopyData(data, m_swap_space.get(), data_size);
	return 0;
}

template <class T>
int HolidayMemoryDataLayerCPU<T>::CroppingImageNoMean(T *src, T *dest, T scale, int channel, int image_height, int image_width, int dest_height, int dest_width)
{
	
	int dest_single_image_element = dest_width*dest_height;
	int src_single_image_element = image_height*image_width;

	auto gun = orz::ctx::lite::ptr<orz::Shotgun>();
	if (gun == nullptr || gun->size() <= 1)
	{
		for (int c = 0; c < channel; c++)
		{
			T * src_start = src + c*src_single_image_element + crop_height*image_width + crop_width;
			T * dest_start = dest + c*dest_single_image_element;
			for (int i = 0; i < dest_height; i++)
			{
				for (int j = 0; j < dest_width; j++)
				{
					T tmp_src_value = src_start[j];
					dest_start[j] = tmp_src_value*scale;
				}
				src_start += image_width;
				dest_start += dest_width;
			}
		}
	}
	else
	{
		auto bins = orz::lsplit_bins(0, channel, gun->size());
		for (auto &bin : bins)
		{
			gun->fire([&, bin](int) {
				for (size_t c = bin.first; c < bin.second; c++)
				{
					T * src_start = src + c*src_single_image_element + crop_height*image_width + crop_width;
					T * dest_start = dest + c*dest_single_image_element;
					for (int i = 0; i < dest_height; i++)
					{
						for (int j = 0; j < dest_width; j++)
						{
							T tmp_src_value = src_start[j];
							dest_start[j] = tmp_src_value*scale;
						}
						src_start += image_width;
						dest_start += dest_width;
					}
				}
			});
		}
		gun->join();
	}

	return 0;
}

template <typename T>
void prewhiten(T *data, size_t len)
{
	double mean = 0;
	double std_dev = 0;
	T *at= nullptr;

	at = data;
	for (size_t i = 0; i < len; ++i, ++at) mean += *at;
	mean /= len;

	at = data;
	for (size_t i = 0; i < len; ++i, ++at) std_dev += (*at - mean) * (*at - mean);
	std_dev = std::sqrt(std_dev / len);
	std_dev = std::max<T>(std_dev, 1 / std::sqrt(len));
	double std_dev_rec = 1 / std_dev;

	at = data;
	for (size_t i = 0; i < len; ++i, ++at) {
		*at -= mean;
		*at *= std_dev_rec;
	}
}


template <class	T>
int HolidayMemoryDataLayerCPU<T>::Process(std::vector<HolidayFeatureMap<T>*> input_data_map, std::vector<HolidayFeatureMap<T>*>& output_data_map)
{
	input_data_map[0]->TransFormDataIn();
	//memcpy(output_data_map[0]->m_cpu.dataMemoryPtr(), input_data_map[0]->m_cpu.dataMemoryPtr(), sizeof(T)*input_data_map[0]->m_cpu.count_);
	
	int edge_height = input_data_map[0]->data_shape[2] - m_dest_height;
	int edge_width = input_data_map[0]->data_shape[3] - m_dest_width;

	crop_height = edge_height / 2;
	crop_width = edge_width / 2;

	int tmp_dest_height = 0;
	int tmp_dest_width = 0;
	if (edge_height<0 || edge_width<0)
	{
		crop_height = 0;
		crop_width = 0;

		tmp_dest_height = input_data_map[0]->data_shape[2];
		tmp_dest_width = input_data_map[0]->data_shape[3];
	}
	else
	{
		tmp_dest_height = m_dest_height;
		tmp_dest_width = m_dest_width;
	}

	T *dest_base = output_data_map[0]->m_cpu.dataMemoryPtr();
	T *src_base = input_data_map[0]->m_cpu.dataMemoryPtr();

	output_data_map[0]->data_shape[0] = input_data_map[0]->data_shape[0];
	output_data_map[0]->data_shape[1] = input_data_map[0]->data_shape[1];
	output_data_map[0]->data_shape[2] = tmp_dest_height;
	output_data_map[0]->data_shape[3] = tmp_dest_width;

	int channel = input_data_map[0]->data_shape[1];
	int image_height = input_data_map[0]->data_shape[2];
	int image_width = input_data_map[0]->data_shape[3];
	
	int n_single_image_size_src = input_data_map[0]->data_shape[1] * input_data_map[0]->data_shape[2] * input_data_map[0]->data_shape[3];
	int n_single_image_size_dest = output_data_map[0]->data_shape[1] * output_data_map[0]->data_shape[2] * output_data_map[0]->data_shape[3];
	if (HolidayMeanFile == m_mean_type)
	{
		for (int in_number = 0; in_number < input_data_map[0]->data_shape[0]; in_number++)
		{
			T *src = src_base + in_number* n_single_image_size_src;
			T *dest = dest_base + in_number* n_single_image_size_dest;
			
			T* mean_point = m_mean_blob.dataMemoryPtr();

			CroppingImageWithMean(src, mean_point, dest, data_scale, channel, image_height, image_width,tmp_dest_height, tmp_dest_width);
		}
	}
	else if (HolidayNoMean == m_mean_type)
	{
		for (int in_number = 0; in_number < input_data_map[0]->data_shape[0]; in_number++)
		{
			T *src = src_base + in_number* n_single_image_size_src;
			T *dest = dest_base + in_number* n_single_image_size_dest;

			CroppingImageNoMean(src, dest, data_scale, channel, image_height, image_width,tmp_dest_height, tmp_dest_width);
		}
	}
	else if (HolidayMeanValue == m_mean_type)
	{
		for (int in_number = 0; in_number < input_data_map[0]->data_shape[0]; in_number++)
		{
			T *src = src_base + in_number* n_single_image_size_src;
			T *dest = dest_base + in_number* n_single_image_size_dest;

			CroppingImageWithMeanValue(src, &m_mean_value[0], dest, data_scale, channel, image_height, image_width, tmp_dest_height, tmp_dest_width);
		}
	}
	else
	{

	}

	if (m_channel_swaps.size())
	{
		ChannelSwap(dest_base, input_data_map[0]->data_shape[0], channel, tmp_dest_height, tmp_dest_width);
	}

	output_data_map[0]->dwStorageType = DATA_CPU_WIDTH;

	output_data_map[0]->data_shape[0] = input_data_map[0]->data_shape[0];
	output_data_map[0]->data_shape[1] = input_data_map[0]->data_shape[1];
	output_data_map[0]->data_shape[2] = tmp_dest_height;
	output_data_map[0]->data_shape[3] = tmp_dest_width;

	// do prewhiten
    if (this->m_prewhiten)
    {
        int img_size = output_data_map[0]->data_shape[1] *
                       output_data_map[0]->data_shape[2] *
                       output_data_map[0]->data_shape[3];
        int img_num = output_data_map[0]->data_shape[0];

        T *img_data = output_data_map[0]->m_cpu.dataMemoryPtr();

        for (int i = 0; i < img_num; ++i, img_data += img_size)
        {
            prewhiten(img_data, img_size);
        }
    }

	return 0;
}

void memset(void *dst, size_t dst_size, const void *src, size_t src_size)
{
	std::memcpy(dst, src, std::min(dst_size, src_size));
	size_t copy_anchor = src_size;
	while (copy_anchor <= dst_size >> 1)
	{
		std::memcpy(reinterpret_cast<char*>(dst) + copy_anchor, dst, copy_anchor);
		copy_anchor <<= 1;
	}
	if (dst_size > copy_anchor)
	{
		std::memcpy(reinterpret_cast<char*>(dst)+copy_anchor, dst, dst_size - copy_anchor);
	}
}

template <class	T>
int HolidayMemoryDataLayerCPU<T>::Init(Holiday_LayerParameter& inputparam, HolidayNetResource<T> *pNetResource)
{


	this->bottom_data_size.resize(1);
	this->bottom_data_size[0].data_dim.resize(4);
	this->bottom_data_size[0].data_dim[0] = pNetResource->max_batch_size;
	this->bottom_data_size[0].data_dim[1] = inputparam.memory_data_param().channels();

	int local_input_height = pNetResource->m_new_height > 0
		? pNetResource->m_new_height
		: int(inputparam.memory_data_param().height());
	int local_input_width = pNetResource->m_new_width > 0
		? pNetResource->m_new_width
		: int(inputparam.memory_data_param().width());

	this->bottom_data_size[0].data_dim[2] = local_input_height;
	this->bottom_data_size[0].data_dim[3] = local_input_width;
	
	

	if (inputparam.memory_data_param().has_crop_size_height() && inputparam.memory_data_param().has_crop_size_width())
	{
		m_dest_height = inputparam.memory_data_param().crop_size_height();
		m_dest_width = inputparam.memory_data_param().crop_size_width();

	
	}
	else if (inputparam.memory_data_param().has_crop_size_height())
	{
		m_dest_height = inputparam.memory_data_param().crop_size_height();
		m_dest_width = this->bottom_data_size[0].data_dim[3];
	
	}
	else if (inputparam.memory_data_param().has_crop_size_width())
	{
		m_dest_width = inputparam.memory_data_param().crop_size_width();
		m_dest_height = this->bottom_data_size[0].data_dim[2];
		
	}
	else
	{
		m_dest_height = this->bottom_data_size[0].data_dim[2];
		m_dest_width = this->bottom_data_size[0].data_dim[3];
		
	}

	this->m_prewhiten = inputparam.memory_data_param().prewhiten();

	std::vector<int> tmp_mean_shape;
	tmp_mean_shape.push_back(1);
	tmp_mean_shape.push_back(this->bottom_data_size[0].data_dim[1]);
	tmp_mean_shape.push_back(this->bottom_data_size[0].data_dim[2]);
	tmp_mean_shape.push_back(this->bottom_data_size[0].data_dim[3]);
	m_mean_blob.Reshape(tmp_mean_shape);

	data_scale = inputparam.memory_data_param().scale();

	
	if (inputparam.memory_data_param().has_mean_file())
	{
		m_mean_type = HolidayMeanFile;
		std::vector<int> index;
		index.resize(4);
		index[0] = 0;
		index[1] = 0;
		index[2] = 0;
		index[3] = 0;
		int index_input = 0;
		for (int i = 0; i < this->bottom_data_size[0].data_dim[1]; i++)
		{
			index[1] = i;
			for (int j = 0; j < this->bottom_data_size[0].data_dim[2]; j++)
			{
				index[2] = j;
				for (int k = 0; k < this->bottom_data_size[0].data_dim[3]; k++)
				{
					index[3] = k;
					index_input++;
					m_mean_blob.data_at(index) = inputparam.memory_data_param().mean_file().data(index_input);
				}
			}
		}
	}
	else if (inputparam.memory_data_param().mean_value_size()>0)
	{
		m_mean_type = HolidayMeanValue;
		std::vector<int> index;
		index.resize(4);
		index[0] = 0;
		index[1] = 0;
		index[2] = 0;
		index[3] = 0;
		auto block_size = this->bottom_data_size[0].data_dim[2] * this->bottom_data_size[0].data_dim[3];
		for (int i = 0; i < this->bottom_data_size[0].data_dim[1];i++)
		{
			index[1] = i;
			float mean = inputparam.memory_data_param().mean_value(i);
			memset(&m_mean_blob.data_at(index), block_size * sizeof(float), &mean, sizeof(float));
			/*
			for (int j = 0; j < this->bottom_data_size[0].data_dim[2];j++)
			{
				index[2] = j;
				for (int k = 0; k < this->bottom_data_size[0].data_dim[3];k++)
				{
					index[3] = k;
					m_mean_blob.data_at(index) = inputparam.memory_data_param().mean_value(i);
				}
			}
			*/
		}
		m_mean_value.resize(this->bottom_data_size[0].data_dim[1]);
		for (int i = 0; i < this->bottom_data_size[0].data_dim[1]; i++)
		{
			m_mean_value[i] = inputparam.memory_data_param().mean_value(i);
		}
	}
	else
	{
		m_mean_type = 0;
	}

	// for channel_swaps
	m_swap_space_size = 0;
	if (inputparam.memory_data_param().channel_swaps_size() == this->bottom_data_size[0].data_dim[1])
	{
		auto &memory_data_param = inputparam.memory_data_param();
		m_channel_swaps.reserve(memory_data_param.channel_swaps_size());
		bool swapped = false;
		for (int i = 0; i < memory_data_param.channel_swaps_size(); ++i)
		{
			m_channel_swaps.emplace_back(memory_data_param.channel_swaps(i));
			if (memory_data_param.channel_swaps(i) != i) swapped = true;
			if (memory_data_param.channel_swaps(i) >= this->bottom_data_size[0].data_dim[1] ||
				memory_data_param.channel_swaps(i) < 0) swapped = false;
		}
		if (swapped)
		{
			m_swap_space_size = pNetResource->max_batch_size * this->bottom_data_size[0].data_dim[1] * m_dest_height * m_dest_width;
			m_swap_space.reset(new T[m_swap_space_size], std::default_delete<T[]>());
		}
		else
		{
			std::cerr << "Error: ImageData layer: " << "channel_swaps not using. Invalid or useless." << std::endl;
			m_channel_swaps.clear();
			m_channel_swaps.shrink_to_fit();
		}
	}
	else if (inputparam.memory_data_param().channel_swaps_size())
	{
		std::cerr << "Error: ImageData layer: " << "channel_swaps size must match input channels, got "
			      << inputparam.memory_data_param().channel_swaps_size() << "vs. "
				  << this->bottom_data_size[0].data_dim[1] << std::endl;
	}

	this->top_data_size.resize(2);
	for (int i = 0; i < 2;i++)
	{
		this->top_data_size[i].data_dim.resize(4);
	}
	this->top_data_size[0].data_dim[0] = pNetResource->max_batch_size;
	this->top_data_size[0].data_dim[1] = this->bottom_data_size[0].data_dim[1];
	this->top_data_size[0].data_dim[2] = m_dest_height;
	this->top_data_size[0].data_dim[3] = m_dest_width;
		  
	this->top_data_size[1].data_dim[0] = pNetResource->max_batch_size;
	this->top_data_size[1].data_dim[1] = 1;
	this->top_data_size[1].data_dim[2] = 1;
	this->top_data_size[1].data_dim[3] = 1;

	return 0;
}




#endif
