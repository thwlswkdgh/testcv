#include "opencv.hpp"
using namespace cv;
using namespace std;

void showResult(char* name, cv::Mat image)

{	//window's name
	cv::namedWindow(name);

	//image
	cv::imshow(name, image);
}


Mat addIcon(Mat backGround, Mat icon, int x, int y, int iconColsSize, int iconRowsSize)
{

	cv::Rect rectDst(x, y, icon.cols, icon.rows);
	icon.copyTo(backGround(rectDst));

	//showResult("ImageROI(결과영상)", backGround);

	return backGround;
	//cv::waitKey(0);

	
}


int main()
{
	VideoCapture inputVideo(0);
	if (!inputVideo.isOpened())
	{
		cout << " Can not open inputVideo !!!" << endl;
		return -1;
	}
	Size size = Size((int)inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
	cout << "Size = " << size << endl;

	int fps = (int)(inputVideo.get(CV_CAP_PROP_FPS));
	fps = 50;
	cout << "fps = " << fps << endl;
	int frameNum = -1;
	int delay = 1000 / fps;




	Mat frame;
	//add icon from image file
	Mat icon = cv::imread("BlackFoot.png");
	Mat icon2 = cv::imread("RedFoot.png");
	int iconSize = 180;
	Mat dstImage;
	Scalar lowerb(0, 40, 0);
	Scalar upperb(20, 180, 255);
	bool mode = 0; // 0: don't find 1:find mode
	int handSize = icon.rows * icon.cols;
	int whiteCount = 0, blackCount = 0;


	resize(icon, icon, Size(iconSize, iconSize));
	resize(icon2, icon2, Size(iconSize, iconSize));

	for (;;)
	{	inputVideo >> frame;
		if (frame.empty())
			break;
		flip(frame, frame, 1);
		frameNum++;
		cout << "frameNum: " << frameNum << endl;




		// [HS] 살색 검출-> 이진화로 (검/흰 영상 추출한 이미지 dstImage)
		Mat hsvImage;
		cvtColor(frame, hsvImage, COLOR_BGR2HSV);
		inRange(hsvImage, lowerb, upperb, dstImage);
		imshow("dst", dstImage);
		Rect rtROI(460, 300, 180, 180);
		Mat roi = dstImage(rtROI);
		unsigned char *image_p = roi.data;
		for (int i = 0; i < handSize; i++)
		{
			if (image_p[3*i] > 150) whiteCount++;
			else blackCount++;
		}

		if (whiteCount*5 > blackCount){
			mode = 1;
		}
			// [HS] Add Icon image
			//row: 460~640 , col: 300~480

		if (mode==0)
			frame = addIcon(frame, icon, 460, 300, icon.cols, icon.rows);
		else if (mode==1)
			frame = addIcon(frame, icon2, 460, 300, icon2.cols, icon2.rows);
		whiteCount = blackCount = 0;

		imshow("avgImage", frame);
		


		int ckey = waitKey(delay);
		if (ckey == 27) break;
	}
	return 0;
}