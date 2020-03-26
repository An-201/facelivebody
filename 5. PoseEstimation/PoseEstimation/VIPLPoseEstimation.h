#pragma once

#define VIPL_POSE_ESTIMATION_MAJOR_VERSION     1
#define VIPL_POSE_ESTIMATION_MINOR_VERSION     1
#define VIPL_POSE_ESTIMATION_SUBMINOR_VERSION  0

#define _SDK_MIN_MSC_VER 1800
#define _SDK_MAX_MSC_VER 1900
#if defined(_MSC_VER)
#if _MSC_VER < _SDK_MIN_MSC_VER || _MSC_VER > _SDK_MAX_MSC_VER
#error "Unsupported MSVC. Please use VS2013(v120) or compatible VS2015(v140)."
#endif // _MSC_VER < 1800 || _MSC_VER > 1900
#endif // defined(_MSC_VER)

#include <memory>

#include "VIPLStruct.h"

class VIPLPoseEstimationCore;

#if _MSC_VER >= 1600
extern template std::shared_ptr<VIPLPoseEstimationCore>;
#endif

/**
 * \brief ��̬������
 */
class VIPLPoseEstimation
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

	/**
	 * \brief ��̬���ƹ��캯������Ҫ�ڹ����ʱ������̬����ģ��
	 * \param [in] model_path ��̬����ģ���ļ���Ĭ������Ϊ��VIPLPoseEstimation1.0.0.ext.dat
	 * \note Ĭ�ϻ��� AUTO ģʽѡ������豸
	 */
	VIPL_API VIPLPoseEstimation(const char *model_path);

	/**
	 * \brief ��̬���ƹ��캯������Ҫ�ڹ����ʱ������̬����ģ��
	 * \param [in] model_path ��̬����ģ���ļ���Ĭ������Ϊ��VIPLPoseEstimation1.0.0.ext.dat
	 * \note Ĭ�ϻ��� AUTO ģʽѡ������豸
	 */
    VIPL_API VIPLPoseEstimation(const char *model_path, Device device);

    /**
	 * \brief ��̬���ƹ��캯���������趨ģ�Ͱ汾
	 * \param [in] device ��̬����ģ���ļ���Ĭ������Ϊ��VIPLPoseEstimation1.0.0.ext.dat
	 * \note Ĭ�ϻ��� AUTO ģʽѡ������豸
	 */
    VIPL_API VIPLPoseEstimation(Device device = AUTO);
	
    VIPL_API ~VIPLPoseEstimation();

	/**
	 * \brief ִ����̬����
	 * \param [in] src_img ����ͼ����Ҫ�ǲ�ɫͼ������ͨ���� BGR ����ѹ��
	 * \param [in] info ���������Ϣ��Ϊ�������ķ���ֵ
	 * \param [out] yaw yaw ����ĽǶ� 
	 * \param [out] pitch pitch ����ĽǶ� 
	 * \param [out] roll roll ����ĽǶ� 
	 * \return ֻ�е���̬���Ƴɹ��󣬲ŷ�����
     * \note �Ƕȵķ�ΧΪ[-90, 90]����������ͼ��ɼ��豸�ǣ���������Ƕ�Ϊ0
	 */
	VIPL_API bool Estimate(const VIPLImageData &src_img, const VIPLFaceInfo &info, float &yaw, float &pitch, float &roll);

private:
	VIPLPoseEstimation(const VIPLPoseEstimation &other) = delete;
	const VIPLPoseEstimation &operator=(const VIPLPoseEstimation &other) = delete;

private:
	std::shared_ptr<VIPLPoseEstimationCore> impl;
};

