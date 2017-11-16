#include "MenuInterface.h"


MenuInterface::MenuInterface(Point point)
{
	center = point;
	Scalar lowerb(0, 40, 0);
	Scalar upperb(20, 180, 255);
	 icon = cv::imread("BlackFoot.png");
	 icon2 = cv::imread("RedFoot.png");
	resize(icon, icon, Size(iconSize, iconSize));
	resize(icon2, icon2, Size(iconSize, iconSize));
	 handSize = icon.rows * icon.cols;
}

void MenuInterface::setCenter(Point point)
{
	center= point;
}

MenuInterface::~MenuInterface()
{
}


void MenuInterface::showResult(char* name, cv::Mat image)

{	//window's name
	cv::namedWindow(name);

	//image
	cv::imshow(name, image);
}


Mat MenuInterface::addIcon(Mat backGround, Mat icon, int x, int y, int iconColsSize, int iconRowsSize)
{
	


	cv::Rect rectDst(460, 640, iconRowsSize, iconColsSize);
	icon.copyTo(backGround(rectDst));

	//showResult("ImageROI(결과영상)", backGround);

	return backGround;
	//cv::waitKey(0);


}

Mat MenuInterface::trackHand(Mat backGround, Mat icon, int x, int y, int iconRowsSize, int iconColsSize)
{
	int centerX = x - iconSize / 2;
	int	centerY = y - iconSize / 2;
	if (centerX > 460)
		centerX = 460;
	if (centerX < 10)
		centerX = 10;
	if (centerY > 300)
		centerY = 300;
	if (centerY < 10)
		centerY = 10;

	cv::Rect rectDst(centerX, centerY, iconRowsSize, iconColsSize);
	icon.copyTo(backGround(rectDst));

	//showResult("ImageROI(결과영상)", backGround);

	return backGround;


}

void MenuInterface::setMode(bool modeNum)
{mode = modeNum;}

Point MenuInterface::getCenter()
{
	return center;
}


Mat MenuInterface::decideMenuMode(Mat img)
{
	 
	Rect rect(400, 250, iconSize, iconSize);
	if (rect.contains(center))
		setMode(true);
	 
	// [HS] Add Icon image
	//row: 460~640 , col: 300~480

	if (mode == false)
	{
		icon.copyTo(img(rect));
	}
	else if (mode == true)
		img = trackHand(img, icon2, center.x, center.y, iconSize, iconSize);


	return img;
}

/*
// [HS] 살색 검출-> 이진화로 (검/흰 영상 추출한 이미지 dstImage)
Mat hsvImage;
cvtColor(frame, hsvImage, COLOR_BGR2HSV);
inRange(hsvImage, lowerb, upperb, dstImage);

cvtColor(frame, hsvImage, COLOR_BGR2HSV);
//	imshow("hsvImage", hsvImage);

Scalar lowerb(0, 40, 0);
Scalar upperb(20, 180, 255);
inRange(hsvImage, lowerb, upperb, dstImage);
// imshow("bImage", bImage);
erode(dstImage, dstImage, Mat());
dilate(dstImage, dstImage, cv::Mat(), Point(-1, -1), 2);



//imshow("dst", dstImage);
Rect rtROI(460, 300, 180, 180);
Mat roi = dstImage(rtROI);
unsigned char *image_p = roi.data;

for (int i = 0; i < handSize; i++)
{
if (image_p[3 * i] > 150) whiteCount++;
else blackCount++;
}

if (whiteCount *8> blackCount){
//mode = 1;
}
*/








