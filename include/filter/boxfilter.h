#ifndef __BOXFILTER_H__
#define __BOXFILTER_H__
#include "filter.h"

class BoxFilter : public Filter {

  public:
    BoxFilter(int radius);
    virtual ~BoxFilter();
    cv::Mat operator()(const cv::Mat &src) ;

  private:
    enum AXIS_T {X_AXIS, Y_AXIS};
    cv::Mat cumsum(const cv::Mat &src, AXIS_T axis);

  private:
    int radius;
};

#endif // __BOXFILTER_H__
