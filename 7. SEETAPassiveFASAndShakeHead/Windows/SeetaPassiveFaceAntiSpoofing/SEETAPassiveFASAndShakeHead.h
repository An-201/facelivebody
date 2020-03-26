#pragma once

#include "VIPLStruct.h"
#include <string>
#include <memory>
#include <vector>

enum SystemState{ noFace, detecting, pleaseTrun, passed, notPass, pleaseBlink,pleaseNodHead, pleaseFaceTheCamera};	//ϵͳ��ǰ�ļ��״̬ ���ֱ�Ϊû�����������ڼ�⣬��תͷ��ͨ����δͨ��,��


class FusionFASProcess;
class SEETAPassiveFASAndShakeHead
{
public:
    class ModelSetting
    {
    public:
        std::string FAS;
        std::string PE;
        std::string EBD;
        std::string FR;
    };

    class Buffer
    {
    public:
        Buffer() {}
        Buffer(void *data, size_t size) : data(data), size(size) {}

        void *data = nullptr;
        size_t size = 0;
    };

    class BufferSetting
    {
    public:
        Buffer FAS;
        Buffer PE;
        Buffer EBD;
        Buffer FR;
    };

    VIPL_API SEETAPassiveFASAndShakeHead(const std::string ModelPath,
        const int shakeHeadAngleThreshold,
        const int nodHeadAngleThreshold,
        const double clarityTheshold,
        const double fuseThreshold,
        SystemState &systemState,
        const int firstPhaseFrameNum,
        const int detectFrameNum);

    VIPL_API SEETAPassiveFASAndShakeHead(const ModelSetting &model_setting,
        const int shakeHeadAngleThreshold,
        const int nodHeadAngleThreshold,
        const double clarityTheshold,
        const double fuseThreshold,
        SystemState &systemState,
        const int firstPhaseFrameNum,
        const int detectFrameNum);
	VIPL_API ~SEETAPassiveFASAndShakeHead();

	/**
	 * \brief ������
	 * \param VIPLImage ����ͼ��
	 * \param info ����λ��
	 * \param points5 ������
	 * \param systemState ϵͳ״̬
	 */
	VIPL_API void detecion(VIPLImageData VIPLImage, const VIPLFaceInfo &info, const VIPLPoint *points5, SystemState &systemState);

	/**
	 * \brief ����ϵͳ״̬����ʼ��һ�����
	 * \param systemState 
	 */
	VIPL_API void reset(SystemState &systemState);

	/**
	 * \brief ����Ҫ���Ķ������У������� pleaseTrun �� pleaseBlink
	 * \param actions 
	 * \return ���ö����Ƿ�ɹ�
	 * \note Ĭ�϶���ֻ�� pleaseTrun����������˲�֧�ֵĶ����������ؼ�
	 */
	VIPL_API bool set_actions(const std::vector<SystemState>& actions);

	/**
	 * \brief ��ȡ��־���ڵ���
	 * \param value1 
	 * \param value2 
	 * \param yawAngle 
	 */
	VIPL_API void getLog(double &value1, double &value2, double &yawAngle, double &pitchAngle);
	/**
	 * \brief ����ʶ���������̬�����������ͼƬ
	 * \param 
	 */
    VIPL_API VIPLImageData getSnapshot();

    /**
     * \brief ����ʶ���������̬�����������ͼƬ
     * \param
     */
    VIPL_API VIPLImageData getSnapshot(VIPLFaceInfo &face);
private:
	SEETAPassiveFASAndShakeHead(const SEETAPassiveFASAndShakeHead &other) = delete;
	const SEETAPassiveFASAndShakeHead &operator=(const SEETAPassiveFASAndShakeHead &other) = delete;
	std::shared_ptr<FusionFASProcess> impl;
};

