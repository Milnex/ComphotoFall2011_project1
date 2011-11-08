/*
 * project1.h
 *
 *  Created on: 2011/11/5
 *      Author: milnex
 */

#ifndef __PROJECT1_H__
#define __PROJECT1_H__
#include "ui/processedimage.h"
#include <Wt/WContainerWidget>
#include <Wt/WTabWidget>
#include <Wt/WPanel>
#include <Wt/WDoubleSpinBox>
#include <Wt/WSlider>
#include <vector>

class Project1 : public Wt::WContainerWidget {
public:
	Project1(Wt::WContainerWidget *parent);
};


class SelfGuidedEnhancement : public IProcessedMethod {
public:
	SelfGuidedEnhancement(int radius, double epsilon) : r(radius), eps(epsilon) {}
	~SelfGuidedEnhancement() { }
	cv::Mat operator()(const cv::Mat &src);
private:
	int r;
	double eps;
};

class SmoothingMethod : public IProcessedMethod {
public:
	SmoothingMethod(int radius, double epsilon) : r(radius), eps(epsilon) {}
	~SmoothingMethod() { }
	cv::Mat operator()(const cv::Mat &src);
private:
	int r;
	double eps;
};


class DetailManipulation : public Wt::WContainerWidget {
public:
	DetailManipulation(bool enhance=true, Wt::WContainerWidget *parent=0);
private:
	void prepareInputImages();
	void selectImage(size_t id);
	void applyEnhancement(Wt::WDoubleSpinBox *r, Wt::WDoubleSpinBox *eps);
	void switchToOriginalImage();
	void switchToEnhancedImage();
	void changeDoubleSpinBoxValue(Wt::WDoubleSpinBox *box, bool isInt, int value);
	void changeSliderValue(Wt::WSlider *slider, double normalizer, double value);
private:
	const static int SLIDER_MINIMUM = 0;
	const static int SLIDER_MAXIMUM = 1000;
	const static int R_MINIMUM = 0;
	const static int R_MAXIMUM = 40;
	const static int EPS_MINIMUM = 0;
	const static int EPS_MAXIMUM = 1;
	bool onlySmooth;
	std::vector<ProcessedImage *> inputImages;
	Wt::WTabWidget *imageTab;
	size_t	selectedImageId;
	std::vector< std::pair<std::string, std::string> > smoothedResult;
	std::vector< std::pair<std::string, std::string> > enhancedResult;
};

class Smoothing : public Wt::WContainerWidget {
public:
	Smoothing(Wt::WContainerWidget *parent=0);
private:
	std::vector<ProcessedImage *> inputImages;
	size_t selectedImageId;
};

#endif /* __PROJECT1_H__ */
