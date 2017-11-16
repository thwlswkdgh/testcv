#include "opencv.hpp"
using namespace cv;
using namespace std;

Rect    selection;
bool  bLButtonDown = false;
typedef enum { INIT, CALC_HIST, TRACKING } STATUS;
STATUS  trackingMode = INIT;

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

void onMouse(int mevent, int x, int y, int flags, void* param)
{
	static Point   origin;
	Mat *pMat = (Mat *)param;
	Mat image = Mat(*pMat);
	if (bLButtonDown)
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = selection.x + abs(x - origin.x);
		selection.height = selection.y + abs(y - origin.y);

		selection.x = MAX(selection.x, 0);
		selection.y = MAX(selection.y, 0);
		selection.width = MIN(selection.width, image.cols);
		selection.height = MIN(selection.height, image.rows);
		selection.width -= selection.x;
		selection.height -= selection.y;
	}
	switch (mevent)
	{
	case EVENT_LBUTTONDOWN:
		origin = Point(x, y);
		selection = Rect(x, y, 0, 0);
		bLButtonDown = true;
		break;
	case EVENT_LBUTTONUP:
		bLButtonDown = false;
		if (selection.width > 0 && selection.height > 0)
			trackingMode = CALC_HIST;
		break;
	}
}

void apricot(Mat rgbImage)
{
	//rgbImage = imread("hand.jpg");
	//	Mat rgbImage = imread("flower.jpg");

	//imshow("rgbImage", rgbImage);

	Mat hsvImage;
	cvtColor(rgbImage, hsvImage, COLOR_BGR2HSV);
	//imshow("hsvImage", hsvImage);

	// hand.jpg
	Scalar lowerb(0, 40, 0);
	Scalar upperb(20, 180, 255);

	// flower.jpg
	//	Scalar lowerb(150, 100,  100);
	//	Scalar upperb(180, 255, 255);

	Mat dstImage;
	inRange(hsvImage, lowerb, upperb, dstImage);
	 
	//imshow("dstImage1", dstImage);
	
	// check HSV range in object hand
	vector<Mat> planes;
	split(hsvImage, planes);
	//	imshow("planes[0]", planes[0]);
	//	imshow("planes[1]", planes[1]);
	//	imshow("planes[2]", planes[2]);

	double minH, maxH;
	minMaxLoc(planes[0], &minH, &maxH, NULL, NULL, dstImage);
	cout << "minH =" << minH << ", maxH =" << maxH << endl;

	double minS, maxS;
	minMaxLoc(planes[1], &minS, &maxS, NULL, NULL, dstImage);
	cout << "minS =" << minS << ", maxS =" << maxS << endl;

	double minV, maxV;
	minMaxLoc(planes[2], &minV, &maxV, NULL, NULL, dstImage);
	cout << "minV =" << minV << ", maxV =" << maxV << endl;


}

int main()
{
	//	VideoCapture inputVideo(0);
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
	setMouseCallback("dstImage", onMouse, (void *)&dstImage);

	int    histSize = 8;
	float  valueRange[] = { 0, 180 }; //hue's maximum is 180.
	const float* ranges[] = { valueRange };
	int channels = 0;
	Mat hist, backProject;

	int  fourcc = CV_FOURCC('D', 'I', 'V', 'X'); //CV_FOURCC
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
	TermCriteria  criteria = TermCriteria(TermCriteria::COUNT
		+ TermCriteria::EPS, 10, 2);
	Rect    trackWindow;
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
			//			imshow("mask", mask); 	

			int ch[] = { 0, 0 };
			hImage.create(hsvImage.size(), CV_8U);
			mixChannels(&hsvImage, 1, &hImage, 1, ch, 1);
			//			imshow("hImage", hImage); 
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
			//			bitwise_and(backProject, mask, backProject);
			//			imshow("backProject", backProject); 

			meanShift(backProject, trackWindow, criteria);
			Point pt1 = Point(trackWindow.x, trackWindow.y);
			Point pt2 = Point(pt1.x + trackWindow.width,
				pt1.y + trackWindow.height);
			rectangle(dstImage, pt1, pt2, Scalar(0, 0, 255), 2);
		}
		imshow("dstImage", dstImage);
		Convex_Hull(dstImage);
	//	Match(dstImage);
	//	apricot(dstImage);
		
		outputVideo << dstImage;

		int ckey = waitKey(delay);
		if (ckey == 27) break;`



	}
	return 0;
}