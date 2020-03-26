#include <iostream>
#include <fstream>
#include <chrono>

#include <VIPLPointDetector.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

VIPLImageData vipl_convert(const cv::Mat &img)
{
	VIPLImageData vimg(img.cols, img.rows, img.channels());
	vimg.data = img.data;
	return vimg;
}

int main()
{
	// 0.0 ������̬����ģ��
	std::cout << "== Load model ==" << std::endl;

	VIPLPointDetector PD("model/VIPLPointDetector5.0.pts5.ats");

	std::cout << "Load model success." << std::endl;
	std::cout << std::endl;

	// 0.1 ���ش�ʶ��ͼƬ
	std::cout << "== Load image ==" << std::endl;

	cv::Mat mat = cv::imread("1.jpg", cv::IMREAD_COLOR);
	VIPLImageData image = vipl_convert(mat);

	std::cout << "Load image: " << image.width << "x" << image.height << std::endl;
	std::cout << std::endl;

	// 0.2 ���ؼ�����������Ӧ������Ҫ��̬���������㶨λ
	std::cout << "== Load face infomation ==" << std::endl;

	VIPLFaceInfo info;
	std::ifstream faceInfo("faceInfo.txt");
	faceInfo >> info.x >> info.y >> info.width >> info.height;
	faceInfo.close();

	std::cout << "Detect face at: (" << info.x << ", " << info.y << ", " << info.width << ", " << info.height << ")" << std::endl;
	std::cout << std::endl;

	// 0.3 �����㶨λ
	std::cout << "== Start test ==" << std::endl;
	std::vector<VIPLPoint> landmarks;

	int N = 1;
	std::cout << "Compute " << N << " times. " << std::endl;

	using namespace std::chrono;
	microseconds duration(0);
	for (int i = 0; i < N; ++i)
	{
		if (i % 10 == 0) std::cout << '.' << std::flush;
		auto start = system_clock::now();
		PD.DetectLandmarks(image, info, landmarks);
		auto end = system_clock::now();
		duration += duration_cast<microseconds>(end - start);
	}
	std::cout << std::endl;
	double spent = 1.0 * duration.count() / 1000 / N;

	std::cout << "Average takes " << spent << " ms " << std::endl;
	std::cout << std::endl;

	// 0.4 ��ȡ���
	std::cout << "== Plot result ==" << std::endl;
	std::ofstream out_landmarks("landmarks.txt");

	std::cout << "Detect landmarks at: [" << std::endl;
	for (int i = 0; i < landmarks.size(); ++i)
	{
		std::cout << "(" << landmarks[i].x << ", " << landmarks[i].y << "), ";
		out_landmarks << landmarks[i].x << " " << landmarks[i].y << std::endl;
	}
	std::cout << std::endl;
	std::cout << "]" << std::endl;
	std::cout << std::endl;

	cv::Rect face_rect(info.x, info.y, info.width, info.height);
	cv::rectangle(mat, face_rect, CV_RGB(0, 0, 255), 4, 8, 0);
	for (int i = 0; i < landmarks.size(); ++i)
	{
		cv::Point point(landmarks[i].x, landmarks[i].y);
		cv::circle(mat, point, 3, CV_RGB(0, 255, 0), -1);
	}
	cv::imwrite("face_with_points.jpg", mat);

	std::cout << "Output file: face_with_points.jpg" << std::endl;
	std::cout << "Output file: landmarks.txt" << std::endl;
	std::cout << std::endl;

	return 0;
}
