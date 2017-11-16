#pragma once
#include "opencv.hpp"

using namespace cv;
using namespace std;

class FindingFinger
{
private:
	Scalar color2; 
	Point center;
	int center_x;
	int center_y;


public:
	FindingFinger();
	~FindingFinger();
	Mat FindFinger(Scalar color,Mat drawpannel, Mat dstImage, Mat sumpannel, Mat preImage, IplImage* subskin, Point center, Point endPoint);
	Scalar getcolor();
};