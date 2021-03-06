#include "ImageProcess.h"

#include "common_alignment.h"
#include <array>

/*
const seeta::Image seeta::crop(const Image &img, const Rect &_rect)
{
	using namespace std;
	// Adjust rect
	Rect rect = _rect;
	rect.width += rect.x;
	rect.height += rect.y;
	rect.x = max(0, min(img.width() - 1, rect.x));
	rect.y = max(0, min(img.height() - 1, rect.y));
	rect.width = max(0, min(img.width() - 1, rect.width));
	rect.height = max(0, min(img.height() - 1, rect.height));
	rect.width -= rect.x;
	rect.height -= rect.y;

	// crop image
	Image result(rect.width, rect.height, img.channels());

	const Image::Datum *iter_in_ptr = &img.data()[rect.y * img.width() * img.channels() + rect.x * img.channels()];
	int iter_in_step = img.width() * img.channels();
	int copy_size = rect.width * img.channels();
	int iter_size = rect.height;
	Image::Datum *iter_out_ptr = &result.data()[0];
	int iter_out_step = result.width() * result.channels();

	for (int i = 0; i < iter_size; ++i, iter_in_ptr += iter_in_step, iter_out_ptr += iter_out_step)
	{
		CopyData(iter_out_ptr, iter_in_ptr, copy_size);
	}

	return std::move(result);
}
*/


const seeta::Image seeta::color(const Image& img)
{
	if (img.channels() == 3) return img;
	if (img.channels() != 1) throw std::logic_error(str("Can not convert image with channels: ", img.channels()));
	Image color_img(img.width(), img.height(), 3);
	int _count = img.width() * img.height();
	for (int i = 0; i < _count; ++i)
	{
		const seeta::Image::Datum *gray = &img[i];
		seeta::Image::Datum *color = &color_img[i * 3];
		color[0] = color[1] = color[2] = gray[0];
	}
	return std::move(color_img);
}

const seeta::Image seeta::gray(const Image& img)
{
	if (img.channels() == 1) return img;
	if (img.channels() != 3) throw std::logic_error(str("Can not convert image with channels: ", img.channels()));
	Image gray_img(img.width(), img.height(), 1);
	int _count = img.width() * img.height();
	for (int i = 0; i < _count; ++i)
	{
		const seeta::Image::Datum *color = &img[i * 3];
		seeta::Image::Datum *gray = &gray_img[i];
		gray[0] = static_cast<seeta::Image::Datum>(0.1140 * color[0] + 0.5870 * color[1] + 0.2989 * color[2]);
	}
	return std::move(gray_img);
}

const seeta::Image seeta::crop(const Image& img, const Rect& rect)
{
	using namespace std;
	// Adjust rect
	Rect fixed_rect = rect;
	fixed_rect.width += fixed_rect.x;
	fixed_rect.height += fixed_rect.y;
	fixed_rect.x = max(0, min(img.width() - 1, fixed_rect.x));
	fixed_rect.y = max(0, min(img.height() - 1, fixed_rect.y));
	fixed_rect.width = max(0, min(img.width() - 1, fixed_rect.width));
	fixed_rect.height = max(0, min(img.height() - 1, fixed_rect.height));
	fixed_rect.width -= fixed_rect.x;
	fixed_rect.height -= fixed_rect.y;

	// crop image
	Image result(rect.width, rect.height, img.channels());
	memset(result.data(), 0, sizeof(Image::Datum) * result.width() * result.height() * result.channels());

	const Image::Datum *iter_in_ptr = &img.data()[fixed_rect.y * img.width() * img.channels() + fixed_rect.x * img.channels()];
	int iter_in_step = img.width() * img.channels();
	int copy_size = fixed_rect.width * img.channels();
	int iter_size = fixed_rect.height;
	Image::Datum *iter_out_ptr = &result.data()[max(0, fixed_rect.y - rect.y) * result.width() * result.channels() + max(0, fixed_rect.x - rect.x) * result.channels()];
	int iter_out_step = result.width() * result.channels();

	for (int i = 0; i < iter_size; ++i, iter_in_ptr += iter_in_step, iter_out_ptr += iter_out_step)
	{
		CopyData(iter_out_ptr, iter_in_ptr, copy_size);
	}

	return std::move(result);
}


