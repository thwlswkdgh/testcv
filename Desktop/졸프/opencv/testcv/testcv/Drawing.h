#pragma once
#include "opencv.hpp"
using namespace cv;
using namespace std;
#include <vector>

class Drawing
{
private:
	Point dot, predot;
	
	std::vector<cv::Mat> planes;

public:
	Drawing();
	~Drawing();
	Mat draw(Mat dstImage, Scalar color, Mat drawpannel, Mat sumpannel, Point curpoint, Point prepoint);
	CvScalar select(Mat drawpannel, Mat sumpannel, Point curpoint, Point prepoint);

};
