#ifndef CONVERTIMAGE_H
#define CONVERTIMAGE_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QImage>
using namespace cv;

class ConvertImage
{
public:
    ConvertImage();
    static QImage cvMat2QImage(const cv::Mat& mat);
    static cv::Mat QImage2cvMat(QImage image);
};

#endif // CONVERTIMAGE_H
