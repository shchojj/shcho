#ifndef ENHANCE_H
#define ENHANCE_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class Enhance
{
public:
    Enhance();
    static void equalizeHist_Enhance(const cv::Mat &gray_image,cv::Mat &result_image);
    static void Laplace_Enhance(const cv::Mat &gray_image,cv::Mat &result_image);
    static void Log_Enhance(const cv::Mat &gray_image,cv::Mat &result_image);
    static void Gamma_Enhance(const cv::Mat &gray_image,cv::Mat &result_image);
    static void sharpenImage(const cv::Mat &gray_image, cv::Mat &result_image);
    static void negative(const cv::Mat &gray_image, cv::Mat &result_image);
    static void histogram_extension(const cv::Mat &gray_image,int min,int max, cv::Mat &result_image);
};

#endif // ENHANCE_H
