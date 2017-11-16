

#include <cv.h>
#include <highgui.h>

using namespace cv;

void main()
{
	IplImage* image = 0;
	CvCapture* capture = cvCaptureFromCAM(0);
	cvNamedWindow("T9-camera", 0);


	cvResizeWindow("T9-camera", 320, 240);


	while (1) {
		cvGrabFrame(capture);
		image = cvRetrieveFrame(capture);
		cvFlip(image,image,1);




		cvShowImage("T9-camera", image);

		if (cvWaitKey(10) >= 0)
			break;
	}


	cvReleaseCapture(&capture);
	cvDestroyWindow("T9-camera");
}


void showResult(char* name, cv::Mat image)

{	//window's name
	cv::namedWindow(name);

	//image
	cv::imshow(name, image);
}


Mat addIcon(Mat backGround, Mat icon, int x, int y, int iconColsSize, int iconRowsSize)
{


	cv::Rect rectDst(50, 10, icon.cols, icon.rows);
	icon.copyTo(backGround(rectDst));

	showResult("ImageROI(결과영상)", backGround);


	cv::waitKey(0);




	return backGround;

}
