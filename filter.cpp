#include "filter.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include <vector>
using namespace cv;
using namespace std;
Filter::Filter()
{

}
//输入形参为pass by conference-const，保证输入的图像不会被修改，并且为传递引用
void Filter::Guass_Filter(const cv::Mat &gray_image,cv::Mat &result_image){
    //为输出图像分配内存
    result_image.create(gray_image.size(),gray_image.type());
    GaussianBlur(gray_image,result_image,Size(5,5),0,0);
     //imshow("Guass_Filter",result_image);
}
void Filter::median_Filter(const cv::Mat &gray_image,cv::Mat &result_image){
    //为输出图像分配内存
    result_image.create(gray_image.size(),gray_image.type());
    medianBlur(gray_image,result_image,3);
    // imshow("median_Filter",result_image);
}
void Filter::mean_Filter(const cv::Mat &gray_image,cv::Mat &result_image){
    //为输出图像分配内存
    result_image.create(gray_image.size(),gray_image.type());
    blur(gray_image,result_image,Size(3,3),Point(-1,-1));
    // imshow("mean_Filter",result_image);
}
void Filter::bilateral_Filter(const cv::Mat &gray_image,cv::Mat &result_image){
    //result_image.release();
    //为输出图像分配内存
    Mat temp_img;
    if(gray_image.channels()==3){
        cvtColor(gray_image,temp_img,CV_BGR2GRAY);
    }else if(gray_image.channels()==1){
         temp_img=gray_image.clone();
    }
    result_image.create(temp_img.size(),temp_img.type());
    //if(gray_image.data==result_image.data)
       // qDebug()<<"have error";
    bilateralFilter(temp_img,result_image, 25, 25*2, 25/2);//

     //imshow("bilateral_Filter",result_image);
}
void Filter::median_mean_filter(const cv::Mat &gray_image,cv::Mat &result_image){
    Mat temp_img;
    if(gray_image.channels()==3){
        cvtColor(gray_image,temp_img,CV_BGR2GRAY);
    }else if(gray_image.channels()==1){
         temp_img=gray_image.clone();
    }
    result_image.create(temp_img.size(),temp_img.type());
    int N=5;//5*5的滤波器
    int k=cvFloor(N/2);
    ///取出要滤波的点
    for(int i=0;i<temp_img.rows;i++)
        for(int j=0;j<temp_img.cols;j++){
            if(i<k|i>(temp_img.rows-k-1)|j<k|j>(temp_img.rows-k-1))
                result_image.at<uchar>(i,j)=temp_img.at<uchar>(i,j);//边界点不计算
            else{
                Mat template_mat;
                template_mat.create(N,N,temp_img.type());
                ///分离出要处理的模板区域
                for(int i1=-k;i1<=k;i1++){
                    for(int j1=-k;j1<=k;j1++){
                        template_mat.at<uchar>(k+i1,k+j1)=temp_img.at<uchar>(i+i1,j+j1);
                    }
                }
                //自适应中值滤波
                //计算四个方向上的中值
                Mat z1=Mat::ones(1,N,template_mat.type());
                Mat z2=Mat::ones(1,N,template_mat.type());
                Mat z3=Mat::ones(1,N,template_mat.type());
                Mat z4=Mat::ones(1,N,template_mat.type());
                for(int i1=-k;i1<=k;i1++){
                    z1.at<uchar>(0,i1+k) = template_mat.at<uchar>(k+i1,k);
                    z2.at<uchar>(0,k+i1) = template_mat.at<uchar>(k+i1,k+i1);
                    z3.at<uchar>(0,k+i1) = template_mat.at<uchar>(k,k+i1);
                    z4.at<uchar>(0,k+i1) = template_mat.at<uchar>(k+i1,k-i1);
                }
                //求中值
                int zmed1=get_medianNum(z1);
                int zmed2=get_medianNum(z2);
                int zmed3=get_medianNum(z3);
                int zmed4=get_medianNum(z4);
                //计算四个方向上的差值;
                double d1=abs(zmed1-(double)template_mat.at<uchar>(i,j))+0.001;
                double d2=abs(zmed2-(double)template_mat.at<uchar>(i,j))+0.001;
                double d3=abs(zmed3-(double)template_mat.at<uchar>(i,j))+0.001;
                double d4=abs(zmed4-(double)template_mat.at<uchar>(i,j))+0.001;
                //计算四个方向个点对中心点贡献的大小。
                double sum_d=1/d1+1/d2+1/d3+1/d4;
                double w1=(1/d1)/sum_d;//偏差越大，贡献度越小
                double w2=(1/d2)/sum_d;
                double w3=(1/d3)/sum_d;
                double w4=(1/d4)/sum_d;
                //计算中心像素点
                result_image.at<uchar>(i,j)=zmed1*w1+zmed2*w2+zmed3*w3+zmed4*w4;
                /*double mu=0;
                double template_sum=0;
                double Scala_k=300.0;
                //基于模糊隶属度加权均值滤波器。
                for(int i1=-k;i1<=k;i1++)
                    for(int j1=-k;j1<=k;j1++){
                        //计算隶属度
                        int template_temp=abs((int)template_mat.at<uchar>(k,k)-(int)template_mat.at<uchar>(k+i1,k+j1));
                        mu=std::exp(-template_temp*template_temp*1.0/Scala_k);
                        template_sum+=mu*((double)template_mat.at<uchar>(k,k));
                    }
                result_image.at<uchar>(i,j)=template_sum;*/
            }
        }












   /* ///取出要滤波的点
    for( int i =k; i <temp_img.rows-k; i++)
    {
        for( int j = k; j<temp_img.cols-k; j++)
        {
            Mat template_mat;
            template_mat.create(N,N,temp_img.type());
            ///分离出要处理的模板区域
            for(int i_1=-k;i_1<=k;i_1++){
                for(int j_1=-k;j_1<=k;j_1++){
                    template_mat.at<uchar>(k+i_1,k+j_1)=gray_image.at<uchar>(i+i_1,j+j_1);
                }
            }
            ///噪声分离
            //获取要处理的模板中值
            int template_median=get_medianNum(template_mat);

            //计算该点与中值的绝对值之差
            int l_i_j=abs((int)template_mat.at<uchar>(k,k)-template_median);

            //最近的两个点
            int a=abs((int)template_mat.at<uchar>(k,k-1)-(int)template_mat.at<uchar>(k,k));
            int b=abs((int)template_mat.at<uchar>(k+1,k)-(int)template_mat.at<uchar>(k,k));
            double v_i_j=(double)(a+b)/2.0;

            //计算T1
            Mat noise_mat;
            noise_mat.create(N,N,gray_image.type());
            for(int i_2=0;i_2<=2*k;i_2++){
                for(int j_2=0;j_2<=2*k;j_2++){
                    noise_mat.at<uchar>(i_2,j_2)=(uchar)abs((int)template_mat.at<uchar>(i_2,j_2)-template_median);
                }
            }
            int noise_i=get_medianNum(noise_mat);

            double T1=2*1.75*noise_i;

            //计算T2  6~10之间
            int T2=10;
            //qDebug()<<"noise_i"<<noise_i<<"; l_i_j="<<l_i_j<<"; v_i_j="<<v_i_j<<"; T1="<<T1<<"; T2="<<T2;
            ////////////////////
            //计算四个方向上的中值
            Mat z1=Mat::ones(1,N,template_mat.type());
            Mat z2=Mat::ones(1,N,template_mat.type());
            Mat z3=Mat::ones(1,N,template_mat.type());
            Mat z4=Mat::ones(1,N,template_mat.type());
            //水平方向上
            for(int i1=-k;i1<=k;i1++){
                z1.at<uchar>(0,k+i1) = template_mat.at<uchar>(k,k+i1);
            }
            //45°方向上
            for (int i1=-k;i1<=k;i1++){
                z2.at<uchar>(0,k+i1) = template_mat.at<uchar>(k+i1,k-i1);
            }
            //垂直方向
            for(int i1=-k;i1<=k;i1++){
                z3.at<uchar>(0,i1+k) = template_mat.at<uchar>(k+i1,k);
            }
            //135°方向
            for(int i1=-k;i1<=k;i1++){
                z4.at<uchar>(0,k+i1) = template_mat.at<uchar>(k+i1,k+i1);
            }
            //求中值
            int zmed1=get_medianNum(z1);//%水平方向上
            int zmed2=get_medianNum(z2);//%45°方向上
            int zmed3=get_medianNum(z3);//%垂直方向
            int zmed4=get_medianNum(z4);//%135°方向
            //计算四个方向上的差值;
            double d1=abs(zmed1-(double)result_image.at<uchar>(i,j));
            double d2=abs(zmed2-(double)result_image.at<uchar>(i,j));
            double d3=abs(zmed3-(double)result_image.at<uchar>(i,j));
            double d4=abs(zmed4-(double)result_image.at<uchar>(i,j));
            //计算四个方向个点对中心点贡献的大小。
            double sum_d=1/d1+1/d2+1/d3+1/d4;
            double w1=(1/d1)/sum_d;//偏差越大，贡献度越小
            double w2=(1/d2)/sum_d;
            double w3=(1/d3)/sum_d;
            double w4=(1/d4)/sum_d;
            //计算中心像素点
            temp_img.at<uchar>(i,j)=zmed1*w1+zmed2*w2+zmed3*w3+zmed4*w4;
            //temp_img.at<uchar>(i,j)=get_medianNum(template_mat);
            //temp_img.at<uchar>(i,j)=zmed2;


            ///三种去噪规则
            ///l_i_j>=T1&v_i_j>=T2自适应中值滤波
            if(l_i_j>=T1&v_i_j>=T2){
                //计算四个方向上的中值
                Mat z1=Mat::ones(1,2*k+1,temp_img.type());
                Mat z2=Mat::ones(1,2*k+1,temp_img.type());
                Mat z3=Mat::ones(1,2*k+1,temp_img.type());
                Mat z4=Mat::ones(1,2*k+1,temp_img.type());
                //水平方向上
                for(int i1=-k;i1<=k;i1++){
                    z1.at<uchar>(0,k+i1) = template_mat.at<uchar>(k,k+i1);
                }
                //45°方向上
                for (int i1=-k;i1<=k;i1++){
                    z2.at<uchar>(0,k+i1) = template_mat.at<uchar>(k+i1,k-i1);
                }
                //垂直方向
                for(int i1=-k;i1<=k;i1++){
                    z3.at<uchar>(0,i1+k) = template_mat.at<uchar>(k+i1,k);
                }
                //135°方向
                for(int i1=-k;i1<=k;i1++){
                    z4.at<uchar>(0,k+i1) = template_mat.at<uchar>(k+i1,k+i1);
                }
                //求中值
                int zmed1=get_medianNum(z1);//%水平方向上
                int zmed2=get_medianNum(z2);//%45°方向上
                int zmed3=get_medianNum(z3);//%垂直方向
                int zmed4=get_medianNum(z4);//%135°方向
                //计算四个方向上的差值;
                int d1=abs(zmed1-(template_mat.at<uchar>(k,k)-'0'));
                int d2=abs(zmed2-(template_mat.at<uchar>(k,k)-'0'));
                int d3=abs(zmed3-(template_mat.at<uchar>(k,k)-'0'));
                int d4=abs(zmed4-(template_mat.at<uchar>(k,k)-'0'));
                //计算四个方向个点对中心点贡献的大小。
                double sum_d=1/d1+1/d2+1/d3+1/d4;
                double w1=(1/d1)/sum_d;
                double w2=(1/d2)/sum_d;
                double w3=(1/d3)/sum_d;
                double w4=(1/d4)/sum_d;
                //计算中心像素点
                temp_img.at<uchar>(i,j)=cvFloor(zmed1*w1+zmed2*w2+zmed3*w3+zmed4*w4)+'0';
            }
            ///((l_i_j>=T1&v_i_j<T2)||l_i_j<T1)基于模糊隶属度加权均值滤波器
            if((l_i_j>=T1&v_i_j<T2)|(l_i_j<T1)){
                double mu=0;
                double template_sum=0;
                //基于模糊隶属度加权均值滤波器。
                for(int ls_i=0;ls_i<=2*k;ls_i++)
                    for(int ls_j=0;ls_j<=2*k;ls_j++){
                        //计算隶属度
                        double template_temp=template_mat.at<uchar>(k,k)-template_mat.at<uchar>(ls_i,ls_j)-'0';
                        mu=std::exp(-template_temp*template_temp/N);
                        template_sum+=mu*(template_mat.at<uchar>(k,k)-'0');
                    }

                temp_img.at<uchar>(i,j)=cvFloor(template_sum)-'0';
            }
            //////////////////////////
        }
    }*/
    normalize(result_image, result_image, 0, 255, CV_MINMAX);
}
int Filter::get_medianNum(Mat & imageROI)	//获取矩阵的中值
{
    std::vector<uchar> vectorTemp;
    for (int i = 0; i < imageROI.rows; i++)	//将感兴趣区域矩阵拉成一个向量
    {
        for (int j = 0; j < imageROI.cols; j++)
        {
            vectorTemp.push_back(imageROI.at<uchar>(i, j));
        }
    }
    for (int i = 0; i < (int)vectorTemp.size(); i++)	//进行排序
    {
        for (int j = i + 1; j < (int)vectorTemp.size(); j++)
        {
            if ((int)vectorTemp.at(i) > (int)vectorTemp.at(j))
            {
                uchar temp;
                temp = vectorTemp.at(i);
                vectorTemp.at(i) = vectorTemp.at(j);
                vectorTemp.at(j) = temp;
            }
        }
    }
    return vectorTemp.at(cvCeil((int)vectorTemp.size()/2));	//返回中值
}
