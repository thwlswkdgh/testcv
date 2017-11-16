#include "opencv.hpp"
#pragma once
 
using namespace cv;
using namespace std;
class MenuInterface
{private:
	
	Point center;
	Mat frame;
	//add icon from image file
	Mat icon;// = cv::imread("BlackFoot.png");
	Mat icon2;// = cv::imread("RedFoot.png");
	int iconSize = 180,frameX=500,frameY=400;
	Mat dstImage;
	Scalar lowerb;// (0, 40, 0);
	Scalar upperb;// (20, 180, 255);
	bool mode = 0; // 0: don't find 1:find mode
	int handSize;// = icon.rows * icon.cols;
	int whiteCount = 0, blackCount = 0;

public:
	Mat trackHand(Mat backGround, Mat icon, int x, int y, int iconColsSize, int iconRowsSize);
	void setCenter(Point);
	MenuInterface(Point);
	~MenuInterface();
	void showResult(char* name, Mat image);
	void setMode(bool);
	Mat addIcon(Mat backGround, Mat icon, int x, int y, int iconColsSize, int iconRowsSize);
	Mat decideMenuMode(Mat frame);
	Point getCenter();
};

