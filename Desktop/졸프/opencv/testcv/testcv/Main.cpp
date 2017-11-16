#include "opencv.hpp"
#include "HandDetection.h"
#include "ConvexHull.h"
#include "MenuInterface.h"
#include "FindingFinger.h"
#include "Drawing.h"
#include "Windows.h"
using namespace cv;
using namespace std;


int main()
{
	VideoCapture inputVideo(0);
	if (!inputVideo.isOpened())
	{
		cout << " Can not open inputVideo !!!" << endl;
		return 0;
	}
	Size size = Size((int)inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
	int fps = (int)(inputVideo.get(CV_CAP_PROP_FPS));
	if (fps <= 0) fps = 24; // for camera
	
	Mat dstImage;
	namedWindow("dstImage");
	TermCriteria criteria = TermCriteria(TermCriteria::COUNT
		+ TermCriteria::EPS, 10, 2);
	Rect trackWindow;
	int delay = 1000 / fps;
	Mat frame,preframe;
	bool IsFirst=true;
	Point prevCenter, center;
	Mat channel[3];	
	MenuInterface menuInterface(Point(0, 0));
	//for drawing
	Mat sumpannel = imread("Black.jpg");
	Mat drawpannel = imread("Black.jpg");
	
	bool checkfinger = false;
	Scalar color=Scalar(255,0,0);
	FindingFinger findFinger;

	for (;;)
	{
		inputVideo >> frame;
		
		if (frame.empty())
			continue;
		if (preframe.empty())	//previous frame's initial value
			preframe = 0;

		flip(frame, frame, 1);
		frame.copyTo(dstImage);
		
		
		//Mat menu = imread("menu.jpg", IMREAD_GRAYSCALE);
		//resize(menu, menu, Size(480, 640));

		//finding center point
		ConvexHull convex;
		convex.convex(dstImage).copyTo(dstImage);

/*Çö¼ø(¹ß¹Ù´Ú)	if (IsFirst == true)
		{
			IsFirst = false;
			prevCenter = convex.getCenter();
		}

		if (abs(convex.getCenter().x - prevCenter.x) > 100 || abs(convex.getCenter().y - prevCenter.y) > 100)
		{
			menuInterface.setMode(0);
			IsFirst = true;
		}
*/
		menuInterface.setCenter(center=convex.getCenter());
		//menuInterface.decideMenuMode(dstImage).copyTo(dstImage);
		//imshow("dstImage", menuInterface.decideMenuMode(dstImage));

		//rectangle(menu, Point(center.x - 10, center.y-10), Point(center.x+10,center.y+10 ), Scalar(0, 0, 255), 2);
		//imshow("menu", menu);
		
		//checking finger & drawing
		if (!preframe.empty())
		{
			findFinger.FindFinger(color, drawpannel, dstImage, sumpannel, preframe, convex.getIpbImage(), convex.getCenter(), convex.getEndPoint()).copyTo(dstImage);
			color = findFinger.getcolor();	//selecting color
		}
		
		imshow("Painting Screen", dstImage);

		//previous frame for drawing (comparing previous screen)
		preframe = frame.clone();		

		int ckey = waitKey(delay);
		if (ckey == 27) break;
	}
	return 0;
}