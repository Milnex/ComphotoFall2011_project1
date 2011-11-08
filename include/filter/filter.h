#ifndef __FILTER_H__
#define __FILTER_H__
#include <opencv2/core/core.hpp>

class Filter {
  public:
    virtual cv::Mat operator()(const cv::Mat &) = 0;
    virtual ~Filter() {
    }
};

#endif // __FILTER_H__
