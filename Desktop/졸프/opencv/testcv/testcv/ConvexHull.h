#pragma once
#include "opencv.hpp"
using namespace cv;
using namespace std;

class ConvexHull
{private:

	Point *P4=NULL;
	Point *maxp = NULL;
	Point *minp = NULL;
	Point center;
	int i=0;
	Mat dstImage, bImage, hsvImage;
	vector<Point> handContour;
	int maxy_x;
	int max_x;
	int max_y;
	int r;
	IplImage *IpbImage2;
	IplImage *IpbImage;
	Mat circleImage;
	IplImage *IpcircleImage;
	IplImage *vcircle;
public:
	ConvexHull();
	~ConvexHull();
	Point getCenter();
	Mat convex(Mat Image);
	template <class T>
	T* vec_to_arr(vector<T> v1);// 1D array
	template <class K>
	K* findmin(K* P1, int num);
	template <class R>
	R* findmax(R* P1, int num);
	template <class R>
	int findx(R P, int maxy, int num);
	IplImage* getcircle();
	IplImage* getIpbImage();
	int getR();
	Mat getbImage();
	Point getEndPoint();


};

