#include "opencv.hpp"
#include <Windows.h>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	//if (argc != 2)
	/*{
		cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}
	*/
	Mat img, fin_img;
	img = imread("bgr.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	Mat me = imread("BkgImage00.jpg");
	/*if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Show our image inside it.

	// Create Windows
	/*namedWindow("Red", 1);
	namedWindow("Green", 1);
	namedWindow("Blue", 1);
	*/
	// Create Matrices (make sure there is an image in input!)
	/*
	Mat channel[3];
	imshow("Original Image", image);


	// The actual splitting.
	split(image, channel);


	//channel[0] = Mat::zeros(image.rows, image.cols, CV_8UC1);//Set blue channel to 0
	//channel[1] = Mat::zeros(image.rows, image.cols, CV_8UC1);//Set blue channel to 0
	//channel[2] = Mat::zeros(image.rows, image.cols, CV_8UC1);//Set blue channel to 0
	//Merging red and green channels	

	merge(channel, 3, image);
	imshow("R+G", image);
	imwrite("dest.jpg", image);

	add(me, image, me);
	*/
		Mat range(img.size(), CV_8UC1, cv::Scalar(0));

	// EX) RED
	Scalar from(255, 255, 255);
	Scalar to(255, 255, 255);

	cv::inRange(img, from, to, range);

	cv::Mat hsv;
	vector<cv::Mat> split;
	cv::cvtColor(img, hsv, CV_BGR2HSV);
	cv::split(hsv, split);

	for (int y = 0; y<range.rows; ++y){
		for (int x = 0; x<range.cols; ++x){
			if (range.at<UCHAR>(y, x) == 255){
				split[0].at<UCHAR>(y, x) -= 255;
				split[1].at<UCHAR>(y, x) -= 255;
				split[2].at<UCHAR>(y, x) -= 255;

			}
		}
	}

	cv::merge(split, hsv);
	cv::cvtColor(hsv, img, CV_HSV2BGR);
	imshow("img", img);
	add(me, img, me);

		imshow("me", me);
	waitKey(0);//Wait for a keystroke in the window
	return 0;
}