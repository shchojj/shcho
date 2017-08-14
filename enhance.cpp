#include "enhance.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
using namespace cv;
Enhance::Enhance()
{

}
void Enhance::equalizeHist_Enhance(const cv::Mat &gray_image,cv::Mat &result_image){
    //直方图均衡
    //为输出图像分配内存
    result_image.create(gray_image.size(),gray_image.type());
    cv::equalizeHist(gray_image,result_image);
    //imshow("hist sys",result_image);
}

void Enhance::Laplace_Enhance(const cv::Mat &gray_image,cv::Mat &result_image){
    //拉普拉算子增强
    //为输出图像分配内存    
    result_image.create(gray_image.size(),gray_image.type());
    Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
    filter2D(gray_image, result_image, CV_8UC3, kernel);
    // imshow("median_Filter",result_image);
}
void Enhance::Log_Enhance(const cv::Mat &gray_image,cv::Mat &result_image){
    //对数Log增强,低灰度值部分扩展，将其高灰度值部分压缩,以达到强调图像低灰度部分的目的。
    //为输出图像分配内存
    result_image.create(gray_image.size(),gray_image.type());
    uchar lut[256] ={0};
    double temp = 255/log(256);
    for ( int i =0; i<255; i++)
    {
        lut[i] = (uchar)(temp* log(i+1)+0.5);
    }
    for( int row =0; row <gray_image.rows; row++)
    {
        for( int col = 0; col<gray_image.cols; col++)
        {
            result_image.at<uchar>(row, col)=lut[gray_image.at<uchar>(row, col)];
        }
    }
    //归一化到0~255
    normalize(result_image, result_image, 0, 255, CV_MINMAX);
    //转换成8bit图像显示
    convertScaleAbs(result_image, result_image);
}
void Enhance::Gamma_Enhance(const cv::Mat &gray_image,cv::Mat &result_image){
    ///伽马变换将漂白的图片或者是过黑的图片，进行修正。
    //运用迭代器访问矩阵元素
    // build look up table
    result_image.create(gray_image.size(),gray_image.type());
    result_image=gray_image.clone();
    float fGamma=1/2.2;
    unsigned char lut[256];
    for( int i = 0; i < 256; i++ )
    {
        lut[i] = pow((float)(i/255.0), fGamma) * 255.0;
    }
    MatIterator_<uchar> it, end;
    for( it = result_image.begin<uchar>(),end=result_image.end<uchar>(); it != end; it++ )
        *it = lut[(*it)];
    //归一化到0~255
    normalize(result_image, result_image, 0, 255, CV_MINMAX);
    //转换成8bit图像显示
    convertScaleAbs(result_image, result_image);
}
void Enhance::sharpenImage(const cv::Mat &gray_image, cv::Mat &result_image)
{
    //创建并初始化滤波模板
    cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));
    kernel.at<float>(1,1) = 5.0;
    kernel.at<float>(0,1) = -1.0;
    kernel.at<float>(1,0) = -1.0;
    kernel.at<float>(1,2) = -1.0;
    kernel.at<float>(2,1) = -1.0;
    result_image.create(gray_image.size(),gray_image.type());
    //对图像进行滤波
    cv::filter2D(gray_image,result_image,gray_image.depth(),kernel);
}
void Enhance::negative(const cv::Mat &gray_image, cv::Mat &result_image)
{
    result_image.create(gray_image.size(),gray_image.type());
    for( int row =0; row <gray_image.rows; row++)
    {
        for( int col = 0; col<gray_image.cols; col++)
        {
            result_image.at<uchar>(row, col)=255-gray_image.at<uchar>(row, col);
        }
    }
}
void Enhance::histogram_extension(const cv::Mat &gray_image,int min,int max, cv::Mat &result_image){
    result_image.create(gray_image.size(),gray_image.type());
    /*
    int  p[256];
    //赋值
    for(int i=0;i<256;i++)
        p[i]=0;

    //统计
    for(int i=0;i<gray_image.rows;i++)
        for(int j=0;j<gray_image.cols;j++)
            p[(int)gray_image.at<uchar>(i,j)]++;
    double p1[256];
    //赋值
    for(int i=0;i<256;i++)
        p1[i]=0.0;
    //灰度分布密度
    for(int i=0;i<256;i++)
        p1[i]=p[i]/(gray_image.rows*gray_image.cols*1.0);
    //累计直方图
    double p2[256];
    for(int i=0;i<256;i++)
        p2[i]=0.0;
    p2[0]=p2[0]+p1[0];
    for(int i=1;i<256;i++)
        p2[i]=p2[i-1]+p1[i];
    for(int i=0;i<256;i++)
        qDebug()<<p2[i];
    //累计分布取整
    int p3[256];
    for(int i=0;i<256;i++)
        p3[i]=0;
    for(int i=0;i<256;i++)
        p3[i]=cvFloor(p2[i]*255);
    //对灰度值进行映射（均衡化）
    for(int i=0;i<gray_image.rows;i++)
        for(int j=0;j<gray_image.cols;j++)
            result_image.at<uchar>(i,j) = (uchar)p3[(int)gray_image.at<uchar>(i,j)];
    imshow("result_image",result_image);
    */
    int sum=0;//计算要均衡区域个数
    for(int i=0;i<gray_image.rows;i++)
        for(int j=0;j<gray_image.cols;j++)
        {
            if(((int)gray_image.at<uchar>(i,j)>=min)&((int)gray_image.at<uchar>(i,j)<=max))
            {
                sum++;
            }
        }
    int  scale=max-min+1;
    int  p[scale];
    //初始化
    for(int i=0;i<scale;i++)
        p[i]=0;
    //统计
    for(int i=0;i<gray_image.rows;i++)
        for(int j=0;j<gray_image.cols;j++)
        {
            if(((int)gray_image.at<uchar>(i,j)>=min)&((int)gray_image.at<uchar>(i,j)<=max))
            {
                p[(int)gray_image.at<uchar>(i,j)-min]++;
            }
        }

    double p1[scale];
    //赋值
    for(int i=0;i<scale;i++)
        p1[i]=0.0;
    //灰度分布密度
    for(int i=0;i<scale;i++)
        p1[i]=p[i]/(sum*1.0);
    //累计直方图
    double p2[scale];
    for(int i=0;i<scale;i++)
        p2[i]=0.0;
    p2[0]=p1[0];
    for(int i=1;i<scale;i++)
        p2[i]=p2[i-1]+p1[i];
    //累计分布取整
    int p3[scale];
    for(int i=0;i<scale;i++)
        p3[i]=0;
    for(int i=0;i<scale;i++)
        p3[i]=cvFloor(p2[i]*255);
    //对灰度值进行映射（均衡化）
    for(int i=0;i<gray_image.rows;i++)
        for(int j=0;j<gray_image.cols;j++){
            if(((int)gray_image.at<uchar>(i,j)>=min)&((int)gray_image.at<uchar>(i,j)<=max))
                result_image.at<uchar>(i,j) = (uchar)p3[(int)gray_image.at<uchar>(i,j)-min];
            else if((int)gray_image.at<uchar>(i,j)<min)
                result_image.at<uchar>(i,j)=(uchar)0;
            else if((int)gray_image.at<uchar>(i,j)>max)
                result_image.at<uchar>(i,j)=(uchar)255;;
        }


    /////绘制直方图
    int size = scale;
    int scale1 = 1;
    Mat imageShow(size * scale1, size, CV_8U, Scalar(0));
    // 获取最大值和最小值
    double minVal = p1[0];
    double maxVal = p1[0];
    for(int i=1;i<scale;i++){
        if(p1[i]<minVal)
            minVal=p1[i];
        if(p1[i]>maxVal)
            maxVal=p1[i];
    }

   // minMaxLoc(hist,&minVal, &maxVal, 0, 0);  //  cv中用的是cvGetMinMaxHistValue
    //显示直方图的图像
    int hpt = saturate_cast<int>(0.9 * size);

    for(int i = 0; i < size; i++)
    {
        float value = p1[i];           //   注意hist中是float类型    cv中用cvQueryHistValue_1D
        int realValue = saturate_cast<int>(value * hpt/maxVal);
        rectangle(imageShow,Point(i*scale1, size - 1), Point((i+1)*scale1 - 1, size - realValue), Scalar(size-1));
    }
    namedWindow("Hist");
    imshow("Hist", imageShow);
    //imshow("result_image",result_image);
}
