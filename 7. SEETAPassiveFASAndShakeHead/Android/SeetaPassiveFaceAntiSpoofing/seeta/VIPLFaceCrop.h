#ifndef _VIPL_FACE_CROP_H
#define _VIPL_FACE_CROP_H

#include "VIPLStruct.h"

namespace VIPL
{

	/**
	 * \brief �����ü�����
	 */
	enum CROP_METHOD
	{
		BY_LINEAR,	///< ���Բ�ֵ
		BY_BICUBIC	///< ����˫���Բ�ֵ
	};

	/**
	 * \brief ��ȡӵ�ж�Ӧ��������Ŀ��ƽ������ģ��
	 * \param num ƽ������ģ�����������Ŀ
	 * \param mean_shape ƽ������ģ�͵�������
	 * \param mean_shape_size ƽ������ģ�͵Ĵ�С
	 * \param id ƽ������ģ�͵����
	 * \return ��Ӧ�����Ƿ���Ԥ��ƽ������ģ��
	 * \note [num, id] ��Ӧ��ȡ��ƽ������ģ��
	 *       [5, 0]: ����ʶ��
	 *       [5, 1]: ����ʶ��
	 */
	VIPL_API bool FaceMeanShape(VIPLPoint *mean_shape, int num, int *mean_shape_size, int id = 0);
	
	/**
	 * \brief ������ź��ƽ������ģ��
	 * \param num ����������
	 * \param mean_shape ƽ������ģ���е�������
	 * \param scaler ���ų߶�
	 */
	VIPL_API void ResizeMeanShape(VIPLPoint *mean_shape, int num, double scaler);

	/**
	 * \brief ���������ü�
	 * \param src_img ԭʼͼ��
	 * \param dst_img Crop �õ�����ͼ��
	 * \param points ������������
	 * \param num �������������
	 * \param mean_shape ƽ������ģ��
	 * \param mean_shape_size ƽ������ģ�͵Ĵ�С
	 * \param method ��������
	 * \param final_points �����任���������
	 * \param final_size ���Ĵ�С�������ƽ������ģ��
	 * \return ��� Crop �ɹ��򷵻���
	 */
	VIPL_API bool FaceCrop(
		const VIPLImageData &src_img, VIPLImageData &dst_img,
		const VIPLPoint *points, int num,
		const VIPLPoint *mean_shape, int mean_shape_size,
		CROP_METHOD method = BY_LINEAR,
		VIPLPoint *final_points = nullptr,
		int final_size = -1);
}

#endif
