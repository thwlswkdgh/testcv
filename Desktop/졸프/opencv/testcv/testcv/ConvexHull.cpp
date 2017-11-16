#include "ConvexHull.h"

ConvexHull::ConvexHull()
{
	center.x = center.y= 0;
	maxy_x = 0;
}

ConvexHull::~ConvexHull()
{
}
/*
find hand with skin color (erode, dilate)
find endpoint, centerpoint and tracking them
*/
Mat ConvexHull::convex(Mat srcImage)
{
	
	Mat errorMat;
	dstImage = srcImage.clone();
	GaussianBlur(srcImage, srcImage, Size(3, 3), 0.0);
 
	cvtColor(srcImage, hsvImage, COLOR_BGR2HSV);
 
	Scalar lowerb(0, 40, 0);
	Scalar upperb(20, 180, 255);
	inRange(hsvImage, lowerb, upperb, bImage);

	erode(bImage, bImage, Mat());
	dilate(bImage, bImage, cv::Mat(), Point(-1, -1), 2);
	//erode(bImage, bImage, getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	//dilate(bImage, bImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	
	/*
	removing noise
	erode = 애매한 부분 없애기
	dilate = 애매한 부분 살리기
	*/
	Size size(5, 5);
	Mat rectKernel = getStructuringElement(MORPH_RECT, size);

	int iterations = 15;
	Point anchor(-1, -1);
	Mat erodeImage;
	erode(bImage, bImage, MORPH_RECT, Point(-1, -1), 3);
	dilate(bImage, bImage, MORPH_RECT, anchor, iterations);
	//morphologyEx(bImage, bImage, MORPH_OPEN, rectKernel, anchor, 2);
	morphologyEx(bImage, bImage, MORPH_CLOSE, rectKernel, anchor, 20);

	//IpbImage Changing Type -> 위치조정하지말것
	IpbImage2 = new IplImage(bImage);
	IpbImage = cvCloneImage(IpbImage2);

	vector<vector<Point> > contours;
	findContours(bImage, contours, noArray(),RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	if (contours.size() < 1)
	{
		//cout << "[ConvexHull.cpp]return errorMat" << contours.size() << endl;
		return srcImage;
	}
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
	handContour = contours[maxK];

	vector<int> hull;
	convexHull(handContour, hull);
	
	P4 = vec_to_arr<Point>(handContour);
	
	//finding max value, min value
	if (handContour.size() > 0)
	{
		maxp = findmax(P4, handContour.size());
		max_x = maxp->x; max_y = maxp->y;
	}
	if (handContour.size() > 0)
		minp = findmin(P4, handContour.size());
	
	rectangle(dstImage, Point(max_x, max_y), Point(minp->x, minp->y), Scalar(0, 0, 255), 2);
	//finding center & drawing circle
	center.x = (max_x + minp->x) / 2;
	center.y = (max_y + minp->y) / 2;
	circle(dstImage, center , 10, Scalar(0, 255, 0), -1);
	
	//finding endpoint & drawing circle
	maxy_x = findx(handContour, minp->y, handContour.size());
	circle(dstImage, Point(maxy_x, minp->y), 10, Scalar(0, 255, 0), -1);

	return dstImage;
}
int ConvexHull::getR()
{
	return r;
}
Point ConvexHull::getCenter()
{ 
	if (P4!=NULL)
		return center;
} 
Point ConvexHull::getEndPoint()
{
	if (P4!=NULL)
		return Point(maxy_x,minp->y);
}
IplImage* ConvexHull::getcircle()
{
	return vcircle;
}

IplImage* ConvexHull::getIpbImage()
{
	return IpbImage;
}
Mat ConvexHull::getbImage()
{
	return bImage;
}
template <class T>
T* ConvexHull::vec_to_arr(vector<T> v1) // 1D array
{
	T* v2 = new T[v1.size()];
	for (int i = 0; i < v1.size(); i++)
		v2[i] = v1[i]; // copy
	return v2;
}
template <class K>
K* ConvexHull::findmin(K* P1, int num)
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
R* ConvexHull::findmax(R* P1, int num)
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

template <class R>
int ConvexHull::findx(R P, int maxy, int num)
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
