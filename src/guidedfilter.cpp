#include "filter/guidedfilter.h"
#include "filter/boxfilter.h"
#include <stdexcept>
#include <vector>

GuidedFilter::GuidedFilter(const cv::Mat &guidanceImage, int radius, double epsilon) {
	if (guidanceImage.channels() != 1 && guidanceImage.channels() != 3)
		throw std::invalid_argument("Guidance image must be a 1-channel or 3-channel image");

	I = guidanceImage;
	r = radius;
	eps = epsilon;
}

cv::Mat GuidedFilter::operator()(const cv::Mat &p) {
	if (p.dims != 2)
		throw std::invalid_argument("Wrong dimension of source matrix 'p': " + p.dims);
	if (p.channels() != 1)
		throw std::invalid_argument("Source matrix 'p' must be grayscale/single-channel");

	if (I.channels() == 1)
		return singleChannelFiltering(p);
	else
		return colorImageFiltering(p);

}

cv::Mat GuidedFilter::singleChannelFiltering(const cv::Mat &p) {
	BoxFilter boxFilter(r);

	cv::Mat N = boxFilter(cv::Mat::ones(p.rows, p.cols, p.type()));

	// Compute mean of guidance image
	cv::Mat mean_I = boxFilter(I) / N;
	// Compute mean of input image
	cv::Mat mean_p = boxFilter(p) / N;
	// Compute mean of (I, p)
	cv::Mat mean_Ip = boxFilter(I.mul(p)) / N;
	// Compute covariance of (I, p) in each local patch
	cv::Mat cov_Ip = mean_Ip - mean_I.mul(mean_p);

	cv::Mat mean_II = boxFilter(I.mul(I)) / N;
	cv::Mat var_I = mean_II - mean_I.mul(mean_I);

	// Eqn. (5) in the paper
	cv::Mat a = cov_Ip.mul(var_I + eps);
	// Eqn. (6) in the paper
	cv::Mat b = mean_p - a.mul(mean_I);

	cv::Mat mean_a = boxFilter(a) / N;
	cv::Mat mean_b = boxFilter(b) / N;

	// Eqn. (8) in the paper
	cv::Mat q = mean_a.mul(I) + mean_b;

	return q;
}

cv::Mat GuidedFilter::colorImageFiltering(const cv::Mat &p) {
	const int rows = p.rows, cols = p.cols;
	const int channelType = p.depth();
	std::vector<cv::Mat> Ichannels(3);
	cv::split(I, Ichannels);
	BoxFilter boxFilter(r);

	cv::Mat N = boxFilter(cv::Mat::ones(rows, cols, channelType));

	// Compute mean of each channel of guidance image
	cv::Mat mean_I_r = boxFilter(Ichannels[0]) / N;
	cv::Mat mean_I_g = boxFilter(Ichannels[1]) / N;
	cv::Mat mean_I_b = boxFilter(Ichannels[2]) / N;

	// Compute mean of input image
	cv::Mat mean_p = boxFilter(p) / N;

	// Compute mean of each channel of (I, p)
	cv::Mat mean_Ip_r = boxFilter(Ichannels[0].mul(p)) / N;
	cv::Mat mean_Ip_g = boxFilter(Ichannels[1].mul(p)) / N;
	cv::Mat mean_Ip_b = boxFilter(Ichannels[2].mul(p)) / N;

	// Compute covariance of (I, p) in each local patch
	cv::Mat cov_Ip_r = mean_Ip_r - mean_I_r.mul(mean_p);
	cv::Mat cov_Ip_g = mean_Ip_g - mean_I_g.mul(mean_p);
	cv::Mat cov_Ip_b = mean_Ip_b - mean_I_b.mul(mean_p);

	//variance of I in each local patch: the matrix Sigma in Eqn (14).
	//Note the variance in each local patch is a 3x3 symmetric matrix:
	//          rr, rg, rb
	//  Sigma = rg, gg, gb
	//          rb, gb, bb
	cv::Mat var_I_rr = boxFilter(Ichannels[0].mul(Ichannels[0])) / N - mean_I_r.mul(mean_I_r);
	cv::Mat var_I_rg = boxFilter(Ichannels[0].mul(Ichannels[1])) / N - mean_I_r.mul(mean_I_g);
	cv::Mat var_I_rb = boxFilter(Ichannels[0].mul(Ichannels[2])) / N - mean_I_r.mul(mean_I_b);
	cv::Mat var_I_gg = boxFilter(Ichannels[1].mul(Ichannels[1])) / N - mean_I_g.mul(mean_I_g);
	cv::Mat var_I_gb = boxFilter(Ichannels[1].mul(Ichannels[2])) / N - mean_I_g.mul(mean_I_b);
	cv::Mat var_I_bb = boxFilter(Ichannels[2].mul(Ichannels[2])) / N - mean_I_b.mul(mean_I_b);

	cv::Mat a = cv::Mat::zeros(rows, cols, I.type());
	cv::Mat epsXeye3 = cv::Mat::eye(3, 3, channelType) * eps;
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			double ary3x3[3][3] = {{var_I_rr.at<double>(y, x), var_I_rg.at<double>(y, x), var_I_rb.at<double>(y, x)},
								  {var_I_rg.at<double>(y, x), var_I_gg.at<double>(y, x), var_I_gb.at<double>(y, x)},
								  {var_I_rb.at<double>(y, x), var_I_gb.at<double>(y, x), var_I_bb.at<double>(y, x)}
			};
			cv::Mat Sigma(3, 3, channelType, ary3x3);

			double ary1x3[1][3] = {{cov_Ip_r.at<double>(y, x), cov_Ip_g.at<double>(y, x), cov_Ip_b.at<double>(y, x)}};
			cv::Mat cov_Ip(1, 3, channelType, ary1x3);

			cv::Mat ret = cov_Ip * (Sigma + epsXeye3).inv();
			a.at<cv::Vec3d>(y, x)[0] = ret.at<double>(0, 0);
			a.at<cv::Vec3d>(y, x)[1] = ret.at<double>(0, 1);
			a.at<cv::Vec3d>(y, x)[2] = ret.at<double>(0, 2);
		}
	}


	std::vector<cv::Mat> aChannels(3);
	cv::split(a, aChannels);

	// Eqn. (15) in the paper
	cv::Mat b = mean_p - aChannels[0].mul(mean_I_r) - aChannels[1].mul(mean_I_g) - aChannels[2].mul(mean_I_b);

	// Eqn. (16) in the paper
	cv::Mat q = ( boxFilter(aChannels[0]).mul(Ichannels[0])
				+ boxFilter(aChannels[1]).mul(Ichannels[1])
				+ boxFilter(aChannels[2]).mul(Ichannels[2])
				+ boxFilter(b)
				) / N;

	return q;
}