const seeta::Image seeta::pad(const Image& img, const Padding& padding)
{
	int w = padding.width;
	int h = padding.height;
	if (w * h < 0)
	{
		throw std::logic_error(str("Illegal padding arguments (", w, ", ", h, ")"));
	}
	if (w == 0 && h == 0)
	{
		return img;
	}
	if (w < 0 || h < 0)
	{
		return crop(img, Rect(0 - w, 0 - h, img.width() + 2 * w, img.height() + 2 * h));
	}

	// pad image
	Image result(img.width() + 2 * w, img.height() + 2 * h, img.channels());
	memset(result.data(), 0, result.count() * sizeof(Image::Datum));

	const Image::Datum *iter_in_ptr = &img.data()[0];
	int iter_in_step = img.width() * img.channels();
	int copy_size = img.width() * img.channels();
	int iter_size = img.height();
	Image::Datum *iter_out_ptr = &result.data()[h * result.width() * result.channels() + w * result.channels()];
	int iter_out_step = result.width() * result.channels();

	for (int i = 0; i < iter_size; ++i, iter_in_ptr += iter_in_step, iter_out_ptr += iter_out_step)
	{
		CopyData(iter_out_ptr, iter_in_ptr, copy_size);
	}

	return std::move(result);
}

/**
* Copy from "VIPLNetGenderPredictor.cpp". Original author: ZhangJie, HeZhenliang
*/
const seeta::Image seeta::resize(const Image &img, const Size &size)
{
	using namespace std;

	if (img.width() == size.width && img.height() == size.height)
	{
		return img;
	}

	Image resized_img(size.width, size.height, img.channels());

	int src_width = img.width();
	int src_height = img.height();
	int src_channels = img.channels();
	int dst_width = resized_img.width();
	int dst_height = resized_img.height();
	int dst_channels = resized_img.channels();
	const seeta::Image::Datum *src_im = img.data();
	seeta::Image::Datum *dst_im = resized_img.data();

	double lfx_scl = static_cast<double>(src_width) / dst_width;
	double lfy_scl = static_cast<double>(src_height) / dst_height;
	double bias_x = lfx_scl / 2 - 0.5;
	double bias_y = lfy_scl / 2 - 0.5;

	for (int n_y_d = 0; n_y_d < dst_height; n_y_d++)
	{
			std::unique_ptr<double[]> raw_channel_buff(new double[src_channels]);
			double *channel_buff = raw_channel_buff.get();
			for (int n_x_d = 0; n_x_d < dst_width; n_x_d++)
			{
				double lf_x_s = lfx_scl * n_x_d + bias_x;
				double lf_y_s = lfy_scl * n_y_d + bias_y;

				lf_x_s = lf_x_s >= 0 ? lf_x_s : 0;
				lf_x_s = lf_x_s < src_width - 1 ? lf_x_s : src_width - 1 - 1e-5;
				lf_y_s = lf_y_s >= 0 ? lf_y_s : 0;
				lf_y_s = lf_y_s < src_height - 1 ? lf_y_s : src_height - 1 - 1e-5;

				int n_x_s = static_cast<int>(lf_x_s);
				int n_y_s = static_cast<int>(lf_y_s);

				double lf_weight_x = lf_x_s - n_x_s;
				double lf_weight_y = lf_y_s - n_y_s;

				for (int c = 0; c < src_channels; c++)
				{
					channel_buff[c] =
						(1 - lf_weight_y) * (1 - lf_weight_x) * src_im[(n_y_s * src_width + n_x_s) * src_channels + c] +
						(1 - lf_weight_y) * lf_weight_x * src_im[(n_y_s * src_width + n_x_s + 1) * src_channels + c] +
						lf_weight_y * (1 - lf_weight_x) * src_im[((n_y_s + 1) * src_width + n_x_s) * src_channels + c] +
						lf_weight_y * lf_weight_x * src_im[((n_y_s + 1) * src_width + n_x_s + 1) * src_channels + c];
				}

				// must have same channels
				for (int c = 0; c < dst_channels; c++)
				{
					dst_im[(n_y_d * dst_width + n_x_d) * dst_channels + c] =
						static_cast<seeta::Image::Datum>(max(0.0f, min(255.0f, static_cast<float>(channel_buff[c]))));
				}
			}
	}

	return std::move(resized_img);
}

