#include "opencv.hpp"
#include "BlobLabeling.h"
using namespace cv;
using namespace std;

template <class T>
T* vec_to_arr(vector<T> v1) // 1D array
{
	T* v2 = new T[v1.size()];
	for (int i = 0; i < v1.size(); i++)
		v2[i] = v1[i]; // copy
	return v2;
}

template <class R>
int findx(R P, int maxy, int num)
{
	int maxy_x = 0;
	for (int i = 0; i<num; i++)
	{

		if (P[i].y == maxy)
		{
			maxy_x = P[i].x;
			return maxy_x;
		}

	}
}

template <class T>
void delete_arr(T* arr)
{
	delete[] arr;
}

template <class K>
K* findmin(K* P1, int num)
{
	int i; int minx; int miny;
	minx = P1[0].x;
	miny = P1[0].y;
	for (i = 0; i < num; i++)
	{
		if (P1[i].x < minx)
			minx = P1[i].x;
		if (P1[i].y < miny)
			miny = P1[i].y;
	}
	P1->x = minx; P1->y = miny;
	return P1;
}
template <class R>
R* findmax(R* P1, int num)
{
	int i; int maxx; int maxy;
	maxx = P1[0].x;
	maxy = P1[0].y;
	for (i = 0; i < num; i++)
	{
		if (P1[i].x > maxx)
			maxx = P1[i].x;
		if (P1[i].y > maxy)
			maxy = P1[i].y;
	}
	P1->x = maxx; P1->y = maxy;
	return P1;
}


int Convex_hull(Mat srcImage)
{
	Point *P4;
	Point *maxp;
	Point *minp;
	int i;

	
	Mat dstImage = srcImage.clone();
	GaussianBlur(srcImage, srcImage, Size(3, 3), 0.0);

	Mat hsvImage;
	cvtColor(srcImage, hsvImage, COLOR_BGR2HSV);
	imshow("hsvImage", hsvImage);

	Mat bImage;
	Scalar lowerb(0, 40, 0);
	Scalar upperb(20, 180, 255);
	inRange(hsvImage, lowerb, upperb, bImage);
	//	imshow("bImage", bImage);

	erode(bImage, bImage, Mat());
	dilate(bImage, bImage, cv::Mat(), Point(-1, -1), 2);
	imshow("bImage", bImage);

	vector<vector<Point> > contours;
	findContours(bImage, contours, noArray(),
		RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	cout << "contours.size()=" << contours.size() << endl;
	if (contours.size()<1)
		return 0;

	int maxK = 0;
	double maxArea = contourArea(contours[0]);
	for (int k = 1; k<contours.size(); k++)
	{
		double area = contourArea(contours[k]);
		if (area > maxArea)
		{
			maxK = k;
			maxArea = area;
		}
	}
	vector<Point> handContour = contours[maxK];



	vector<int> hull;
	convexHull(handContour, hull);

	P4 = vec_to_arr<Point>(handContour);


	maxp = findmax(P4, handContour.size());
	int max_x = maxp->x; int max_y = maxp->y;
	minp = findmin(P4, handContour.size());
	cout << "최대:(" << max_x << "," << max_y << ")" << endl;
	cout << "최소:(" << minp->x << "," << minp->y << ")" << endl;
	///왜 findmax는 없어지징?!?!?!!?
	rectangle(dstImage, Point(max_x, max_y), Point(minp->x, minp->y), Scalar(0, 0, 255), 2);
	/////////////////////////////////////
	Point center; int maxy_x, r; IplImage *IpbImage, *vcircle, *IpbImage2, *IpcircleImage; Mat circleImage;
	IpbImage2 = new IplImage(bImage);
	IpbImage = cvCloneImage(IpbImage2);
	circleImage = cvLoadImage("img6.jpg");
	IpcircleImage = new IplImage(circleImage);
	imshow("bImage", bImage);

	center.x = (max_x + minp->x) / 2;
	center.y = (max_y + minp->y) / 2;


	circle(dstImage, center, 10, Scalar(0, 255, 0), -1);

	/*최대 y값에 초록색 원그리기*/
	maxy_x = findx(handContour, minp->y, handContour.size());
	circle(dstImage, Point(maxy_x, minp->y), 10, Scalar(0, 255, 0), -1);

	/*겹치는 부분 찾기위한 원그리기*/
	r = sqrt(pow(abs(maxy_x - (max_x + minp->x) / 2), 2) + pow(abs(minp->y - (max_y + minp->y) / 2), 2));
	//Mat -> Image // Changing Type
	vcircle = cvCreateImage(cvGetSize(IpbImage), 8, 1);
	cvSetZero(vcircle);
	cvLine(vcircle, Point(center.x + r, center.y - (3.0 / 5)*r), Point(center.x - r, center.y - (3.0 / 5)*r), Scalar(255, 0, 0), 5);
	cvLine(IpbImage, Point(center.x + r, center.y - (3.0 / 5)*r), Point(center.x - r, center.y - (3.0 / 5)*r), Scalar(255, 0, 0), 5);

	//cvCircle(IpbImage, Point((max_x + minp->x) / 2, (max_y + minp->y) / 2), r * 2 / 3, Scalar(255, 0, 0), 3);

	/////////////////////////////
	


	CBlobLabeling label;
	label.SetParam(IpbImage, 50);

	label.DoLabeling();
	cout << label.m_nBlobs << "\n";
	cvAnd(IpbImage, vcircle, IpbImage, 0);

	cvShowImage("FindingFinger", IpbImage);
	imshow("dstImage", dstImage);

}
int main()
{
	Mat srcImage = imread("hand.jpg");
	Convex_hull(srcImage);
	waitKey(0);
	return 0;
}