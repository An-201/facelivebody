#ifndef _SEETA_FACE_COMPARER_H
#define _SEETA_FACE_COMPARER_H

#include "VIPLStruct.h"
#include <vector>

class VIPLFaceComparer {
public:
    VIPL_API explicit VIPLFaceComparer(const char* modelPath = NULL);

    VIPL_API ~VIPLFaceComparer();


    /**
    * \brief Ϊʶ��������ģ�ͣ���ж��ԭ����ģ��
    * \param [in] modelPath ʶ��ģ��·��
    * \return ���سɹ��󷵻���
    * \note �˺�����Ϊ���ڹ����ʱ��û�м���ģ�͵�����µ���
    * \note Ĭ�ϻ��� AUTO ģʽʹ�ü����豸
    */
    VIPL_API bool LoadModel(const char* modelPath);

	/**
	 * \brief �������� fc1 �� fc2 �����ƶ�
	 * \param [in] fc1 ��������1
	 * \param [in] fc2 ��������2
	 * \param [in] dim ����ά��
	 * \return ���ƶ�
	 * \note Ĭ������ά��Ӧ���� GetFeatureSize() �ķ���ֵ��������ǣ�����Ҫ�����Ӧ����������
	 */
	VIPL_API float CalcSimilarity(const float *fc1, const float *fc2, long dim = -1);

	/**
	 * \brief �������� fc1 �� fc2 �����ƶ�
	 * \param [in] fc1 ��������1����һ���ģ�
	 * \param [in] fc2 ��������2����һ���ģ�
	 * \param [in] dim ����ά��
	 * \return ���ƶ�
	 * \note Ĭ������ά��Ӧ���� GetFeatureSize() �ķ���ֵ��������ǣ�����Ҫ�����Ӧ����������
	 * \note �˼������ƶȺ������������һ���������������ɺ�׺ Normalized �ĺ�����ȡ������
	 */
    VIPL_API float CalcSimilarityNormalized(const float *fc1, const float *fc2, long dim = -1);

    /**
    * \brief ��ȡ����ʶ����ü�������ͼƬ���
    * \return ����ͼƬ���
    */
    VIPL_API uint32_t GetCropWidth();

    /**
    * \brief ��ȡ����ʶ����ü�������ͼƬ�߶�
    * \return ����ͼƬ�߶�
    */
    VIPL_API uint32_t GetCropHeight();

    /**
    * \brief ��ȡ����ʶ����ü�������ͼƬͨ����
    * \return ����ͼƬͨ����
    */
    VIPL_API uint32_t GetCropChannels();

    /**
    * \brief �ü�����
    * \param [in] srcImg ԭʼͼ�񣬲�ɫ
    * \param [in] llpoint ԭʼͼ�������������㣬5��
    * \param [out] dstImg Ŀ��ͼ�񣬸��ݲü���ϢԤ��������ڴ�
    * \param [in] posNum ������̬�������������˰汾������
    * \return ֻ�вü��ɹ���ŷ�����
    */
    VIPL_API bool CropFace(const VIPLImageData &srcImg,
        const VIPLPoint (&llpoint)[5],
        const VIPLImageData &dstImg,
        uint8_t posNum = 1);

private:
    VIPLFaceComparer(const VIPLFaceComparer &other) = delete;
    const VIPLFaceComparer &operator=(const VIPLFaceComparer &other) = delete;

private:
	class Data;
    Data* data;
};
#endif // _SEETA_FACE_COMPARER_H
