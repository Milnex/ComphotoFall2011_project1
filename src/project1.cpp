/*
 * project1.cpp
 *
 *  Created on: 2011/11/5
 *      Author: milnex
 */
#include "ui/project1.h"
#include "filter/guidedfilter.h"
#include <Wt/WTemplate>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WBorderLayout>
#include <Wt/WImage>
#include <Wt/WGridLayout>
#include <Wt/WPushButton>
#include <sstream>
using namespace Wt;
using namespace std;

Project1::Project1(WContainerWidget *parent) : WContainerWidget(parent)
{
	// Read XML resource message
	WTemplate *mainTemplate = new WTemplate(WString::tr("project1"), this);
	mainTemplate->bindWidget("smoothing", new DetailManipulation(false));
	mainTemplate->bindWidget("detail_manipulation", new DetailManipulation(true));
}


DetailManipulation::DetailManipulation(bool enhance, WContainerWidget *parent)
	: WContainerWidget(parent), onlySmooth(!enhance)
{

	resize(WLength::Auto, WLength::Auto);

	// Image bar
	prepareInputImages();
	WContainerWidget *imageBar = new WContainerWidget;
	WVBoxLayout *imageBarLayout = new WVBoxLayout();
	for (size_t i = 0; i < inputImages.size(); ++i) {
		WImage *img = inputImages[i]->getOriginalImage();
		img->setStyleClass("image_button");
		img->resize(160, 120);
		img->setAttributeValue("onMouseOver", "this.width=192; this.height=144;");
		img->setAttributeValue("onMouseOut", "this.width=160; this.height=120;");
		img->clicked().connect(
				boost::bind(&DetailManipulation::selectImage,
							this,
							i
							)
		);
		imageBarLayout->addWidget(img);
	}
	imageBar->resize(200, WLength::Auto);
	imageBar->setLayout(imageBarLayout);
	selectedImageId = 0;

	// Main component
	imageTab = new WTabWidget();
	imageTab->addTab(inputImages[selectedImageId]->getOriginalImage(), "Original");
	if (onlySmooth) {
		imageTab->addTab(new WImage(smoothedResult[selectedImageId].second), smoothedResult[selectedImageId].first);
	}
	else {
		imageTab->addTab(new WImage(enhancedResult[selectedImageId].second), enhancedResult[selectedImageId].first);
	}
	imageTab->resize(600, WLength::Auto);


	WGridLayout *controllerLayout = new WGridLayout();
	WSlider *rSlider = new WSlider(Wt::Vertical);
	rSlider->setRange(SLIDER_MINIMUM, SLIDER_MAXIMUM);
	rSlider->setTickPosition(Wt::WSlider::TicksBothSides);
	WDoubleSpinBox *rSpinBox = new WDoubleSpinBox();
	rSpinBox->setMinimum(R_MINIMUM);
	rSpinBox->setMaximum(R_MAXIMUM);
	rSlider->sliderMoved().connect(
			boost::bind(&DetailManipulation::changeDoubleSpinBoxValue,
						this,
						rSpinBox,
						true,
						_1)
	);
	rSpinBox->valueChanged().connect(
			boost::bind(&DetailManipulation::changeSliderValue,
						this,
						rSlider,
						1.f/(R_MAXIMUM-R_MINIMUM),
						_1)
	);
	controllerLayout->addWidget(rSlider, 2, 0, 6, 1);
	controllerLayout->addWidget(rSpinBox, 9, 0);
	controllerLayout->addWidget(new WText("radius"), 10, 0);


	WSlider *epsSlider = new WSlider(Wt::Vertical);
	epsSlider->setMinimum(SLIDER_MINIMUM);
	epsSlider->setMaximum(SLIDER_MAXIMUM);
	epsSlider->setRange(SLIDER_MINIMUM, SLIDER_MAXIMUM);
	epsSlider->setTickPosition(WSlider::TicksBothSides);
	WDoubleSpinBox *epsSpinBox = new WDoubleSpinBox();
	epsSpinBox->setMinimum(EPS_MINIMUM);
	epsSpinBox->setMaximum(EPS_MAXIMUM);
	epsSlider->sliderMoved().connect(
			boost::bind(&DetailManipulation::changeDoubleSpinBoxValue,
						this,
						epsSpinBox,
						false,
						_1)
	);
	epsSpinBox->valueChanged().connect(
			boost::bind(&DetailManipulation::changeSliderValue,
						this,
						epsSlider,
						1.f/(EPS_MAXIMUM-EPS_MINIMUM),
						_1)
	);
	controllerLayout->addWidget(epsSlider, 2, 1, 6, 1);
	controllerLayout->addWidget(epsSpinBox, 9, 1);
	controllerLayout->addWidget(new WText("epsilon"), 10, 1);

	WPushButton *apply = new WPushButton("Apply");
	apply->clicked().connect(
			boost::bind(&DetailManipulation::applyEnhancement,
					this,
					rSpinBox,
					epsSpinBox
					)
	);
	controllerLayout->addWidget(apply, 1, 0, 1, 2);


	WContainerWidget *controller = new WContainerWidget();
	controller->resize(120, WLength::Auto);
	controller->setLayout(controllerLayout);


	WBorderLayout *mainLayout = new WBorderLayout(this);
	mainLayout->addWidget(imageBar, WBorderLayout::West);
	mainLayout->addWidget(imageTab, WBorderLayout::Center);
	mainLayout->addWidget(controller, WBorderLayout::East);
	//setLayout(mainLayout, Wt::AlignTop | Wt::AlignJustify);
	/*
		doJavaScript(
			    "(function () { \
						     var imageTags = document.getElementsByClassName(\"image_button\"); \
						     for (var i=0; image=imageTags[i]; ++i) { \
						         if (image.width>200 && image.height>0) { \
						             image.width = 192;  \
						         } \
						     }\
				 })();"
		);*/

}
void DetailManipulation::prepareInputImages()
{
	const char *imagePaths[][3] = {
			{"static/project1/flower.png", "static/project1/result/flower-smooth-r-16-eps-001.png", "static/project1/result/flower-enhance-r-16-eps-001.png"},
			{"static/project1/rock2.png", "static/project1/result/rock2-smooth-r-16-eps-001.png", "static/project1/result/rock2-enhance-r-16-eps-001.png"},
			{"static/project1/tulips.bmp", "static/project1/result/tulips-smooth-r-16-eps-001.png", "static/project1/result/tulips-enhance-r-16-eps-001.png"}
	};
	const int imageNum = 3;
	for (int i = 0; i < imageNum; ++i) {
		inputImages.push_back(new ProcessedImage(imagePaths[i][0]));
		smoothedResult.push_back( make_pair(string("r=16, eps=0.01"), string(imagePaths[i][1])) );
		enhancedResult.push_back( make_pair(string("r=16, eps=0.01"), string(imagePaths[i][2])) );
	}
}
void DetailManipulation::selectImage(size_t id)
{
	if (id == selectedImageId)
		return;
	const int tabCount = imageTab->count();
	selectedImageId = id;

	imageTab->addTab(inputImages[selectedImageId]->getOriginalImage(), "Original");
	if (onlySmooth) {
		imageTab->addTab(new WImage(smoothedResult[selectedImageId].second), smoothedResult[selectedImageId].first);
	}
	else {
		imageTab->addTab(new WImage(enhancedResult[selectedImageId].second), enhancedResult[selectedImageId].first);
	}
	for (int i = 0; i < tabCount; ++i) {
		imageTab->closeTab(i);
	}
}
void DetailManipulation::applyEnhancement(Wt::WDoubleSpinBox *r, Wt::WDoubleSpinBox *eps)
{
	if (onlySmooth) {
		inputImages[selectedImageId]->setProcessedMethod(new SmoothingMethod(r->value(), eps->value()));
	}
	else {
		inputImages[selectedImageId]->setProcessedMethod(new SelfGuidedEnhancement(r->value(), eps->value()));
	}
	string buf;
	stringstream title(buf);
	title << "r=" << r->value() << ", eps=" << eps->value();
	int last = imageTab->count();
	imageTab->addTab(inputImages[selectedImageId]->getProcessedImage(), title.str());
	imageTab->setTabCloseable(last, true);
	imageTab->setCurrentIndex(last);
}
void DetailManipulation::switchToOriginalImage()
{

}
void DetailManipulation::switchToEnhancedImage()
{

}
void DetailManipulation::changeDoubleSpinBoxValue(WDoubleSpinBox *box, bool isInt, int value)
{
	const static double normalizer = 1.f / (SLIDER_MAXIMUM-SLIDER_MINIMUM);
	if (isInt) {
		int v = box->minimum() + value*normalizer * (box->maximum()-box->minimum());
		box->setValue(v);
	}
	else {
		double v = box->minimum() + value*normalizer * (box->maximum()-box->minimum());
		box->setValue(v);
	}
}
void DetailManipulation::changeSliderValue(Wt::WSlider *slider, double normalizer, double value)
{
	const static int range = SLIDER_MAXIMUM-SLIDER_MINIMUM;
	slider->setValue(value*normalizer * range);
}


