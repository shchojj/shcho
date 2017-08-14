#include "evaluationindex.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include <vector>
using namespace cv;
EvaluationIndex::EvaluationIndex()
{

}
double EvaluationIndex::PSNR_Index(const cv::Mat &gray_image){
    double psnr_value;
    //qDebug()<<"几通道"<<gray_image.channels();
    cv::Scalar sum=cv::sum(gray_image);
    cv::Scalar mean=cv::mean(gray_image);
    //qDebug()<<"均值："<<mean.val[0];
    cv::Scalar stddev;
    cv::meanStdDev(gray_image,mean,stddev);
    //qDebug()<<"标准差"<<stddev.val[0];
    psnr_value=20*log10(255/stddev.val[0]);
    //qDebug()<<"峰值信噪比"<<psnr_value;
    return psnr_value;
}
