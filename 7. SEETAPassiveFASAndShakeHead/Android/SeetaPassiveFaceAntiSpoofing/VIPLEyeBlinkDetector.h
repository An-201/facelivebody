#pragma once

#define VIPL_EYE_BLINK_DETECOTR_MAJOR_VERSION     1
#define VIPL_EYE_BLINK_DETECOTR_MINOR_VERSION     2
#define VIPL_EYE_BLINK_DETECOTR_SUBMINOR_VERSION  0

#define _SDK_MIN_MSC_VER 1800
#define _SDK_MAX_MSC_VER 1900
#if defined(_MSC_VER)
#if _MSC_VER < _SDK_MIN_MSC_VER || _MSC_VER > _SDK_MAX_MSC_VER
#error "Unsupported MSVC. Please use VS2013(v120) or compatible VS2015(v140)."
#endif // _MSC_VER < 1800 || _MSC_VER > 1900
#endif // defined(_MSC_VER)

#include "VIPLStruct.h"
#include <memory>
#include <string>
#include <array>

class VIPLEyeBlinkDetectorCore;

#if _MSC_VER >= 1600
extern template std::shared_ptr<VIPLEyeBlinkDetectorCore>;
extern template std::array<VIPLPoint, 5>;
#endif

class VIPLEyeBlinkDetector
{
public:
	/**
	* \brief ģ�������豸
	*/
	enum Device
	{
		AUTO,	/**< �Զ���⣬������ʹ�� GPU */
		CPU,	/**< ʹ�� CPU ���� */
		GPU,	/**< ʹ�� GPU ���� */
	};

	static const int LEFT_EYE = 1;
	static const int RIGHT_EYE = 2;

	VIPL_API VIPLEyeBlinkDetector(const char *model_path, Device device = AUTO);
	VIPL_API ~VIPLEyeBlinkDetector();

	/**
	 * \brief ���գ��
	 * \param image ԭʼ��ɫͼ��
	 * \param points ��λ�������㣬��Ҫǰ������ֱ��ʾ���ۺ���������
	 * \return ���ض���������ʾ�Ƿ�գ�ۣ����۱��ϻ��LEFT_EYEλ��Ϊ1�����۱��ϻ��RIGHT_EYEλ��Ϊ1��
	 */
	VIPL_API int Detect(const VIPLImageData &image, const VIPLPoint *points);

	/**
	 * ��⵱ǰ״̬�Ƿ����˫�ۣ�����һ���գ�ۼ��.
	 * \return trueΪ����
	 */
	VIPL_API bool ClosedEyes(const VIPLImageData &image, const VIPLPoint *points);

private:
	VIPLEyeBlinkDetector(const VIPLEyeBlinkDetector &other) = delete;
	const VIPLEyeBlinkDetector &operator=(const VIPLEyeBlinkDetector &other) = delete;

private:
	std::shared_ptr<VIPLEyeBlinkDetectorCore> impl;
};

