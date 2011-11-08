#include <gtest/gtest.h>
#include "filter/boxfilter.h"
#include <vector>
using std::vector;

namespace {

class TestBoxFilter : public ::testing::Test {
protected:
	static void SetUpTestCase() {
	}
	static void TearDownTestCase() {
	}
	static cv::Mat zero3x3;
	static cv::Mat eye3x3;
	static cv::Mat eye5x5;
};

cv::Mat TestBoxFilter::zero3x3 = cv::Mat::zeros(3, 3, CV_32F);
cv::Mat TestBoxFilter::eye3x3 = cv::Mat::eye(3, 3, CV_32F);
cv::Mat TestBoxFilter::eye5x5 = cv::Mat::eye(5, 5, CV_64F);
} // namespace


TEST_F(TestBoxFilter, Case_Zero3x3) {
	for (int r = 0; r < 2; ++r) {
		cv::Mat ret = BoxFilter(r)(zero3x3);
		EXPECT_TRUE(ret.rows==3 && ret.cols==3) << "Wrong dimension of filtered zero matrix: " << ret.rows << ", " << ret.cols;
		EXPECT_EQ(cv::countNonZero(ret == zero3x3), 9) << "Wrong result of filtered zero matrix:\n " << ret;
	}
}

TEST_F(TestBoxFilter, Case_Eye3x3) {
	vector<cv::Mat> ans(2);
	ans[0] = eye3x3;
	ans[1] = (cv::Mat_<float>(3,3) << 2, 2, 1, 2, 3, 2, 1, 2, 2);
	for (int r = 0; r < 2; ++r) {
		cv::Mat ret = BoxFilter(r)(eye3x3);
		EXPECT_TRUE(ret.rows==3 && ret.cols==3) << "Wrong dimension of filtered identity matrix: " << ret.rows << ", " << ret.cols;
		EXPECT_EQ(cv::countNonZero(ret == ans[r]), 9) << "Wrong result of filtered identity matrix:\n " << ret;
	}
}

TEST_F(TestBoxFilter, Case_Eye5x5) {
	vector<cv::Mat> ans(3);
	ans[0] = eye5x5;
	ans[1] = (cv::Mat_<double>(5,5) << 2, 2, 1, 0, 0,
									   2, 3, 2, 1, 0,
									   1, 2, 3, 2, 1,
									   0, 1, 2, 3, 2,
									   0, 0, 1, 2, 2);
	ans[2] = (cv::Mat_<double>(5,5) << 3, 3, 3, 2, 1,
									   3, 4, 4, 3, 2,
									   3, 4, 5, 4, 3,
									   2, 3, 4, 4, 3,
									   1, 2, 3, 3, 3);
	for (int r = 0; r < 3; ++r) {
		cv::Mat ret = BoxFilter(r)(eye5x5);
		EXPECT_TRUE(ret.rows==5 && ret.cols==5) << "Wrong dimension of filtered identity matrix: " << ret.rows << ", " << ret.cols;
		EXPECT_EQ(cv::countNonZero(ret == ans[r]), 25) << "Wrong result of filtered identity matrix(r="<< r <<"):\n " << ret;
	}
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
