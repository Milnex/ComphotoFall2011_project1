#include "filter/boxfilter.h"
#include <stdexcept>
#include <iostream>

BoxFilter::BoxFilter(int r) {
	radius = r;
}

BoxFilter::~BoxFilter() {

}

cv::Mat BoxFilter::operator()(const cv::Mat &src) {
	if (src.empty())
		throw std::invalid_argument("Source matrix should not be empty");
	if (src.channels() != 1)
		throw std::invalid_argument("Source matrix must be single channel");
	if (src.dims != 2)
		throw std::invalid_argument("Wrong dimension of source matrix: " + src.dims);

	int rows = src.rows, cols = src.cols;
	cv::Mat dst(rows, cols, src.type(), cv::Scalar(0));

	// Compute cumulative sum over Y-axis
	cv::Mat cum = cumsum(src, Y_AXIS);
	for (int y = 0; y < radius+1; ++y)
		dst.row(y) = cum.row(y+radius) + cv::Scalar(0);
	for (int y = radius+1; y < rows-radius; ++y)
		dst.row(y) = cum.row(y+radius) - cum.row(y-radius-1);
	dst.rowRange(rows-radius, rows) = repeat(cum.row(rows-1), radius, 1) - cum.rowRange(rows-2*radius-1, rows-radius-1);


	// Compute cumulative sum over X-axis
	cum = cumsum(dst, X_AXIS);
	for (int x = 0; x < radius+1; ++x)
		dst.col(x) = cum.col(x+radius) + cv::Scalar(0);
	for (int x = radius+1; x < cols-radius; ++x)
		dst.col(x) = cum.col(x+radius) - cum.col(x-radius-1);
	dst.colRange(cols-radius, cols) = repeat(cum.col(cols-1), 1, radius) - cum.colRange(cols-2*radius-1, cols-radius-1);


	return dst;
}

cv::Mat BoxFilter::cumsum(const cv::Mat &src, AXIS_T axis) {
	if (src.empty())
		throw std::invalid_argument("Source matrix should not be empty");
	if (src.dims != 2)
		throw std::invalid_argument("Wrong dimension of source matrix: " + src.dims);

	cv::Mat dst = src.clone();
	switch (axis) {
	case X_AXIS:
		for (int x = 1; x < dst.cols; ++x)
			dst.col(x) += dst.col(x-1);
		break;
	case Y_AXIS:
		for (int y = 1; y < dst.rows; ++y)
			dst.row(y) += dst.row(y-1);
		break;
	}

	return dst;
}
