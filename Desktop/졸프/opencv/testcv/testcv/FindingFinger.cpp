#include "FindingFinger.h"
#include "BlobLabeling.h"
#include "ConvexHull.h"
#include "Drawing.h"
FindingFinger::FindingFinger()
{
	color2 = Scalar(255, 0, 0);  
}

FindingFinger::~FindingFinger()
{
}

/*
  checking finger
*/

Mat FindingFinger::FindFinger(Scalar color,Mat drawpannel,Mat dstImage,Mat sumpannel,Mat preImage,IplImage* subskin,Point center,Point endPoint)
{
	cvShowImage("FindingFinger2", subskin);
	ConvexHull preconvex;
	if (center.x > endPoint.x && center.y >endPoint.y)
	{
		//검지 손가락(selecting finger)
		preconvex.convex(preImage);
		Drawing drawing;
		color2 = drawing.select(drawpannel, sumpannel, endPoint, preconvex.getEndPoint());
 	}
	else 
	{
		//새끼 손가락(drawing)
		preconvex.convex(preImage);
		Drawing drawing;
		sumpannel = drawing.draw(dstImage, color, drawpannel, sumpannel, endPoint, preconvex.getEndPoint());
	}

	//480 = dstImage.rows
	//640 = dstImage.cols

	//webcam screen +  drawing screen
	dstImage = dstImage(cv::Rect(0, 0, sumpannel.cols, sumpannel.rows));
	add(dstImage, sumpannel, dstImage);
	return dstImage;

}
Scalar FindingFinger::getcolor(){
	return color2;
}

bool isSecondFinger(Point center, Point endPoint)
{
	if (center.x > endPoint.x && center.y > endPoint.y)
		return true;
	else
		return false;
}