cv::Mat SelfGuidedEnhancement::operator()(const cv::Mat &src)
{
	cv::Mat p, ret;
	src.convertTo(p, CV_64FC3);
	p /= 255.f;

	// Run the guided filter on source image
	GuidedFilter guidedFilter(p, r, eps);
	std::vector<cv::Mat> pChannels(3), retChannels(3);
	cv::split(p, pChannels);
	for (int i = 0; i < 3; ++i) {
		retChannels[i] = guidedFilter(pChannels[i]);
	}
	cv::merge(retChannels, ret);
	cv::Mat enhanced = (p - ret)*5 + ret;

	enhanced *= 255.f;
	enhanced.convertTo(ret, src.type());
	return ret;
}

cv::Mat SmoothingMethod::operator()(const cv::Mat &src)
{
	cv::Mat p, ret;
	src.convertTo(p, CV_64FC3);
	p /= 255.f;

	// Run the guided filter on source image
	GuidedFilter guidedFilter(p, r, eps);
	std::vector<cv::Mat> pChannels(3), retChannels(3);
	cv::split(p, pChannels);
	for (int i = 0; i < 3; ++i) {
		retChannels[i] = guidedFilter(pChannels[i]);
	}
	cv::merge(retChannels, ret);

	ret *= 255.f;
	ret.convertTo(p, src.type());
	return p;
}
