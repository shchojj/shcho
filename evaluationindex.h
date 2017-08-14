#ifndef EVALUATIONINDEX_H
#define EVALUATIONINDEX_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class EvaluationIndex
{
public:
    EvaluationIndex();
    static double PSNR_Index(const cv::Mat &gray_image);
};

#endif // EVALUATIONINDEX_H
