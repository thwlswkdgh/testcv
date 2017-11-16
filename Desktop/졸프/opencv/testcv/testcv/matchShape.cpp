#include "opencv.hpp"
using namespace cv;
using namespace std;
int main()
{


	Mat srcImage1 = imread("hand.jpg", IMREAD_GRAYSCALE);
	Mat srcImage2 = imread("cup2_1.jpg", IMREAD_GRAYSCALE);
	//	Mat srcImage1 = imread("book1.jpg", IMREAD_GRAYSCALE );
	//	Mat srcImage2 = imread("book2.jpg", IMREAD_GRAYSCALE );
	if (srcImage1.empty() || srcImage2.empty())
		return -1;

	//  Step 1, 2: detect the keypoints & descriptors
	vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;

	//OpenCV2.4.13
	//	ORB(1000)(srcImage1, noArray(), keypoints1, descriptors1);
	//	ORB(1000)(srcImage2, noArray(), keypoints2, descriptors2);

	BRISK()(srcImage1, noArray(), keypoints1, descriptors1);
	BRISK()(srcImage2, noArray(), keypoints2, descriptors2);

	//  Step 3: Matching descriptor vectors
	vector< DMatch > matches;
	BFMatcher  matcher(NORM_HAMMING);
	matcher.match(descriptors1, descriptors2, matches);

	//	Ptr<DescriptorMatcher> matcher;
	//	matcher = DescriptorMatcher::create("BruteForce-Hamming");
	//	matcher->match( descriptors1, descriptors2, matches );

	cout << "matches.size()=" << matches.size() << endl;
	if (matches.size() < 4)
		return 0;

	// find goodMatches such that matches[i].distance <= 4*minDist
	double minDist, maxDist;
	minDist = maxDist = matches[0].distance;
	for (int i = 1; i < matches.size(); i++)
	{
		double dist = matches[i].distance;
		if (dist < minDist) minDist = dist;
		if (dist > maxDist) maxDist = dist;
	}
	cout << "minDist=" << minDist << endl;
	cout << "maxDist=" << maxDist << endl;

	vector< DMatch > goodMatches;
	double  fTh = 4 * minDist;
	for (int i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance <= max(fTh, 0.02))
			goodMatches.push_back(matches[i]);
	}
	cout << "goodMatches.size()=" << goodMatches.size() << endl;
	if (goodMatches.size() < 4)
		return 0;

	// draw good_matches
	Mat imgMatches;
	drawMatches(srcImage1, keypoints1, srcImage2, keypoints2,
		goodMatches, imgMatches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS); //DEFAULT
		imshow("Good Matches", imgMatches);

	/*
	for( int i = 0; i < goodMatches.size(); i++ )
	{
	cout << "Good Matches [" << i << "] Keypoint 1:";
	cout << goodMatches[i].queryIdx;
	cout << "--> Keypoint 2:";
	cout << goodMatches[i].trainIdx << endl;
	}
	*/

	// find Homography between keypoints1 and keypoints2
	vector<Point2f> obj;
	vector<Point2f> scene;
	for (int i = 0; i < goodMatches.size(); i++)
	{
		// Get the keypoints from the good matches
		obj.push_back(keypoints1[goodMatches[i].queryIdx].pt);
		scene.push_back(keypoints2[goodMatches[i].trainIdx].pt);
	}
	Mat H = findHomography(obj, scene, RANSAC);

	vector<Point2f> objP(4);
	objP[0] = Point2f(0, 0);
	objP[1] = Point2f(srcImage1.cols, 0);
	objP[2] = Point2f(srcImage1.cols, srcImage1.rows);
	objP[3] = Point2f(0, srcImage1.rows);

	vector<Point2f> sceneP(4);
	perspectiveTransform(objP, sceneP, H);

	// draw sceneP in imgMatches
	for (int i = 0; i < 4; i++)
		sceneP[i] += Point2f(srcImage1.cols, 0);
	for (int i = 0; i < 4; i++)
		line(imgMatches, sceneP[i], sceneP[(i + 1) % 4], Scalar(255, 0, 0), 4);
	imshow("imgMatches", imgMatches);

	waitKey();
	return 0;
}