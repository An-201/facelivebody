#ifndef _SEETA_FACE_RECOGNIZER_H
#define _SEETA_FACE_RECOGNIZER_H

#define VIPL_FACE_RECOGNIZER_MAJOR_VERSION 5
#define VIPL_FACE_RECOGNIZER_MINOR_VERSION 0
#define VIPL_FACE_RECOGNIZER_SUBMINOR_VERSION 0
#define VIPL_FACE_RECOGNIZER_PATH_VERSION 7

#define _SDK_MIN_MSC_VER 1800
#define _SDK_MAX_MSC_VER 1900
#if defined(_MSC_VER)
#if _MSC_VER < _SDK_MIN_MSC_VER || _MSC_VER > _SDK_MAX_MSC_VER
#error "Unsupported MSVC. Please use VS2013(v120) or compatible VS2015(v140)."
#endif // _MSC_VER < 1800 || _MSC_VER > 1900
#endif // defined(_MSC_VER)

#include "VIPLStruct.h"
#include <vector>

typedef float* FaceFeatures;

typedef VIPLPoint VIPLPoint5[5];

class VIPLFaceRecognizerModel
{
public:
	friend class VIPLFaceRecognizer;

	VIPL_API VIPLFaceRecognizerModel(const char *model_path, int device);
	VIPL_API ~VIPLFaceRecognizerModel();
private:
	void *m_impl;
};

class VIPLFaceRecognizer {
public:
	class Param;
	VIPL_API const Param *GetParam() const;

	VIPL_API explicit VIPLFaceRecognizer(const Param *param);

	VIPL_API explicit  VIPLFaceRecognizer(const VIPLFaceRecognizerModel &model);

	VIPL_API static void SetNumThreads(int num);

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
	* \brief ����ʶ����
	* \param [in] modelPath ʶ����ģ��·��
	* \note ʶ����ģ��һ��Ϊ VIPLFaceRecognizer5.0.XXX.dat
	* \note ���ֻ��ʹ�������ü��Ĳ��֣�����Ҫ����ģ��
	* \note Ĭ�ϻ��� AUTO ģʽʹ�ü����豸
	*/
	VIPL_API explicit VIPLFaceRecognizer(const char* modelPath = NULL);

	/**
	* \brief ����ʶ����
	* \param [in] modelPath ʶ����ģ��·��
	* \param [in] device ʹ�õļ����豸
	* \note ʶ����ģ��һ��Ϊ VIPLFaceRecognizer5.0.XXX.dat
	* \note ���ֻ��ʹ�������ü��Ĳ��֣�����Ҫ����ģ��
	*/
	VIPL_API explicit VIPLFaceRecognizer(const char* modelPath, Device device);

	/**
	* \brief ����ʶ����
	* \param [in] modelBuffer ʶ��ģ�͵��ڴ����
	* \param [in] bufferLength ʶ��ģ�͵��ڴ泤��
	* \note ʶ����ģ��һ��Ϊ VIPLFaceRecognizer5.0.XXX.dat
	* \note ���ֻ��ʹ�������ü��Ĳ��֣�����Ҫ����ģ��
	*/
	VIPL_API explicit VIPLFaceRecognizer(const char* modelBuffer, size_t bufferLength, Device device = AUTO);

	VIPL_API ~VIPLFaceRecognizer();

	/**
	* \brief Ϊʶ��������ģ�ͣ���ж��ԭ����ģ��
	* \param [in] modelPath ʶ��ģ��·��
	* \return ���سɹ��󷵻���
	* \note �˺�����Ϊ���ڹ����ʱ��û�м���ģ�͵�����µ���
	* \note Ĭ�ϻ��� AUTO ģʽʹ�ü����豸
	*/
	VIPL_API bool LoadModel(const char* modelPath);

	/**
	* \brief Ϊʶ��������ģ�ͣ���ж��ԭ����ģ��
	* \param [in] modelPath ʶ��ģ��·��
	* \param [in] device ʹ�õļ����豸
	* \return ���سɹ��󷵻���
	* \note �˺�����Ϊ���ڹ����ʱ��û�м���ģ�͵�����µ���
	*/
	VIPL_API bool LoadModel(const char* modelPath, Device device);

