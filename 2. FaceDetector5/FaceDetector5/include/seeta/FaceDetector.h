#pragma once

#include "Struct.h"
#include "CFaceInfo.h"

#include <string>
#include <vector>

#define SEETA_FACE_DETECTOR_MAJOR_VERSION 5
#define SEETA_FACE_DETECTOR_MINOR_VERSION 1
#define SEETA_FACE_DETECTOR_SINOR_VERSION 0

namespace seeta
{
	namespace v510
	{

		class FaceDetector
		{
		public:
			/**
			 * \brief ����ģ���ļ�
			 * \param setting ģ���ļ�
			 */
			SEETA_API explicit FaceDetector(const SeetaModelSetting &setting);
			/**
			 * \brief ����ģ���ļ��������ü����ں˴�С
			 * \param setting ģ���ļ�
			 * \param core_size ���ü����ں˴�С��ԽСռ����ԴԽ��
			 */
			SEETA_API explicit FaceDetector(const SeetaModelSetting &setting, const SeetaSize &core_size);
            SEETA_API ~FaceDetector();

            /**
            * \brief
            * \param level
            * \return older level setting
            * \note level:
            *  DEBUG = 1,
            *  STATUS = 2,
            *  INFO = 3,
            *  FATAL = 4,
            */
            SEETA_API static int SetLogLevel(int level);

            SEETA_API static void SetSingleCalculationThreads(int num);

			/**
			 * \brief �������
			 * \param [in] image ����ͼ����Ҫ RGB ��ɫͨ��
			 * \return ��⵽��������SeetaRotateFaceInfo������
			 * \note �˺�����֧�ֶ��̵߳��ã��ڶ��̻߳�������Ҫ������Ӧ�� FaceDetector �Ķ���ֱ���ü�⺯��
			 * \seet SeetaRotateFaceInfo, SeetaImageData
			 */
            SEETA_API SeetaFaceInfoArray Detect(const SeetaImageData &image) const;

			/**
			 * \brief ������С����
			 * \param [in] size ��С�ɼ���������С��Ϊ������͸߳˻��Ķ��θ�ֵ
			 * \note ��������Ϊ 20��С�� 20 ��ֵ�ᱻ����
			 */
			SEETA_API void SetMinFaceSize(int32_t size);

			/**
			 * \brief ��ȡ��С����
			 * \return ��С�ɼ���������С��Ϊ������͸߳˻��Ķ��θ�ֵ
			 */
			SEETA_API int32_t GetMinFaceSize() const;

			/**
			 * \brief ����ͼ������������ű���
			 * \param [in] factor ���ű���
			 * \note ��ֵ��СΪ 1.414��С�� 1.414 ��ֵ�ᱻ����
			 */
			SEETA_API void SetImagePyramidScaleFactor(float factor);

			/**
			 * \brief ��ȡͼ������������ű���
			 * \return ���ű���
			 */
            SEETA_API float GetImagePyramidScaleFactor() const;

			/**
			 * \brief ���ü�����·�����������ֵ
			 * \param [in] thresh1 ��һ����ֵ
			 * \param [in] thresh2 �ڶ�����ֵ
			 * \param [in] thresh3 ��������ֵ
			 * \note Ĭ���Ƽ�Ϊ��0.67, 0.4, 0.95
			 */
			SEETA_API void SetScoreThresh(float thresh1, float thresh2, float thresh3);

			/**
			 * \brief ��ȡ������·�����������ֵ
			 * \param [out] thresh1 ��һ����ֵ
			 * \param [out] thresh2 �ڶ�����ֵ
			 * \param [out] thresh3 ��������ֵ
			 * \note ��������Ϊ nullptr����ʾ��ȡ��ֵ
			 */
			SEETA_API void GetScoreThresh(float *thresh1, float *thresh2, float *thresh3) const;

			/**
			 * \brief �Ƿ����ȶ�ģʽ������������
			 * \param stable �Ƿ��ȶ�
			 * \note Ĭ���ǲ����ȶ�ģ�͹�����
			 * \note ֻ������Ƶ����������ʱ����ʹ�ô˷���
			*/
			SEETA_API void SetVideoStable(bool stable = true);

			/**
			 * \brief ��ȡ��ǰ�Ƿ����ȶ�����ģʽ
			 * \return �Ƿ��ȶ�
			 */
			SEETA_API bool GetVideoStable() const;

		private:
			FaceDetector(const FaceDetector &other) = delete;
			const FaceDetector &operator=(const FaceDetector &other) = delete;

		private:
			void *m_impl;
		};
	}
    using namespace v510;
}
