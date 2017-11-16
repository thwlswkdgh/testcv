#include "HandDetection.h"
#include "opencv.hpp"
using namespace cv;
using namespace std;
Rect selection;

HandDetection::HandDetection()
{

}


HandDetection::~HandDetection()
{
}


Mat HandDetection::Detection(Mat frame)
{
	float valueRange[] = { 0, 180 }; //hue's maximum is 180.
	const float* ranges[] = { valueRange };

	cvtColor(frame, hsvImage, COLOR_BGR2HSV);
	frame.copyTo(dstImage);

	if (0<selection.width && 0<selection.height)
	{
		Mat dstROI = dstImage(selection);
		bitwise_xor(dstROI, Scalar::all(255), dstROI);
}
	if (trackingMode) // CALC_HIST or TRACKING
	{
		// create mask image
		int vmin = 50, vmax = 256, smin = 50;
		inRange(hsvImage, Scalar(0, smin, MIN(vmin, vmax)),
			Scalar(180, 256, MAX(vmin, vmax)), mask);
		// imshow("mask", mask);
		int ch[] = { 0, 0 };
		hImage.create(hsvImage.size(), CV_8U);
		mixChannels(&hsvImage, 1, &hImage, 1, ch, 1);
		// imshow("hImage", hImage);
		if (trackingMode == CALC_HIST)
		{
			Mat hImageROI(hImage, selection), maskROI(mask, selection);
			calcHist(&hImageROI, 1, &channels, maskROI, hist, 1, &histSize, ranges);
			normalize(hist, hist, 0, 255, NORM_MINMAX);//CV_MINMAX
			trackWindow = selection;
			trackingMode = TRACKING;
		}
		// TRACKING:
		calcBackProject(&hImage, 1, &channels, hist, backProject, ranges);
		backProject &= mask;
		// bitwise_and(backProject, mask, backProject);
		// imshow("backProject", backProject);
		meanShift(backProject, trackWindow, criteria);
		Point pt1 = Point(trackWindow.x, trackWindow.y);
		Point pt2 = Point(pt1.x + trackWindow.width,
			pt1.y + trackWindow.height);
		rectangle(dstImage, pt1, pt2, Scalar(0, 0, 255), 2);
	}
	return dstImage;
}


