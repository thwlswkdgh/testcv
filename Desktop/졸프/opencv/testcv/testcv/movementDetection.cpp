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


	//cv::waitKey(0);




	return backGround;

}



int main()
{
	VideoCapture inputVideo(0); // "video2.avi"
	if (!inputVideo.isOpened())
	{
		cout << " Can not open inputVideo !!!" << endl;
		return 0;
	}
	Size size = Size((int)inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
	cout << "Size = " << size << endl;

	int fps = (int)(inputVideo.get(CV_CAP_PROP_FPS));
	if (fps == 0)
		fps = 50;
	cout << "fps = " << fps << endl;

	int frameNum = 0;
	int delay = 1000 / fps;
	Mat frame, grayImage;
	cv::Mat icon = cv::imread("BlackFoot.png");
	Mat sumImage(size, CV_32F, Scalar::all(0));
	for (;;)
	{
		inputVideo >> frame;
		if (frame.empty())
			break;
		


		frame= addIcon(frame, icon, 50, 50, icon.cols, icon.rows);
		imshow("frame", frame);
		
		
	}


	return 0;
}

 
