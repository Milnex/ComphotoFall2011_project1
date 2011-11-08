/*
 * inputimage.h
 *
 *  Created on: 2011/11/5
 *      Author: milnex
 */

#ifndef __PROCESSEDIMAGE_H__
#define __PROCESSEDIMAGE_H__
#include <Wt/WImage>
#include <Wt/WMemoryResource>
#include <opencv2/core/core.hpp>
#include <string>

class IProcessedMethod {
public:
	virtual ~IProcessedMethod() {}
	virtual cv::Mat operator()(const cv::Mat &) = 0;
};

class ProcessedImage {
public:
	ProcessedImage(const std::string &imagePath);
	Wt::WImage *getOriginalImage();
	Wt::WImage *getProcessedImage();
	void setProcessedMethod(IProcessedMethod *method);
	std::string &getName();
private:
	Wt::WImage *MatToWImage(const cv::Mat &mat, const std::string &name);
private:
	const static std::string compressedType;
	cv::Mat srcMat;
	std::string srcPath;
	std::string srcName;
	Wt::WImage *dstImage;
	IProcessedMethod *method;
};


#endif /* __PROCESSEDIMAGE_H__ */
