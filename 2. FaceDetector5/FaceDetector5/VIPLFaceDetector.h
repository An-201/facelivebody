#ifndef VIPL_FACE_DETECTOR_H_
#define VIPL_FACE_DETECTOR_H_

#define _SDK_MIN_MSC_VER 1800
#define _SDK_MAX_MSC_VER 1900

#if defined(_MSC_VER)
#if _MSC_VER < _SDK_MIN_MSC_VER || _MSC_VER > _SDK_MAX_MSC_VER
#error "Unsupported MSVC. Please use VS2013(v120) or compatible VS2015(v140)."
#endif // _MSC_VER < 1800 || _MSC_VER > 1900
#endif // defined(_MSC_VER)

#define VIPL_FACE_DETECTOR_MAJOR_VERSION     5
#define VIPL_FACE_DETECTOR_MINOR_VERSION     1
#define VIPL_FACE_DETECTOR_SUBMINOR_VERSION  0

#include <cstdint>
#include <vector>

#include "VIPLStruct.h"

#if _MSC_VER >= 1600
extern template std::vector<VIPLFaceInfo>;
#endif

/** @class VIPLFaceDetector VIPLFaceDetector.h
*  @brief The face detector.
*/
class VIPLFaceDetector
{
public:
	/**
	 * \brief ģ�������豸
	 */
	enum Device
	{
		AUTO,	/**< �Զ���⣬������ʹ�� GPU */
		CPU,	/**< ʹ�� CPU ���� */
		GPU,	/**< ʹ�� GPU ���㣬�ȼ���GPU0 */
		GPU0,   /**< Ԥ����GPU��ţ�0�ſ� */
		GPU1,
		GPU2,
		GPU3,
		GPU4,
		GPU5,
		GPU6,
		GPU7,
	};

	class CoreSize
	{
	public:
		CoreSize() : width(-1), height(-1) {}
		CoreSize(int width, int height) : width(width), height(height) {}
		
		int width;
		int height;
	};

	VIPL_API static void SetNumThreads(int num);
	
	/**
	 * \brief �������������
	 * \param [in] model_path �����·��
	 * \note Ĭ�ϻ��� AUTO ģʽʹ�ü����豸
	 */
	VIPL_API explicit VIPLFaceDetector(const char* model_path);

	/**
	 * \brief �������������
	 * \param [in] model_path �����·��
	 * \param [in] device ʹ�õļ����豸
	 */
	VIPL_API explicit VIPLFaceDetector(const char* model_path, Device device);
	
	/**
	 * \brief �������������
	 * \param [in] model_path �����·��
	 * \note Ĭ�ϻ��� AUTO ģʽʹ�ü����豸
	 */
	VIPL_API explicit VIPLFaceDetector(const char* model_path, const CoreSize &core_size);

	/**
	 * \brief �������������
	 * \param [in] model_path �����·��
	 * \param [in] device ʹ�õļ����豸
	 */
	VIPL_API explicit VIPLFaceDetector(const char* model_path, const CoreSize &core_size, Device device);
    
	VIPL_API ~VIPLFaceDetector();
    
	/**
	 * \brief �������
	 * \param [in] img ����ͼ����Ҫ RGB ��ɫͨ��
	 * \return ��⵽��������VIPLFaceInfo������
	 * \note �˺�����֧�ֶ��̵߳��ã��ڶ��̻߳�������Ҫ������Ӧ�� VIPLFaceDetector �Ķ���ֱ���ü�⺯��
	 * \seet VIPLFaceInfo, VIPLImageData 
	 */
	VIPL_API std::vector<VIPLFaceInfo> Detect(const VIPLImageData & img);

	/**
	 * \brief ������С����
	 * \param [in] size ��С�ɼ���������С��Ϊ������͸߳˻��Ķ��θ�ֵ
	 * \note ��������Ϊ 20��С�� 20 ��ֵ�ᱻ���� 
	 */
	VIPL_API void SetMinFaceSize(int32_t size);
    
	/**
	 * \brief ����ͼ������������ű���
	 * \param [in] factor ���ű���
	 * \note ��ֵ��СΪ 1.414��С�� 1.414 ��ֵ�ᱻ����
	 */
	VIPL_API void SetImagePyramidScaleFactor(float factor);

	/**
	 * \brief ���ü�����·�����������ֵ
	 * \param [in] thresh1 ��һ����ֵ
	 * \param [in] thresh2 �ڶ�����ֵ
	 * \param [in] thresh3 ��������ֵ
	 * \note Ĭ���Ƽ�Ϊ��0.62, 0.47, 0.985
	 */
    VIPL_API void SetScoreThresh(float thresh1, float thresh2, float thresh3);

    /**
    * \brief ��ȡ��С����
    * \return size ��С�ɼ���������С��Ϊ������͸߳˻��Ķ��θ�ֵ
    */
    VIPL_API int32_t GetMinFaceSize() const;

    /**
    * \brief ��ȡͼ������������ű���
    * \return factor ���ű���
    */
    VIPL_API float GetImagePyramidScaleFactor() const;

    /**
     * \brief ��ȡ������·�����������ֵ
     * \param [out] thresh1 ��һ����ֵ
     * \param [out] thresh2 �ڶ�����ֵ
     * \param [out] thresh3 ��������ֵ
     * \note ��������Ϊ nullptr����ʾ��ȡ��ֵ
     */
    VIPL_API void GetScoreThresh(float *thresh1, float *thresh2, float *thresh3) const;

	/**
	 * \brief �Ƿ����ȶ�ģʽ������������
	 * \param stable �Ƿ��ȶ�
	 * \note Ĭ���ǲ����ȶ�ģ�͹�����
	 * \note ֻ������Ƶ����������ʱ����ʹ�ô˷���
	 */
	VIPL_API void SetVideoStable(bool stable = true);

	/**
	 * \brief ��ȡ��ǰ�Ƿ����ȶ�����ģʽ
	 * \return �Ƿ��ȶ�
	 */
	VIPL_API bool GetVideoStable() const;

	VIPL_API CoreSize GetCoreSize() const;

private:
	VIPLFaceDetector(const VIPLFaceDetector &other) = delete;
	const VIPLFaceDetector &operator=(const VIPLFaceDetector &other) = delete;

private:
    void* impl_;
};

#endif  // VIPL_FACE_DETECTOR_H_
