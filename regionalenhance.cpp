#include "regionalenhance.h"
#include "ui_regionalenhance.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include "convertimage.h"
#include "filter.h"
#include "enhance.h"
#include "gray2color.h"
#include "regionalenhance.h"
using namespace cv;
RegionalEnhance::RegionalEnhance(cv::Mat src_Regional_image,QWidget *parent) ://构造函数有默认值的要放后面
    QDialog(parent),Regional_image(src_Regional_image),
    ui(new Ui::RegionalEnhance)
{
    ui->setupUi(this);    
    show_image(Regional_image);
    dst=Regional_image.clone();
}

RegionalEnhance::~RegionalEnhance()
{
    delete ui;
}
void RegionalEnhance::check_select(){


    //cvtColor(Regional_image,dst,CV_GRAY2BGR);
    //imshow("enhance test",dst);
    //cvtColor(Regional_image,dst,CV_BGR2GRAY);
    dst=Regional_image.clone();
    Mat resize_img;
    cv::resize(dst,resize_img,Size(dst.cols*ui->DoubleSpinBox_Scale->value(),dst.rows*ui->DoubleSpinBox_Scale->value()),0,0,INTER_LINEAR);
    ui->label->setGeometry(0,0,resize_img.cols,resize_img.rows);
    dst=resize_img.clone();
    //正负片
    if(ui->CheckBox_negative->isChecked()){
        Enhance enhance_do;
        enhance_do.negative(dst,dst);
    }
    //去噪
    if(ui->CheckBox_denoise->isChecked()){
        Filter denoise_fillter;
        denoise_fillter.Guass_Filter(dst,dst);
    }
    //灰度范围
    int min=ui->spinBox_graydown->value();
    int max=ui->spinBox_grayup->value();
    Mat limit_img;
    limit_img=dst.clone();
    for(int i=0;i<dst.rows;i++)
        for(int j=0;j<dst.cols;j++){
            if((int)dst.at<uchar>(i,j)<min)
                limit_img.at<uchar>(i,j)=(uchar)0;
            else if((int)dst.at<uchar>(i,j)>max)
                limit_img.at<uchar>(i,j)=(uchar)255;
        }
    dst=limit_img.clone();

    if(ui->CheckBox_Enhance->isChecked()){
        /*Mat test1,test2;
        Enhance enhance_do;
        enhance_do.equalizeHist_Enhance(dst,test1);
        int min=ui->spinBox_graydown->value();
        int max=ui->spinBox_grayup->value();
        enhance_do.histogram_extension(dst,min,max,test2);
        dst=test1.clone();*/

        Mat enhance_img;
        Enhance enhance_do;
        enhance_do.histogram_extension(dst,min,max,enhance_img);
        dst=enhance_img.clone();
    }
    if(ui->CheckBox_sharpen->isChecked()){
        Enhance enhance_do;
        enhance_do.sharpenImage(dst,dst);
    }
    if(ui->CheckBox_pseudoColor->isChecked()){
        Gray2Color gray_color;
        Mat color_map;
        Mat color_img;
        gray_color.Create_map(256,color_map);
        gray_color.Pseudo_Color(dst,color_img,color_map);
        for(int i=0;i<limit_img.rows;i++)
            for(int j=0;j<limit_img.cols;j++){
                if((int)limit_img.at<uchar>(i,j)<min)
                    color_img.at<cv::Vec3b>(i,j)=cv::Vec3b(0,0,0);
                else if((int)limit_img.at<uchar>(i,j)>max)
                    color_img.at<cv::Vec3b>(i,j)=cv::Vec3b(255,255,255);
            }
        dst=color_img.clone();
    }
    show_image(dst);
}
void RegionalEnhance::show_image(Mat image){
    //imshow("test",image);
    QImage showImage;
    ConvertImage coverimg;
    showImage=coverimg.cvMat2QImage(image);
    ui->label->setPixmap(QPixmap::fromImage(showImage));

}


void RegionalEnhance::on_CheckBox_denoise_clicked()
{
    check_select();
}

void RegionalEnhance::on_CheckBox_Enhance_clicked()
{
    check_select();
}

void RegionalEnhance::on_CheckBox_sharpen_clicked()
{
    check_select();
}

void RegionalEnhance::on_DoubleSpinBox_Scale_valueChanged(double arg1)
{
    Mat resize_img;
    cv::resize(dst,resize_img,Size(dst.cols*arg1,dst.rows*arg1),0,0,INTER_LINEAR);
    ui->label->setGeometry(0,0,resize_img.cols,resize_img.rows);
    show_image(resize_img);
}

void RegionalEnhance::on_CheckBox_negative_clicked()
{
    check_select();
}

void RegionalEnhance::on_CheckBox_pseudoColor_clicked()
{
    check_select();
}
void RegionalEnhance::on_spinBox_graydown_valueChanged(int arg1)
{
    int Max =ui->spinBox_grayup->value();
    ui->spinBox_grayup->setRange(arg1, 255);  // 范围
    ui->spinBox_graydown->setRange(0, Max);  // 范围
    check_select();
}

void RegionalEnhance::on_spinBox_grayup_valueChanged(int arg1)
{
    int Min =ui->spinBox_graydown->value();
    ui->spinBox_grayup->setRange(Min, 255);  // 范围
    ui->spinBox_graydown->setRange(0, arg1);  // 范围
    check_select();
}
