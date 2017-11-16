#include "Drawing.h"
#include "ConvexHull.h"
#include "Windows.h"
Drawing::Drawing()
{}
Drawing::~Drawing()
{}

Mat Drawing::draw(Mat dstImage,Scalar color,Mat drawpannel,Mat sumpannel, Point curpoint,Point prepoint){

	line(drawpannel, curpoint, prepoint, color, 8, 8);
	add(drawpannel, sumpannel, sumpannel);  //sum drawing screens
	imshow("sumImage", sumpannel);

	//white area -> black area
/*	if (color == Scalar(255,255,255) || color == Scalar(255,0,0) )
	{
		//dstImage = dstImage(cv::Rect(0, 0, sumpannel.cols, sumpannel.rows));
		Mat range(sumpannel.size(), CV_8UC1, cv::Scalar(0));

		Scalar from(255, 255, 255);
		Scalar to(255, 255, 255);
		cv::inRange(sumpannel, from, to, range);

		cv::Mat hsv;
		vector<cv::Mat> split;
		cv::cvtColor(sumpannel, hsv, CV_BGR2HSV);
		cv::split(hsv, split);
		//r,g,b -> 255(white)
		for (int y = 0; y<range.rows; ++y){
			for (int x = 0; x<range.cols; ++x){
				if (range.at<UCHAR>(y, x) == 255){
					split[0].at<UCHAR>(y, x) -= 255;
					split[1].at<UCHAR>(y, x) -= 255;
					split[2].at<UCHAR>(y, x) -= 255;
				}
			}
		}
*///		cv::merge(split, hsv);
	//	cv::cvtColor(hsv, sumpannel, CV_HSV2BGR);
//	}	
	return sumpannel;
}


CvScalar Drawing::select(Mat drawpannel, Mat sumpannel, Point curpoint, Point prepoint){
	//selecting color
	return Scalar(255, 255, 255);
}