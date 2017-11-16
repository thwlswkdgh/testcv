#include<iostream>

#include<opencv2\core\core.hpp>

#include<opencv2\highgui\highgui.hpp>

//��� ��� �Լ�

void ShowResult(char* name, cv::Mat image);




int main()

{




	cv::Mat image = cv::imread("hand.jpg");

	cv::Mat logo = cv::imread("book1.jpg");




	ShowResult("panda", image);

	ShowResult("logo", logo);




	cv::Mat imageROI = image(cv::Rect(50, 10, logo.cols, logo.rows));

	//�ΰ��� x,y ������ǥ����,col,row ��������

	cv::addWeighted(imageROI, 1.0, logo, 0.3, 0.0, imageROI);

	//���� �ΰ� ���̱�




	ShowResult("ImageROI(�������)", image);




	cv::waitKey(0);




	return 1;

}

void ShowResult(char* name, cv::Mat image)

{

	cv::namedWindow(name);//â����

	cv::imshow(name, image);//�����̹��� ���̱�

}

