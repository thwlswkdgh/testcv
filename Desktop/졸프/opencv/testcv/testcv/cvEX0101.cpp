
#include "opencv.hpp"
using namespace cv;
using namespace std;
int main()
{
	Mat srcImage = imread("lena.jpg", IMREAD_GRAYSCALE);
	if (srcImage.empty())
		return -1;

	imshow("srcImage", srcImage);
	waitKey();
	return 0;
}
