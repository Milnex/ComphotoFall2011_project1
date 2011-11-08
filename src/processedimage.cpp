/*
 * inputimage.cpp
 *
 *  Created on: 2011/11/5
 *      Author: milnex
 */
#include "ui/processedimage.h"
#include <opencv2/highgui/highgui.hpp>
#include <vector>
using namespace cv;
using namespace Wt;
using namespace std;

const string ProcessedImage::compressedType = ".png";

ProcessedImage::ProcessedImage(const string &imagePath) : srcPath(imagePath), method(NULL)
{
	srcMat = imread(imagePath, CV_LOAD_IMAGE_ANYCOLOR);
	srcName = imagePath.substr(imagePath.find_last_of('/')+1);
}

WImage *ProcessedImage::getOriginalImage()
{
	return new WImage(srcPath);
}

WImage *ProcessedImage::getProcessedImage()
{
	return method ? MatToWImage((*method)(srcMat), srcName) : NULL;
}
void ProcessedImage::setProcessedMethod(IProcessedMethod *m) {
	method = m;
}

WImage *ProcessedImage::MatToWImage(const Mat &mat, const string &name)
{
	vector<unsigned char> buf;
	imencode(compressedType, mat, buf);
	WMemoryResource *resource = new WMemoryResource("image/"+compressedType.substr(1));
	resource->setData(buf);
	return new Wt::WImage(resource, name);
}

string &ProcessedImage::getName() {
	return srcName;
}
