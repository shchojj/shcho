#ifndef FILTER_H
#define FILTER_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class Filter
{
public:
    Filter();
    static void Guass_Filter(const cv::Mat &gray_image,cv::Mat &result_image);
    static void median_Filter(const cv::Mat &gray_image,cv::Mat &result_image);
    static void mean_Filter(const cv::Mat &gray_image,cv::Mat &result_image);
    static void bilateral_Filter(const cv::Mat &gray_image,cv::Mat &result_image);
    static void median_mean_filter(const cv::Mat &gray_image,cv::Mat &result_image);
    static int get_medianNum(Mat & imageROI);
};

#endif // FILTER_H
