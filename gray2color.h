#ifndef GRAY2COLOR_H
#define GRAY2COLOR_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
class Gray2Color
{
public:
    Gray2Color();
    static void Pseudo_Color(const cv::Mat &gray_image,cv::Mat &result_image,cv::Mat &color_map);
    static void Create_map(int Level,cv::Mat &result_image);
};

#endif // GRAY2COLOR_H
