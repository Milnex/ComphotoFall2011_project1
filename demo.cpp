/*
 * demo.cpp
 *
 *  Created on: 2011/11/7
 *      Author: milnex
 */
#include "filter/guidedfilter.h"
#include <opencv2/highgui/highgui.hpp>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstring>
#include <sstream>
using namespace std;

enum ActionType {NONE, ENHANCE, SMOOTH};

void actionSmoothing(const string &inpath, const string &outpath, int r, double eps) {
	cv::Mat p, ret;
	cv::Mat src = cv::imread(inpath, CV_LOAD_IMAGE_ANYCOLOR);
	src.convertTo(p, CV_64FC3);
	p /= 255.f;

	// Run the guided filter on source image
	GuidedFilter guidedFilter(p, r, eps);
	vector<cv::Mat> pChannels(3), retChannels(3);
	cv::split(p, pChannels);
	for (int i = 0; i < 3; ++i) {
		retChannels[i] = guidedFilter(pChannels[i]);
	}
	cv::merge(retChannels, ret);
	ret *= 255.f;
	ret.convertTo(p, src.type());
	cv::imwrite(outpath, p);

}
void actionEnhancement(const string &inpath, const string &outpath, int r, double eps) {
	cv::Mat p, ret;
	cv::Mat src = cv::imread(inpath, CV_LOAD_IMAGE_ANYCOLOR);
	src.convertTo(p, CV_64FC3);
	p /= 255.f;

	// Run the guided filter on source image
	GuidedFilter guidedFilter(p, r, eps);
	vector<cv::Mat> pChannels(3), retChannels(3);
	cv::split(p, pChannels);
	for (int i = 0; i < 3; ++i) {
		retChannels[i] = guidedFilter(pChannels[i]);
	}
	cv::merge(retChannels, ret);
	cv::Mat enhanced = (p - ret)*5 + ret;

	enhanced *= 255.f;
	enhanced.convertTo(ret, src.type());
	cv::imwrite(outpath, enhanced);
}

int main(int argc, char **argv)
{
	static const char *helpString = "Usage: %s [method and params] --src [source] --dst [destination]\n\nMethod and Params: \n  --enhance  -r [radius] -eps [epsilon]\n  --smooth   -r [radius] -eps [epsilon]\n";
	if (argc < 10) {
		fprintf(stderr, helpString, argv[0]);
		return 0;
	}

	ActionType type = NONE;
	string infile, outfile;
	int r = 0;
	double eps = 0;

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--enhance") == 0) {
			type = ENHANCE;
		}
		else if (strcmp(argv[i], "--smooth") == 0) {
			type = SMOOTH;
		}
		else if (strcmp(argv[i], "-r") == 0) {
			string str(argv[i+1]);
			stringstream(str) >> r;
			i++;
		}
		else if (strcmp(argv[i], "-eps") == 0) {
			string str(argv[i+1]);
			stringstream(str) >> eps;
			i++;
		}
		else if (strcmp(argv[i], "--src") == 0) {
			infile = string(argv[i+1]);
			i++;
		}
		else if (strcmp(argv[i], "--dst") == 0) {
			outfile = string(argv[i+1]);
			i++;
		}
	}
	if (infile.empty() || outfile.empty() || type == NONE) {
		fprintf(stderr, helpString, argv[0]);
		return 0;
	}

	if (type == SMOOTH) {
		actionSmoothing(infile, outfile, r, eps);
	}
	else {
		actionEnhancement(infile, outfile, r, eps);
	}

	return 0;
}




