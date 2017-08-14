#ifndef GRAYBOUND_H
#define GRAYBOUND_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class GrayBound
{
public:
    GrayBound();
    static void gray_bound(const cv::Mat &gray_image,const cv::Mat &color_image,const int &gray_down,const int &gray_up,cv::Mat &result_image);
};

#endif // GRAYBOUND_H
