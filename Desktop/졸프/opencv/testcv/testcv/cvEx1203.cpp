#include "stdafx.h"
#include "opencv.hpp"
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
	cout << "Size = " << size << endl;

//	int  fourcc = -1; // Codec selection dialog
	int  fourcc = CV_FOURCC('X', 'V', 'I', 'D');
//	int  fourcc = CV_FOURCC('M', 'J', 'P', 'G'); 
//	int  fourcc = CV_FOURCC('M', 'P', '4', '2');
//	int  fourcc = CV_FOURCC('D', 'I', 'V', 'X'); 
//	int  fourcc = CV_FOURCC('P', 'I', 'M', '1'); 
//	int  fourcc = CV_FOURCC('M', 'P', 'E', 'G'); 

	double fps = 24;
	bool isColor = true;
	VideoWriter outputVideo("output.avi", fourcc, fps, size, isColor);
	if (!outputVideo.isOpened())
	{
		cout << " Can not open outputVideo !!!" << endl;
		return 0;
	}
	if (fourcc != -1)
	{
		// for waiting for ready the camera
		imshow("frame", NULL);
		waitKey(100);// not working because of no window
		//	Sleep(100);  // #include <windows.h>
	}
	int delay = 1000 / fps;
	Mat frame;
	for (;;)
	{
		inputVideo >> frame;
		if (frame.empty())
			break;

		outputVideo << frame;
		imshow("frame", frame);

		int ckey = waitKey(delay);
		if (ckey == 27) break;
	}
	return 0;
}