#include "opencv.hpp"
using namespace cv;
using namespace std;
int main()
{
	cv::Mat image = cv::imread("FlowerWar.jpg");
	cv::Mat logo = cv::imread("logo.jpg");

	cv::namedWindow("Image");
	cv::imshow("Image", image);

	cv::namedWindow("Logo");
	cv::imshow("Logo", logo);

	cv::Mat imageROI = image(cv::Rect(410, 250, logo.cols, logo.rows));
	// ���� ROI ����
	// Rect�� �簢�� ���� ����
	// 410, 270�� ���� logo�� x��ǥ, y��ǥ ��������
	// logo.cols, logo.rows�� �ΰ��� ������
	cv::addWeighted(imageROI, 1.0, logo, 1, 1., imageROI);
	// ���� �ΰ� ���̱�
	//imageROI = 1.0*imageROI + 0.3*logo + 0;

	cv::namedWindow("imageROI");
	cv::imshow("imageROI", image);

	cv::waitKey(0);

	return 0;
}