const seeta::Image seeta::crop_resize(const Image &img, const Rect &_rect, const Size &size)
{
	using namespace std;

	Image resized_img(size.width, size.height, img.channels());

	// Ajuest rect
	Rect rect = _rect;
	rect.x = max(0, min(img.width() - 1, rect.x));
	rect.y = max(0, min(img.height() - 1, rect.y));
	rect.width = max(0, min(img.width() - rect.x, rect.width));
	rect.height = max(0, min(img.height() - rect.y, rect.height));

	int src_width = img.width();
	int src_height = img.height();
	int src_channels = img.channels();
	int dst_width = size.width;
	int dst_height = size.height;
	int dst_channels = src_channels;
	int crop_x = rect.x;
	int crop_y = rect.y;
	int crop_w = rect.width;
	int crop_h = rect.height;
	const seeta::Image::Datum *src_im = img.data();
	seeta::Image::Datum *dst_im = resized_img.data();

	float lfx_scl = float(crop_w) / dst_width;
	float lfy_scl = float(crop_h) / dst_height;

	std::unique_ptr<float[]> raw_bufferf(new float[dst_width + dst_height]);
	std::unique_ptr<int[]> raw_bufferi(new int[dst_width + dst_height]);

	float *wx = raw_bufferf.get();
	float *wy = raw_bufferf.get() + dst_width;
	int *nx = raw_bufferi.get();
	int *ny = raw_bufferi.get() + dst_width;

	for (int n_y_d = 0; n_y_d < dst_height; n_y_d++)
	{
		float lf_y_s = lfy_scl * n_y_d + crop_y;
		ny[n_y_d] = int(lf_y_s);
		if (ny[n_y_d] == src_height - 1)
			ny[n_y_d] -= 1;
		wy[n_y_d] = lf_y_s - ny[n_y_d];
	}

	for (int n_x_d = 0; n_x_d < dst_width; n_x_d++)
	{
		float lf_x_s = lfx_scl * n_x_d + crop_x;
		nx[n_x_d] = int(lf_x_s);
		if (nx[n_x_d] == src_width - 1)
			ny[n_x_d] -= 1;
		wx[n_x_d] = lf_x_s - nx[n_x_d];
	}

	for (int i = 0; i < dst_height * dst_width; i++)
	{
		int n_y_d = i / dst_width;
		int n_x_d = i - n_y_d * dst_width;
		int n_x_s = nx[n_x_d];
		int n_y_s = ny[n_y_d];

		float lf_weight_x = wx[n_x_d];
		float lf_weight_y = wy[n_y_d];

		float s1 = (1 - lf_weight_y) * (1 - lf_weight_x);
		float s2 = (1 - lf_weight_y) * lf_weight_x;
		float s3 = lf_weight_y * (1 - lf_weight_x);
		float s4 = lf_weight_y * lf_weight_x;
		int d_index = (n_y_d * dst_width + n_x_d) * dst_channels;
		int s_index1 = (n_y_s * src_width + n_x_s) * src_channels;
		int s_index2 = (n_y_s * src_width + n_x_s + 1) * src_channels;
		int s_index3 = ((n_y_s + 1) * src_width + n_x_s) * src_channels;
		int s_index4 = ((n_y_s + 1) * src_width + n_x_s + 1) * src_channels;
		for (int c = 0; c < src_channels; c++)
		{
			float ans = s1 * src_im[s_index1 + c] +
				s2 * src_im[s_index2 + c] +
				s3 * src_im[s_index3 + c] +
				s4 * src_im[s_index4 + c];
			dst_im[d_index + c] =
				static_cast<seeta::Image::Datum>(max(0.0f, min(255.0f, static_cast<float>(ans))));
		}
	}

	return std::move(resized_img);
}

