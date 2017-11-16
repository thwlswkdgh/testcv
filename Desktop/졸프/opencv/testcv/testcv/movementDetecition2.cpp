#include<iostream>

#include<opencv2\core\core.hpp>

#include<opencv2\highgui\highgui.hpp>

//결과 출력 함수

void ShowResult(char* name, cv::Mat image);




int main()

{




	cv::Mat image = cv::imread("hand.jpg");

	cv::Mat logo = cv::imread("book1.jpg");




	ShowResult("panda", image);

	ShowResult("logo", logo);




	cv::Mat imageROI = image(cv::Rect(50, 10, logo.cols, logo.rows));

	//로고의 x,y 시작좌표지정,col,row 끝점지정

	cv::addWeighted(imageROI, 1.0, logo, 0.3, 0.0, imageROI);

	//영상에 로고 붙이기




	ShowResult("ImageROI(결과영상)", image);




	cv::waitKey(0);




	return 1;

}

void ShowResult(char* name, cv::Mat image)

{

	cv::namedWindow(name);//창정의

	cv::imshow(name, image);//원본이미지 보이기

}

