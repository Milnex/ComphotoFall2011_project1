#include <gtest/gtest.h>
#include "filter/guidedfilter.h"
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
using std::string;
using std::vector;
using std::cerr;
using std::endl;

void showResultImage(const string &title, const cv::Mat &img) {
	cv::namedWindow(title, CV_WINDOW_AUTOSIZE);
	cv::imshow(title, img);
	cv::waitKey(0);
}

TEST(TestGuidedFilter, DISABLED_Case_Smoothing) {
	// Test if source image can be read
	const string imgPath = "../../comphoto2011/static/cat.bmp";
	cv::Mat img;
	cv::imread(imgPath, CV_LOAD_IMAGE_GRAYSCALE).convertTo(img, CV_64F);
	img /= 255.f;
	EXPECT_FALSE(img.empty()) << "Can't read image: " << imgPath;

	// Run the guided filter on source image
	int r = 4;
	double eps = pow(0.2, 2);
	cv::Mat ret = GuidedFilter(img, r, eps)(img);
	EXPECT_FALSE(ret.empty()) << "Guided filtered result is an empty image";

	// Show the result
	showResultImage("Case_Smoothing", ret);
}

TEST(TestGuidedFilter, Case_Enhancement) {
	// Test if source image can be read
	const string imgPath = "../../comphoto2011/static/tulips.bmp";
	cv::Mat img;
	cv::imread(imgPath, CV_LOAD_IMAGE_COLOR).convertTo(img, CV_64FC3);
	img /= 255.f;
	EXPECT_FALSE(img.empty()) << "Can't read image: " << imgPath;

	// Run the guided filter on source image
	std::vector<cv::Mat> pChannels(3), retChannels(3);
	cv::split(img, pChannels);

	cv::Mat ret;
	int r = 4;
	double eps = pow(0.2, 2);
	GuidedFilter guidedFilter(img, r, eps);
	for (int i = 0; i < 3; ++i) {
		retChannels[i] = guidedFilter(pChannels[i]);
		EXPECT_FALSE(retChannels.empty()) << "Guided filtered result is an empty image";
	}
	cv::merge(retChannels, ret);
	cv::Mat enhanced = (img - ret)*5 + ret;

	// Show the result
	showResultImage("Case_Enhancement", enhanced);
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
