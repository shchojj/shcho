#include "graybound.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
GrayBound::GrayBound()
{

}
void GrayBound::gray_bound(const cv::Mat &gray_image,const cv::Mat &color_image,const int &gray_down,const int &gray_up,cv::Mat &result_image){
    //赋值
    //result_image.create(gray_image.size(),CV_8UC3);
    result_image=color_image.clone();
    //首先判断是彩色的还是灰度的
    int ci_channels=color_image.channels();
    if(ci_channels==1){
        if(gray_down>0){//设定了下限
            for(int gd=0;gd<=gray_down;gd++){//检索每一个下限在图像中的位置
                for( int row =0; row <gray_image.rows; row++){
                    for( int col = 0; col<gray_image.cols; col++){
                        int index=gray_image.at<uchar>(row, col);
                        if(index==gd){
                            result_image.at<uchar>(row, col)=0;
                        }
                    }
                }
            }
        }
        if(gray_up<255){//设定了下限
            for(int gu=255;gu>=gray_up;gu--){//检索每一个上限在图像中的位置
                for( int row =0; row <gray_image.rows; row++){
                    for( int col = 0; col<gray_image.cols; col++){
                        int index=gray_image.at<uchar>(row, col);
                        if(index==gu){
                            result_image.at<uchar>(row, col)=0;
                        }
                    }
                }
            }
        }
    }
    if(ci_channels==3){
        if(gray_down>0){//设定了下限
            for(int gd=0;gd<=gray_down;gd++){//检索每一个下限在图像中的位置
                for( int row =0; row <gray_image.rows; row++){
                    for( int col = 0; col<gray_image.cols; col++){
                        int index=gray_image.at<uchar>(row, col);
                        if(index==gd){
                            result_image.at<Vec3b>(row, col)=cv::Vec3b(0,0,0);
                        }
                    }
                }
            }
        }
        if(gray_up<255){//设定了下限
            for(int gu=255;gu>=gray_up;gu--){//检索每一个上限在图像中的位置
                for( int row =0; row <gray_image.rows; row++){
                    for( int col = 0; col<gray_image.cols; col++){
                        int index=gray_image.at<uchar>(row, col);
                        if(index==gu){
                            result_image.at<Vec3b>(row, col)=cv::Vec3b(0,0,0);
                        }
                    }
                }
            }
        }
    }
    //imshow("color img",result_image);

}
