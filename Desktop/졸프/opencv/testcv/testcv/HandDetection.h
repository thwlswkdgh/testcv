#pragma once
#include "opencv.hpp"
using namespace cv;
using namespace std;



class HandDetection
{
private:
	typedef enum { INIT, CALC_HIST, TRACKING } STATUS;
	STATUS trackingMode = INIT;
	Mat dstImage;
	int histSize = 8;
	int channels = 0;
	Mat hist, backProject;
	bool isColor = true;
	TermCriteria criteria = TermCriteria(TermCriteria::COUNT
		+ TermCriteria::EPS, 10, 2);
	Rect trackWindow;
	Mat hImage, hsvImage, mask;

public:
	HandDetection();
	~HandDetection();
	Mat Detection(Mat frame);
};

/*
#include "opencv.hpp"
using namespace cv;
using namespace std;
Rect selection;
bool bLButtonDown = false;
typedef enum { INIT, CALC_HIST, TRACKING } STATUS;
STATUS trackingMode = INIT;
template <class T>
T* vec_to_arr(vector<T> v1) // 1D array
{
T* v2 = new T[v1.size()];
for (int i = 0; i < v1.size(); i++)
v2[i] = v1[i]; // copy
return v2;
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

void Convex_hull(Mat srcImage)
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
// imshow("bImage", bImage);
erode(bImage, bImage, Mat());
dilate(bImage, bImage, cv::Mat(), Point(-1, -1), 2);
imshow("bImage", bImage);
vector<vector<Point> > contours;
findContours(bImage, contours, noArray(),
RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
cout << "contours.size()=" << contours.size() << endl;
if (contours.size()<1)
return ;
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
//벡터를 배열로
P4 = vec_to_arr<Point>(handContour);
//최대 최소 찾는부분
maxp = findmax(P4, handContour.size());
int max_x = maxp->x; int max_y = maxp->y;
minp = findmin(P4, handContour.size());
cout << "최대:(" << max_x << "," << max_y << ")" << endl;
cout << "최소:(" << minp->x << "," << minp->y << ")" << endl;
///왜 findmax는 없어지징?!?!?!!?
rectangle(dstImage, Point(max_x, max_y), Point(minp->x, minp->y), Scalar(0, 0, 255), 2);
//중앙점을 찾고 거기에 원 그림
circle(dstImage, Point((max_x + minp->x) / 2, (max_y + minp->y) / 2), 10, Scalar(0, 255, 0), -1);
cout << " hull.size()=" << hull.size() << endl;
vector<Point> ptsHull;
for (int k = 0; k<hull.size(); k++)
{
int i = hull[k];
ptsHull.push_back(handContour[i]);
}

imshow("dstImage", dstImage);
}
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
int fps = (int)(inputVideo.get(CV_CAP_PROP_FPS));
if (fps <= 0) fps = 24; // for camera
Mat dstImage;
namedWindow("dstImage");
int histSize = 8;
float valueRange[] = { 0, 180 }; //hue's maximum is 180.
const float* ranges[] = { valueRange };
int channels = 0;
Mat hist, backProject;
int fourcc = CV_FOURCC('D', 'I', 'V', 'X'); //CV_FOURCC
bool isColor = true;
VideoWriter outputVideo("trackingRect.avi", fourcc, fps, size, isColor);
if (!outputVideo.isOpened())
{
cout << " Can not open outputVideo !!!" << endl;
return 0;
}
if (fourcc != -1)
{
// for waiting for ready the camera
imshow("dstImage", NULL);
waitKey(100);// not working because of no window
}
TermCriteria criteria = TermCriteria(TermCriteria::COUNT
+ TermCriteria::EPS, 10, 2);
Rect trackWindow;
int delay = 1000 / fps;
Mat frame, hImage, hsvImage, mask;
for (;;)
{
inputVideo >> frame;
if (frame.empty())
break;
flip(frame, frame, 1);
cvtColor(frame, hsvImage, COLOR_BGR2HSV);
frame.copyTo(dstImage);

if (bLButtonDown && 0<selection.width && 0<selection.height)
{
Mat dstROI = dstImage(selection);
bitwise_xor(dstROI, Scalar::all(255), dstROI);
}
if (trackingMode) // CALC_HIST or TRACKING
{
// create mask image
int vmin = 50, vmax = 256, smin = 50;
inRange(hsvImage, Scalar(0, smin, MIN(vmin, vmax)),
Scalar(180, 256, MAX(vmin, vmax)), mask);
// imshow("mask", mask);
int ch[] = { 0, 0 };
hImage.create(hsvImage.size(), CV_8U);
mixChannels(&hsvImage, 1, &hImage, 1, ch, 1);
// imshow("hImage", hImage);
if (trackingMode == CALC_HIST)
{
Mat hImageROI(hImage, selection), maskROI(mask, selection);
calcHist(&hImageROI, 1, &channels, maskROI, hist, 1, &histSize, ranges);
normalize(hist, hist, 0, 255, NORM_MINMAX);//CV_MINMAX
trackWindow = selection;
trackingMode = TRACKING;
}
// TRACKING:
calcBackProject(&hImage, 1, &channels, hist, backProject, ranges);
backProject &= mask;
// bitwise_and(backProject, mask, backProject);
// imshow("backProject", backProject);
meanShift(backProject, trackWindow, criteria);
Point pt1 = Point(trackWindow.x, trackWindow.y);
Point pt2 = Point(pt1.x + trackWindow.width,
pt1.y + trackWindow.height);
rectangle(dstImage, pt1, pt2, Scalar(0, 0, 255), 2);
}
Convex_hull(dstImage);
outputVideo << dstImage;
int ckey = waitKey(delay);
if (ckey == 27) break;
}
return 0;
}

*/