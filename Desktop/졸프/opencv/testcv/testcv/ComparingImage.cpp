#include "opencv.hpp"
using namespace cv;
using namespace std;

#define SCALE 0.2
#define NUM 5
#define BINS 8

int main(int argc, char** argv)
{
	// the names of images to be compared each other
	string imgNames[NUM] = { "perfectcircle.jpg", "30circle.jpg", "img2_beach.jpg", "img2_garden.jpg", "img2_grapefarm.jpg" };

	for (int i = 0; i < NUM; i++)
	{
		cout << imgNames[i] << endl;
	}

	// read all images and convert to HLS format
	
	
		Mat imgs[NUM];
		Mat imgsHLS[NUM];
		Mat image = imread("perfectcircle.jpg");
		Rect rect(0, 0, 10, 10);
	//	Mat subImage = image(rect);
		imshow("image", image);
	//	cout << "Dsdfsdf" << endl;
	for (int i = 0; i < NUM; i++)
	{
		imgs[i] = imread(imgNames[i], IMREAD_COLOR);
		if (imgs[i].data == 0)
		{
			cout << "Unable to read " << imgNames[i] << endl;
			return 0;
		}
		cvtColor(imgs[i], imgsHLS[i], COLOR_BGR2HLS);
	}
	cout << "Succeeded to read all images" << endl;

	// compute 3D histogram
	Mat histogram[NUM];

	int channel_numbers[] = { 0, 1, 2 };
	for (int i = 0; i < NUM; i++)
	{
		int* number_bins = new int[imgsHLS[i].channels()];
		for (int ch = 0; ch < imgsHLS[i].channels(); ch++)
		{
			number_bins[ch] = BINS;
		}
		float ch_range[] = { 0.0, 255.0 };
		const float *channel_ranges[] = { ch_range, ch_range, ch_range };
		calcHist(&imgsHLS[i], 1, channel_numbers, Mat(), histogram[i], imgsHLS[i].channels(), number_bins, channel_ranges);
		normalize(histogram[i], histogram[i], 1.0);
	}

	double matching_score = compareHist(histogram[0], histogram[1], CV_COMP_CORREL);
	cout << imgNames[0] << "-" << imgNames[1] << ", " << matching_score << endl;
	cout << "Image Comparison by HISTCMP_CORREL   " << endl;
	
	for (int i = 0; i < NUM; i++)
	{
		for (int j = i + 1; j < NUM; j++)
		{
			double matching_score = compareHist(histogram[i], histogram[j], CV_COMP_CORREL);
			cout << imgNames[i] << "-" << imgNames[j] << ", " << matching_score << endl;
		}
	}
	
}