const seeta::Image seeta::equalize_hist(const Image& img)
{
	// get hist
	// int hist[3][256] = { 0 };
	std::vector<std::array<int, 256>> hist(img.channels());
	std::vector<int> count(img.channels());
	for (int c = 0; c < img.channels(); ++c)
	{
		count[c] = img.height() * img.width();
		for (int i = 0; i < 256; ++i) hist[c][i] = 0;
		for (int h = 0; h < img.height(); ++h)
		{
			for (int w = 0; w < img.width(); ++w)
			{
				++hist[c][img.data(h, w, c)];
			}
		}
	}
	// get p
	std::vector<std::array<Image::Datum, 256>> LUT(img.channels());
	for (int c = 0; c < img.channels(); ++c)
	{
		LUT[c][0] = static_cast<Image::Datum>(1.0 * hist[c][0] / count[c] * 255);
		int sum = hist[c][0];
		for (int i = 1; i <= 255; ++i)
		{
			sum += hist[c][i];
			LUT[c][i] = static_cast<Image::Datum>(1.0 * sum / count[c] * 255);
		}
	}

	// equalize
	Image result(img.width(), img.height(), img.channels());
	for (int c = 0; c < img.channels(); ++c)
	{
		for (int h = 0; h < img.height(); ++h)
		{
			for (int w = 0; w < img.width(); ++w)
			{
				result.data(h, w, c) = LUT[c][img.data(h, w, c)];
			}
		}
	}

	return std::move(result);
}

void seeta::fill(Image& img, const Point& point, const Image& patch)
{
	if (img.channels() != patch.channels())
	{
		throw std::logic_error(str("Can not file image with mismatch channels ", img.channels(), " vs ", patch.channels()));
	}

	int dst_y_start = std::max<int>(0, point.y);
	int dst_y_end = std::min<int>(img.height(), point.y + patch.height());
	int src_y_start = dst_y_start - point.y;
	// int src_y_end = src_y_start + (dst_y_end - dst_y_start);
	int copy_times = dst_y_end - dst_y_start;
	if (copy_times <= 0) return;
	int dst_x_start = std::max<int>(0, point.x);
	int dst_x_end = std::min<int>(img.width(), point.x + patch.width());
	int src_x_start = dst_x_start - point.x;
	// int src_x_end = src_x_start + (dst_x_end - dst_x_start);
	int copy_size = (dst_x_end - dst_x_start) * patch.channels();
	if (copy_size <= 0) return;
	int dst_step = img.width() * img.channels();
	int src_step = patch.width() * patch.channels();
	auto *dst_ptr = &img.data(dst_y_start, dst_x_start, 0);
	const auto *src_ptr = &patch.data(src_y_start, src_x_start, 0);
	for (int i = 0; i < copy_times; ++i)
	{
		CopyData(dst_ptr, src_ptr, copy_size);
		dst_ptr += dst_step;
		src_ptr += src_step;
	}
}

void seeta::fill(Image& img, const Rect& rect, const Image& patch)
{
	Image fixed_patch = patch;
	if (patch.width() != rect.width || patch.height() != rect.height)
	{
		fixed_patch = resize(patch, rect);
	}
	fill(img, Point(rect), fixed_patch);
}

/*

const seeta::Image seeta::crop_face(const Image& img, const Landmarks& marks, const MeanShape& target, const Size& finalSize, Landmarks* finalMarks)
{
	Image result(finalSize.width, finalSize.height, img.channels());
	// prepare data
	std::unique_ptr<float[]> points(new float[marks.points.size() * 2]);
	std::unique_ptr<float[]> mean_shape(new float[marks.points.size() * 2]);
	std::unique_ptr<float[]> final_points;
	if (finalMarks)
	{
		final_points.reset(new float[marks.points.size() * 2]);
	}
	for (int i = 0; i < marks.points.size(); ++i)
	{
		points[i * 2] = marks.points[i].x;
		points[i * 2 + 1] = marks.points[i].y;
		mean_shape[i * 2] = target.marks.points[i].x;
		mean_shape[i * 2 + 1] = target.marks.points[i].y;
	}
	face_crop_core(
		img.data(), img.width(), img.height(), img.channels(),
		result.data(), target.size.width, target.size.height,
		points.get(), marks.points.size(),
		mean_shape.get(), target.size.width, target.size.height,
		(finalSize.height - target.size.height) / 2,
		(finalSize.height - target.size.height) / 2,
		(finalSize.width - target.size.width) / 2,
		(finalSize.width - target.size.width) / 2,
		finalMarks ? final_points.get() : nullptr,
		BICUBIC);
	if (finalMarks)
	{
		finalMarks->points.resize(marks.points.size());
		for (int i = 0; i < marks.points.size(); ++i)
		{
			finalMarks->points[i].x = final_points.get()[2 * i];
			finalMarks->points[i].y = final_points.get()[2 * i + 1];
		}
	}
	return result;
}
*/
