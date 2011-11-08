#ifndef __GUIDEDFILTER_H__
#define __GUIDEDFILTER_H__
#include "filter.h"

class GuidedFilter : public Filter {

  public:
    GuidedFilter(const cv::Mat &guidanceImage, int radius, double epsilon);
    cv::Mat operator()(const cv::Mat &p);

  private:
    cv::Mat singleChannelFiltering(const cv::Mat &p);
    cv::Mat colorImageFiltering(const cv::Mat &p);

  private:
    cv::Mat I;
    int r;
    double eps;
};



#endif // __GUIDEDFILTER_H__