	/**
	* \brief Ϊʶ��������ģ�ͣ���ж��ԭ����ģ��
	* \param [in] modelBuffer ʶ��ģ�͵��ڴ����
	* \param [in] bufferLength ʶ��ģ�͵��ڴ泤��
	* \param [in] device ʹ�õļ����豸
	* \return ���سɹ��󷵻���
	* \note �˺�����Ϊ���ڹ����ʱ��û�м���ģ�͵�����µ���
	*/
	VIPL_API bool LoadModel(const char* modelBuffer, size_t bufferLength, Device device = AUTO);

	/**
	 * \brief ��ȡʶ������ȡ����ά��
	 * \return ����ά��
	 */
	VIPL_API uint32_t GetFeatureSize();

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
		const VIPLPoint5 &llpoint,
		const VIPLImageData &dstImg,
		uint8_t posNum = 1);

	/**
	 * \brief �ڲü��õ���������ȡ����
	 * \param [in] cropImg �ü��õ���������ɫ����ʹ�� CropFace ���߶�Ӧ����
	 * \param [out] feats ����ȡ������ſռ�
	 * \return ֻ����ȡ�ɹ���ŷ�����
	 */
	VIPL_API bool ExtractFeature(const VIPLImageData &cropImg,
		FaceFeatures const feats);

	/**
	 * \brief �ڲü��õ���������ȡ��������һ���ģ�
	 * \param [in] cropImg �ü��õ���������ɫ����ʹ�� CropFace ���߶�Ӧ����
	 * \param [out] feats ����ȡ������ſռ�
	 * \return ֻ����ȡ�ɹ���ŷ�����
	 */
	VIPL_API bool ExtractFeatureNormalized(const VIPLImageData &cropImg,
		FaceFeatures const feats);

	/**
	 * \brief ��ԭʼͼ���ϣ����ݶ�λ����ȡ����
	 * \param [in] srcImg ԭʼͼ�񣬲�ɫ
	 * \param [in] llpoint ԭʼͼ�������������㣬5��
	 * \param [out] feats ����ȡ������ſռ�
	 * \param [in] posNum ������̬�������������˰汾������
	 * \return ֻ����ȡ�ɹ���ŷ�����
	 */
	VIPL_API bool ExtractFeatureWithCrop(const VIPLImageData &srcImg,
		const VIPLPoint5 &llpoint,
		FaceFeatures const feats,
		uint8_t posNum = 1);

	/**
	 * \brief ��ԭʼͼ���ϣ����ݶ�λ����ȡ��������һ���ģ�
	 * \param [in] srcImg ԭʼͼ�񣬲�ɫ
	 * \param [in] llpoint ԭʼͼ�������������㣬5��
	 * \param [out] feats ����ȡ������ſռ�
	 * \param [in] posNum ������̬�������������˰汾������
	 * \return ֻ����ȡ�ɹ���ŷ�����
	 */
	VIPL_API bool ExtractFeatureWithCropNormalized(const VIPLImageData &srcImg,
		const VIPLPoint5 &llpoint,
		FaceFeatures const feats,
		uint8_t posNum = 1);

	/**
	 * \brief �������� fc1 �� fc2 �����ƶ�
	 * \param [in] fc1 ��������1
	 * \param [in] fc2 ��������2
	 * \param [in] dim ����ά��
	 * \return ���ƶ�
	 * \note Ĭ������ά��Ӧ���� GetFeatureSize() �ķ���ֵ��������ǣ�����Ҫ�����Ӧ����������
	 */
	VIPL_API float CalcSimilarity(FaceFeatures const fc1,
		FaceFeatures const fc2,
		long dim = -1);

	/**
	 * \brief �������� fc1 �� fc2 �����ƶ�
	 * \param [in] fc1 ��������1����һ���ģ�
	 * \param [in] fc2 ��������2����һ���ģ�
	 * \param [in] dim ����ά��
	 * \return ���ƶ�
	 * \note Ĭ������ά��Ӧ���� GetFeatureSize() �ķ���ֵ��������ǣ�����Ҫ�����Ӧ����������
	 * \note �˼������ƶȺ������������һ���������������ɺ�׺ Normalized �ĺ�����ȡ������
	 */
	VIPL_API float CalcSimilarityNormalized(FaceFeatures const fc1,
		FaceFeatures const fc2,
		long dim = -1);

	/**
	 * \brief ������󵥴�����������
	 * \param max_batch ���������
	 * \return ����֮ǰ������
	 * \note ע��˺�������֮�����е�ʶ�������󶼻�ĳ�������ã���ʱ��֧�ֶ�ÿ����������
	 */
	VIPL_API static int SetMaxBatchGlobal(int max_batch);

	/**
	 * \brief ��ȡ��ǰʶ���������������������
	 * \return ���������
	 */
	VIPL_API int GetMaxBatch();

	/**
	* \brief ������ʶ���������߳���
	* \param core_number �����߳���
	* \return ����֮ǰ������
	* \note ע��˺�������֮�����е�ʶ�������󶼻�ĳ�������ã���ʱ��֧�ֶ�ÿ����������
	*/
	VIPL_API static int SetCoreNumberGlobal(int core_number);

	/**
	* \brief ��ȡ��ǰʶ���������߳���
	* \return �����߳���
	*/
	VIPL_API int GetCoreNumber();

	/**
	 * \brief ������ȡ����
	 * \param faces �ü��õ�ÿһ������
	 * \param [out] feats �����ȡ�����Ŀռ䣬��СΪ faces.size() * GetFeatureSize()���� i * GetFeatureSize() �±꿪ʼ�� GetFeatureSize() �������ǵ�i����������
	 * \return ֻ����ȡ�ɹ���ŷ�����
	 */
	VIPL_API bool ExtractFeature(const std::vector<VIPLImageData> &faces, float *feats, bool normalization = false);

	/**
	 * \brief ������ȡ��������һ���ģ�
	 * \param faces �ü��õ�ÿһ������
	 * \param [out] feats �����ȡ�����Ŀռ䣬��СΪ faces.size() * GetFeatureSize()���� i * GetFeatureSize() �±꿪ʼ�� GetFeatureSize() �������ǵ�i����������
	 * \return ֻ����ȡ�ɹ���ŷ�����
	 */
	VIPL_API bool ExtractFeatureNormalized(const std::vector<VIPLImageData> &faces, float *feats);

	/**
	 * \brief ������ȡ����
	 * \param images ԭʼͼ��
	 * \param points ԭʼͼ���ϵ�ÿһ����������СΪ images.size() * 5���� i * 5 �±꿪ʼ�� 5 ���ǵ� i ��ͼƬ�ϵ�������
	 * \param [out] feats �����ȡ�����Ŀռ䣬��СΪ images.size() * GetFeatureSize()���� i * GetFeatureSize() �±꿪ʼ�� GetFeatureSize() �������ǵ�i����������
	 * \return ֻ����ȡ�ɹ���ŷ�����
	 */
	VIPL_API bool ExtractFeatureWithCrop(const std::vector<VIPLImageData> &images, const std::vector<VIPLPoint> &points, float *feats, bool normalization = false);

	/**
	 * \brief ������ȡ��������һ���ģ�
	 * \param images ԭʼͼ��
	 * \param points ԭʼͼ���ϵ�ÿһ����������СΪ images.size() * 5���� i * 5 �±꿪ʼ�� 5 ���ǵ� i ��ͼƬ�ϵ�������
	 * \param [out] feats �����ȡ�����Ŀռ䣬��СΪ images.size() * GetFeatureSize()���� i * GetFeatureSize() �±꿪ʼ�� GetFeatureSize() �������ǵ�i����������
	 * \return ֻ����ȡ�ɹ���ŷ�����
	 */
	VIPL_API bool ExtractFeatureWithCropNormalized(const std::vector<VIPLImageData> &images, const std::vector<VIPLPoint> &points, float *feats);

private:
	VIPLFaceRecognizer(const VIPLFaceRecognizer &other) = delete;
	const VIPLFaceRecognizer &operator=(const VIPLFaceRecognizer &other) = delete;

private:
	class Recognizer;
	friend class VIPLFaceRecognizerModel;
	Recognizer* recognizer;
};
#endif // _SEETA_FACE_RECOGNIZER_H
