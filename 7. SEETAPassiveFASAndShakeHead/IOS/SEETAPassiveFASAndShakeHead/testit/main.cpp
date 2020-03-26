#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <VIPLFaceDetector.h>
#include <VIPLPointDetector.h>
#include"SEETAPassiveFASAndShakeHead.h"
using namespace std;
using namespace cv;
using VIPLPoint5 = VIPLPoint[5];
static cv::Mat noface;	//û������
static cv::Mat boxMat;	//��ʼ��Ĭ������׶�
static cv::Mat passMat;			//ͨ��ʱ���ӵĿ�
static cv::Mat notPassMat;	//δͨ��ʱ���ӵĿ�
static cv::Mat pleaseTrunedMat;	//��תͷ��ʾ��
static cv::Mat pleaseBlinkMat;	//��գ����ʾ��
static cv::Mat pleaseNodMat;	//���ͷ��ʾ��
static cv::Mat pleaseFaceTheCameraMat;	//�����泯������ͷ
static cv::Mat verifyingMat;	//�����泯������ͷ
void loadImgAndResize(const cv::Size imgSize)
{
	noface = cv::imread("ui/noface.png");
	boxMat = cv::imread("ui/box.png");				//������ʱ���ӵĿ�
	passMat = cv::imread("ui/pass.png");			//ͨ��ʱ���ӵĿ�
	notPassMat = cv::imread("ui/notPass.png");	//δͨ��ʱ���ӵĿ�
	pleaseTrunedMat = cv::imread("ui/pleaseTurn.png");	//δͨ��ʱ���ӵĿ�
	pleaseBlinkMat = cv::imread("ui/pleaseBlink.png");	//
	pleaseNodMat = cv::imread("ui/pleaseNod.png");	//
	pleaseFaceTheCameraMat = cv::imread("ui/pleaseFaceTheCamera.png"); 
    verifyingMat = cv::imread("ui/verifying.png");
	cv::resize(boxMat, boxMat, imgSize);	//resize��Ƶ��
	cv::resize(passMat, passMat, imgSize);
	cv::resize(notPassMat, notPassMat, imgSize);
	cv::resize(pleaseTrunedMat, pleaseTrunedMat, imgSize);
	cv::resize(pleaseBlinkMat, pleaseBlinkMat, imgSize);
	cv::resize(pleaseNodMat, pleaseNodMat, imgSize);
	cv::resize(pleaseFaceTheCameraMat, pleaseFaceTheCameraMat, imgSize);
    cv::resize(verifyingMat, verifyingMat, imgSize);
}
/**
* \brief ������ͷ�������Ƭ��ui�����ں�
* \param frame ��ǰ����
* \param systemState ϵͳ��ǰ״̬
*/
void addWeightedToImage(cv::Mat& frame, const int &systemState)
{
	switch (systemState)
	{
	case noFace:
	{
		cv::addWeighted(noface, 1.0, frame, 0.65, 35.0, frame);
		break;
	}
	case detecting:
	{
		cv::addWeighted(boxMat, 1.0, frame, 0.65, 35.0, frame);
		break;
	}
	case passed:
	{
		cv::addWeighted(passMat, 1.0, frame, 0.65, 35.0, frame);
		break;
	}
	case notPass:
	{
		cv::addWeighted(notPassMat, 1.0, frame, 0.65, 35.0, frame);
		break;
	}
	case pleaseTrun:
	{
		cv::addWeighted(pleaseTrunedMat, 1.0, frame, 0.65, 35.0, frame);
		break;
	}
	case pleaseBlink:
	{
		cv::addWeighted(pleaseBlinkMat, 1.0, frame, 0.65, 35.0, frame);
		break;
	}
	case pleaseNodHead:
	{
		cv::addWeighted(pleaseNodMat, 1.0, frame, 0.65, 35.0, frame);
		break;
    }
    case pleaseFaceTheCamera:
    {
        cv::addWeighted(pleaseFaceTheCameraMat, 1.0, frame, 0.65, 35.0, frame);
        break;
    }
    case verifying:
    {
        cv::addWeighted(verifyingMat, 1.0, frame, 0.65, 35.0, frame);
        break;
    }
	default:
	{
		cout << "ERROR: system state is false!" << endl;
	}
	}
}
bool viplFindMaxFace(const std::vector<VIPLFaceInfo>& faces, int &maxFaceID)
{
	if (faces.size() > 0)
	{
		maxFaceID = 0;
		double faceMaxWidth = faces[0].width;
		for (int j = 1; j < faces.size(); ++j) {
			if (faceMaxWidth < faces[j].width) {
				faceMaxWidth = faces[j].width;
				maxFaceID = j;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}


cv::Mat vipl2mat(const VIPLImageData &vimg)
{
	return cv::Mat(vimg.height, vimg.width, CV_8UC(vimg.channels), vimg.data);
}

int main()
{
	const double value1_Threshold = 0.3;		//�������������һ����ֵ
	const double value2_Threshold = 0.04;	//������������ڶ�����ֵ
	const float shakeHeadAngleThreshold = 40;	//תͷͨ���ĽǶ�
	const float nodHeadAngleThreshold = 30;	//תͷͨ���ĽǶ�
	const int totalFrameNum = 80;	//�ó���һ������֡�����ﵽ�趨֡������ͨ�����Ļ����˳�����
	const int firstPhaseFrameNum = 10;
	std::string ModelPath = "./model/";
    SystemState state = noFace;

    SEETAPassiveFASAndShakeHead::ModelSetting model_setting;
    model_setting.PE = ModelPath + "/VIPLPoseEstimation1.1.0.ext.dat";
    model_setting.EBD = ModelPath + "/VIPLEyeBlinkDetector1.3.99x99.raw.dat";
    model_setting.FAS = ModelPath + "/_acc97_res14YuvAlignment_68000_0622.bin";
    model_setting.FR = ModelPath + "/VIPLFaceAntiSpoofing.light.sta";
    std::shared_ptr<SEETAPassiveFASAndShakeHead>  processor = std::make_shared<SEETAPassiveFASAndShakeHead>(model_setting, shakeHeadAngleThreshold, nodHeadAngleThreshold, value1_Threshold, value2_Threshold, state, firstPhaseFrameNum, totalFrameNum);
	
	// û�ж�����ֻ������
	// processor->set_actions({ });
	// ֻ��ⵥһ����
	// processor->set_actions({ pleaseBlink });
	// ҡͷ��գ�۶�ѡһ
	// processor->set_actions({ pleaseTrun, pleaseBlink });
	// ҡͷ��գ�ۡ���ͷ��ѡһ
	processor->set_actions({ pleaseTrun, pleaseBlink, pleaseNodHead });
	// Ĭ������ҡͷ���
	//processor->set_actions({ pleaseNodHead });
	VIPLFaceDetector FaceDetct((ModelPath + "VIPLFaceDetector5.1.0.640x480.dat").c_str());//�������ĳ�ʼ��
	FaceDetct.SetMinFaceSize(80);
	FaceDetct.SetScoreThresh(0.7, 0.7, 0.85);
	FaceDetct.SetImagePyramidScaleFactor(1.414);
	VIPLPointDetector PointDetect((ModelPath + "VIPLPointDetector5.0.pts5.dat").c_str());//�ؼ�����ģ�ͳ�ʼ��
	FaceDetct.SetVideoStable(true);
	VIPLPoint5 points;	//��������5������
	for (int test_n = 0; test_n < 1000; ++test_n) {
		processor->reset(state);
		cv::Mat frame;
		VideoCapture capture(0);//����Ƶ�ļ�
		int videoWidth = 640;	//��Ƶ����ʾ�Ŀ�
		int videoHeight = 480;	//��Ƶ����ʾ�ĸ�

		capture.set(CV_CAP_PROP_FRAME_WIDTH, videoWidth);	//���ö�������ͷͼ��ĳ��Ϳ�������Ҫ�������䣩
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, videoHeight);
		if (!capture.isOpened())	//����Ƿ�������:�ɹ���ʱ��isOpened����ture  
		cout << "fail to open!" << endl;
		loadImgAndResize(cv::Size(videoWidth, videoHeight));
		while (true)
		{
			if (!capture.read(frame))
			{
				cout << "���ܴ���Ƶ�ж�ȡ֡��" << endl;
				break;
			}
			flip(frame, frame, 1);	//������ת����ͷ��ʹ������ͼ����˵ķ���һ��
			if (frame.channels() == 4){	//���Ϊ4ͨ����תΪ3ͨ����rgbͼ��
				cv::cvtColor(frame, frame, CV_RGBA2BGR);
			}
			VIPLImageData VIPLImage(frame.cols, frame.rows, frame.channels());
			VIPLImage.data = frame.data;
			std::vector<VIPLFaceInfo> faces = FaceDetct.Detect(VIPLImage);

			int maxFaceID = 0;	//������������
			if (viplFindMaxFace(faces,maxFaceID))
			{
				PointDetect.DetectLandmarks(VIPLImage, faces[maxFaceID], points);
				processor->detecion(VIPLImage, faces[maxFaceID], points, state);
				
				double value1, value2, yaw, pitch;
				processor->getLog(value1, value2, yaw, pitch);
				std::cout << "value1: " << value1 << ", value2: " << value2 << "yaw: "<<yaw<<" pitch: "<<pitch<<std::endl;

				for (int i = 0; i < 5; ++i)
				{
					cv::circle(frame, cv::Point(points[i].x, points[i].y), 2, cv::Scalar(0, 255, 0), -1);
				}
			}
			else	//û������������
			{
				processor->reset(state);
			}
			for (int i = 0; i < faces.size(); i++)
			{
				rectangle(frame, cv::Rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height), cv::Scalar(232, 162, 0), 2, 8, 0);//����������
			}
			addWeightedToImage(frame, state);
			cv::imshow("SeetaFaceAntiSpoofing", frame);

            VIPLFaceInfo info;
            auto face = vipl2mat(processor->getSnapshot(info));
			if (!face.empty())
            {
                rectangle(face, cv::Rect(info.x, info.y, info.width, info.height), cv::Scalar(232, 162, 0), 2, 8, 0);//����������
				cv::imshow("Face", face);
			}
			if (cv::waitKey(30) == 27) break;
			if (state == passed || state == notPass)
			{
				break;
			}
		}
		cv::waitKey(100);
	}
}