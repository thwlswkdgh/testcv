#include "opencv.hpp"
#include <time.h>
using namespace cv;
using namespace std;

////
const double MHI_DURATION = 1;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
const int N = 4;

//dst = 255 if abs(src1 - src2) > nTh
void  DifferenceIFrames(Mat &src1, Mat &src2, Mat &dst, int nTh)
{
	absdiff(src1, src2, dst);
	threshold(dst, dst, nTh, 255, THRESH_BINARY);
}
// convert MHI to blue 8u image
void Mhi2MotionImage(Mat &mhi, Mat &motion, Mat &mask, double timeStamp)
{
	if (motion.empty())
		motion.create(mhi.size(), CV_8UC3);
	double scale = 255. / MHI_DURATION;
	double t = MHI_DURATION - timeStamp;
	mask = mhi*scale + t*scale;
	mask.convertTo(mask, CV_8U);

	motion = Scalar::all(0);
	Mat tmp = Mat::zeros(mhi.size(), CV_8U);
	Mat outImage[] = { mask, tmp, tmp };
	merge(outImage, 3, motion);
}
Point DrawMotionOrientation(vector<Rect> rects, Mat &silh, Mat &mhi,
	Mat &orient, Mat &mask, Mat &dstImage, double timeStamp)
{
	int i, x, y;
	int count;
	Rect comp_rect;
	Scalar color;
	Point center;
	double r, angle;
	Size size = dstImage.size();

	for (i = -1; i < (int)rects.size(); i++)
	{
		if (i < 0) // global motion
		{
			comp_rect = Rect(0, 0, size.width, size.height);
			color = Scalar(255, 0, 0);
			r = 100;
		}
		else // i-th motion component 
		{
			comp_rect = rects[i];
			// reject very small components
			if (comp_rect.width*comp_rect.height < 100)
				continue;
			color = Scalar(0, 0, 255);
			r = 30;
		}
		// select component ROI
		Mat silhROI = silh(comp_rect);
		Mat mhiROI = mhi(comp_rect);
		Mat orientROI = orient(comp_rect);
		Mat maskROI = mask(comp_rect);

		angle = calcGlobalOrientation(orientROI, maskROI, mhiROI,
			timeStamp, MHI_DURATION);

		angle = 360.0 - angle;  // adjust for images with top-left origin

		//      count = norm(silhROI, CV_L1)/255;
		count = countNonZero(silhROI);
		// check for the case of little motion
		if (count < comp_rect.width*comp_rect.height * 0.001)
			continue;

		// draw a clock with arrow indicating the direction
		center = Point((comp_rect.x + comp_rect.width / 2),
			(comp_rect.y + comp_rect.height / 2));

	//	circle(dstImage, center, cvRound(r*1.2), color, 3, -1);
	//	x = cvRound(center.x + r*cos(angle*CV_PI / 180));
	//	y = cvRound(center.y - r*sin(angle*CV_PI / 180));
	//	line(dstImage, center, Point(x, y), color, 3, -1);
	}
}
void Convex_Hull(Mat srcImage)
{

	Mat dstImage = srcImage.clone();
	GaussianBlur(srcImage, srcImage, Size(3, 3), 0.0);

	Mat hsvImage;
	cvtColor(srcImage, hsvImage, COLOR_BGR2HSV);
	//imshow("hsvImage", hsvImage);

	Mat bImage;
	Scalar lowerb(0, 40, 0);
	Scalar upperb(20, 180, 255);
	inRange(hsvImage, lowerb, upperb, bImage);
	//	imshow("bImage", bImage);

	erode(bImage, bImage, Mat());
	dilate(bImage, bImage, cv::Mat(), Point(-1, -1), 2);
	//imshow("bImage", bImage);

	vector<vector<Point> > contours;
	findContours(bImage, contours, noArray(),
		RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	cout << "contours.size()=" << contours.size() << endl;
	if (contours.size()<1)
		return;

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
	//	vector<Point> handContour(contours[maxK].size());
	//	copy(contours[maxK].begin(),contours[maxK].end(),handContour.begin());

	vector<int> hull;
	convexHull(handContour, hull);
	cout << " hull.size()=" << hull.size() << endl;

	vector<Point> ptsHull;
	for (int k = 0; k<hull.size(); k++)
	{
		int i = hull[k];
		ptsHull.push_back(handContour[i]);
	}
	drawContours(dstImage, vector<vector<Point>>(1, ptsHull), 0,
		Scalar(255, 0, 0), 2);

	vector<Vec4i> defects;
	convexityDefects(handContour, hull, defects);
	for (int k = 0; k<defects.size(); k++)
	{
		Vec4i v = defects[k];
		Point ptStart = handContour[v[0]];
		Point ptEnd = handContour[v[1]];
		Point ptFar = handContour[v[2]];
		float depth = v[3] / 256.0;
		if (depth > 10)
		{
			line(dstImage, ptStart, ptFar, Scalar(0, 255, 0), 2);
			line(dstImage, ptEnd, ptFar, Scalar(0, 255, 0), 2);
			circle(dstImage, ptStart, 6, Scalar(0, 0, 255), 2);
			circle(dstImage, ptEnd, 6, Scalar(0, 0, 255), 2);
			circle(dstImage, ptFar, 6, Scalar(255, 0, 255), 2);
		}
	
	}
	cout << " defects.size()=" << defects.size() << endl;
	imshow("ConvexHull", dstImage);
	//waitKey(0);
 
}

int main()
{
	VideoCapture inputVideo(0);
	//VideoCapture inputVideo("ball.avi");
	if (!inputVideo.isOpened())
	{
		cout << " Can not open inputVideo !!!" << endl;
		return -1;
	}
	Size size = Size((int)inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
	cout << "Size = " << size << endl;

	int fps = (int)(inputVideo.get(CV_CAP_PROP_FPS));
	if (fps == 0) fps = 100;
	cout << "fps = " << fps << endl;

	double timeStamp;
	int  nTh = 50;
	int last = 0;
	int prev, curr;

	int frameNum = -1;
	int delay = 1000 / fps;
	Mat frame, dstImage;
	Mat silh, orient, motion, mask, segmask;
	Mat mhi(size, CV_32F, Scalar::all(0));
	vector<Mat> buffer(N);
	vector<Rect> boundingRects;

	for (;;)
	{
		inputVideo >> frame;
		if (frame.empty())
			break;
		frameNum++;
		//		cout << "frameNum: " << frameNum << endl;

		cvtColor(frame, buffer[last], COLOR_BGR2GRAY);
		curr = last;
		prev = (curr + 1) % N;
		last = prev;
		silh = buffer[prev];

		if (frameNum < N)
			continue;
		DifferenceIFrames(buffer[prev], buffer[curr], silh, nTh);
		imshow("silh", silh);

		// get current time in seconds
		timeStamp = (double)clock() / CLOCKS_PER_SEC;
		updateMotionHistory(silh, mhi, timeStamp, MHI_DURATION);
		Mhi2MotionImage(mhi, motion, mask, timeStamp);
		imshow("motion", motion);

		calcMotionGradient(mhi, mask, orient,
			MAX_TIME_DELTA, MIN_TIME_DELTA, 3);
		segmentMotion(mhi, segmask, boundingRects,
			timeStamp, MAX_TIME_DELTA);

		frame.copyTo(dstImage);
		if (boundingRects.size()>0)
		{

		DrawMotionOrientation(boundingRects, silh, mhi,
				orient, mask, dstImage, timeStamp); 
		}
		imshow("dstImage", dstImage);

		int ckey = waitKey(delay);
		if (ckey == 27) break;
	}
	return 0;
}