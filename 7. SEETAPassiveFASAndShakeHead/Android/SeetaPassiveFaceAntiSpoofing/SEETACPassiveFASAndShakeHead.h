#ifndef SEETA_C_PASSIVE_FAS_AND_SHAKE_HEAD_H
#define SEETA_C_PASSIVE_FAS_AND_SHAKE_HEAD_H

#include "VIPLStruct.h"

#define VIPL_C_API extern "C" VIPL_API

struct SEETACPassiveFASAndShakeHead;

enum SeetaCSystemState
{
	NO_FACE,
	DETECTING,
	PLEASE_TURN,
	PASSED,
	NOT_PASSED,
	PLEASE_BLINK,
	PLEASE_NOD_HEAD
}; //ϵͳ��ǰ�ļ��״̬ ���ֱ�Ϊû�����������ڼ�⣬��תͷ��ͨ����δͨ��,��

VIPL_C_API SEETACPassiveFASAndShakeHead* SEETACPassiveFASAndShakeHead_New(const char* ModelPath,
                                                                          const int shakeHeadAngleThreshold,
                                                                          const int nodHeadAngleThreshold,
                                                                          const double clarityTheshold,
                                                                          const double fuseThreshold,
                                                                          SeetaCSystemState* systemState,
                                                                          const int firstPhaseFrameNum,
                                                                          const int detectFrameNum);
VIPL_C_API void SEETACPassiveFASAndShakeHead_Delete(SEETACPassiveFASAndShakeHead* object);

/**
 * \brief ������
 * \param object SEETACPassiveFASAndShakeHead_New �ķ���ֵ
 * \param VIPLImage ����ͼ��
 * \param info ����λ��
 * \param points5 ������
 * \param systemState ϵͳ״̬
 */
VIPL_C_API void SEETACPassiveFASAndShakeHead_detection(SEETACPassiveFASAndShakeHead* object, const VIPLImageData* VIPLImage, const VIPLFaceInfo* info,
                                                      const VIPLPoint* points5, SeetaCSystemState* systemState);

/**
 * \brief ����ϵͳ״̬����ʼ��һ�����
 * \param object SEETACPassiveFASAndShakeHead_New �ķ���ֵ
 * \param systemState 
 */
VIPL_C_API void SEETACPassiveFASAndShakeHead_reset(SEETACPassiveFASAndShakeHead* object, SeetaCSystemState* systemState);

/**
 * \brief ����Ҫ���Ķ������У������� pleaseTrun �� pleaseBlink
 * \param object SEETACPassiveFASAndShakeHead_New �ķ���ֵ
 * \param actions ���ö���������
 * \param actions_len ���ö�������ĳ���
 * \return ���ö����Ƿ�ɹ�
 * \note Ĭ�϶���ֻ�� pleaseTrun����������˲�֧�ֵĶ����������ؼ�
 */
VIPL_C_API int SEETACPassiveFASAndShakeHead_set_actions(SEETACPassiveFASAndShakeHead* object, const SeetaCSystemState* actions, int actions_len);

/**
 * \brief ��ȡ��־���ڵ���
 * \param object SEETACPassiveFASAndShakeHead_New �ķ���ֵ
 * \param value1 
 * \param value2 
 * \param yawAngle 
 */
VIPL_C_API void SEETACPassiveFASAndShakeHead_getLog(SEETACPassiveFASAndShakeHead* object, double* value1, double* value2, double* yawAngle,
                                                    double* pitchAngle);
/**
 * \brief ����ʶ���������̬�����������ͼƬ
 * \param object SEETACPassiveFASAndShakeHead_New �ķ���ֵ
 * \param [out] snapshot ץ��ͼƬ
 */
VIPL_C_API void SEETACPassiveFASAndShakeHead_getSnapshot(SEETACPassiveFASAndShakeHead* object, VIPLImageData *snapshot);

#endif	// SEETA_C_PASSIVE_FAS_AND_SHAKE_HEAD_H
