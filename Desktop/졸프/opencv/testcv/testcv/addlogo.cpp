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
	// 영상 ROI 정의
	// Rect는 사각형 영역 지정
	// 410, 270은 각각 logo의 x좌표, y좌표 시작지점
	// logo.cols, logo.rows는 로고의 끝지점
	cv::addWeighted(imageROI, 1.0, logo, 1, 1., imageROI);
	// 영상에 로고 붙이기
	//imageROI = 1.0*imageROI + 0.3*logo + 0;

	cv::namedWindow("imageROI");
	cv::imshow("imageROI", image);

	cv::waitKey(0);

	return 0;
}