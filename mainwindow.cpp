#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QButtonGroup>
#include "filter.h"
#include "enhance.h"
#include "gray2color.h"
#include "regionalenhance.h"
#include "convertimage.h"
#include "graybound.h"
#include "evaluationindex.h"
QButtonGroup *btnGroupFilter;
QButtonGroup *btnGroupEnhance;
QButtonGroup *btnGroupColor;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //滤波
    ui->setupUi(this);
    btnGroupFilter = new QButtonGroup(this);
    btnGroupFilter->addButton(ui->radioButton_Gaussian, 0);
    btnGroupFilter->addButton(ui->radioButton_median, 1);
    btnGroupFilter->addButton(ui->radioButton_mean, 2);
    btnGroupFilter->addButton(ui->radioButton_bilateral, 3);
    btnGroupFilter->addButton(ui->radioButton_Mixed, 4);
    btnGroupFilter->addButton(ui->radioButton_nonfilter, 5);
    connect(ui->radioButton_Gaussian, SIGNAL(clicked()), this, SLOT(onRadioClickFilter()));
    connect(ui->radioButton_median, SIGNAL(clicked()), this, SLOT(onRadioClickFilter()));
    connect(ui->radioButton_mean, SIGNAL(clicked()), this, SLOT(onRadioClickFilter()));
    connect(ui->radioButton_bilateral, SIGNAL(clicked()), this, SLOT(onRadioClickFilter()));
    connect(ui->radioButton_nonfilter, SIGNAL(clicked()), this, SLOT(onRadioClickFilter()));
    connect(ui->radioButton_Mixed, SIGNAL(clicked()), this, SLOT(onRadioClickFilter()));
    ui->radioButton_nonfilter->setChecked(true);
    //增强
    btnGroupEnhance = new QButtonGroup(this);
    btnGroupEnhance->addButton(ui->radioButton_equalizeHist, 0);
    btnGroupEnhance->addButton(ui->radioButton_Laplace, 1);
    btnGroupEnhance->addButton(ui->radioButton_Log, 2);
    btnGroupEnhance->addButton(ui->radioButton_Gamma, 3);
    btnGroupEnhance->addButton(ui->radioButton_nonEnhance, 4);
    connect(ui->radioButton_equalizeHist, SIGNAL(clicked()), this, SLOT(onRadioClickEnhance()));
    connect(ui->radioButton_Laplace, SIGNAL(clicked()), this, SLOT(onRadioClickEnhance()));
    connect(ui->radioButton_Log, SIGNAL(clicked()), this, SLOT(onRadioClickEnhance()));
    connect(ui->radioButton_Gamma, SIGNAL(clicked()), this, SLOT(onRadioClickEnhance()));
    connect(ui->radioButton_nonEnhance, SIGNAL(clicked()), this, SLOT(onRadioClickEnhance()));
    ui->radioButton_nonEnhance->setChecked(true);
    //色彩
    btnGroupColor = new QButtonGroup(this);
    btnGroupColor->addButton(ui->radioButton_256, 0);
    btnGroupColor->addButton(ui->radioButton_128, 1);
    btnGroupColor->addButton(ui->radioButton_64, 2);
    btnGroupColor->addButton(ui->radioButton_32, 3);
    btnGroupColor->addButton(ui->radioButton_16, 4);
    btnGroupColor->addButton(ui->radioButton_8, 5);
    btnGroupColor->addButton(ui->radioButton_4, 6);
    btnGroupColor->addButton(ui->radioButton_gray, 7);
    connect(ui->radioButton_256, SIGNAL(clicked()), this, SLOT(onRadioClickColor()));
    connect(ui->radioButton_128, SIGNAL(clicked()), this, SLOT(onRadioClickColor()));
    connect(ui->radioButton_64, SIGNAL(clicked()), this, SLOT(onRadioClickColor()));
    connect(ui->radioButton_32, SIGNAL(clicked()), this, SLOT(onRadioClickColor()));
    connect(ui->radioButton_16, SIGNAL(clicked()), this, SLOT(onRadioClickColor()));
    connect(ui->radioButton_8, SIGNAL(clicked()), this, SLOT(onRadioClickColor()));
    connect(ui->radioButton_4, SIGNAL(clicked()), this, SLOT(onRadioClickColor()));
    connect(ui->radioButton_gray, SIGNAL(clicked()), this, SLOT(onRadioClickColor()));
    ui->radioButton_gray->setChecked(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_read_img_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),
                                    ".",tr("Image Files (*.png *.jpg *.bmp)"));
    //判断是否有选择文件
    if("" == fileName){
        QMessageBox::warning(this,QString("warning"),QString("No image was selected!"));
        return;
    }
    //读取文件，并把QString类型转化为string
    src = imread(fileName.toLocal8Bit().toStdString());
    if(src.empty()) return;
    cvtColor(src,dst,CV_BGR2RGB);//opencv读取图片按照BGR方式读取，为了正常显示，所以将BGR转为RGB
    show_image(dst);
}
void MainWindow::onRadioClickFilter()
{
    check_select();
}
void MainWindow::onRadioClickEnhance()
{
   check_select();
}

void MainWindow::on_checkBox_sharpen_clicked()
{
    check_select();
}
void MainWindow::onRadioClickColor()
{
    check_select();
}
void MainWindow::paintEvent(QPaintEvent *e)
{
    show_image(dst);
}

void MainWindow::on_show_PrtSc_clicked()
{
    scaledW=(float)src.cols/ui->src_img->width();
    scaledH=(float)src.rows/ui->src_img->height();
    if(ui->src_img->p1.rx()!=0){
        QPoint p1,p2;
        p1.setX(scaledW*(ui->src_img->p1.x()));
        p2.setX(scaledW*(ui->src_img->p2.x()));
        p1.setY(scaledH*(ui->src_img->p1.y()));
        p2.setY(scaledH*(ui->src_img->p2.y()));
        Rect rect(p1.rx(),p1.ry(),(p2.rx()-p1.rx()),(p2.y()-p1.y()));
        Mat src_printscreen=src(rect);
        //imshow("src_printscreen",src_printscreen);
        //Mat enhance_image;
        Mat dst_printscreen;
        cvtColor(src_printscreen,dst_printscreen,CV_BGR2GRAY);
       /* Enhance select_enhance ;
        select_enhance.equalizeHist_Enhance(dst_printscreen,enhance_image);
        //imshow("enhance_printscreen",enhance_image);
        Mat sharpen_image;        
        select_enhance.sharpenImage(enhance_image,sharpen_image);
        //imshow("sharpen_printscreen",sharpen_image);*/
        RegionalEnhance* Regional_Dialog =new RegionalEnhance(dst_printscreen,this);
        Regional_Dialog->show();
    }
    else{
        QMessageBox::warning(this,QString("warning"),QString("No image was selected!"));
        return;
    }
}
void MainWindow::show_image(Mat image){
    if(image.channels()==1){
        cvtColor(image,image,CV_GRAY2BGR);
    }
    QImage showImage;
    //ConvertImage coverimg;
    showImage=ConvertImage::cvMat2QImage(image);
    //showImage=coverimg.cvMat2QImage(image);
    //showImage=showImage.scaled(ui->src_img->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->src_img->setPixmap(QPixmap::fromImage(showImage));
}
void MainWindow::show_colormap(Mat image){
    QImage showImage;
    //ConvertImage coverimg;
    //showImage=coverimg.cvMat2QImage(image);
    showImage=ConvertImage::cvMat2QImage(image);
    ui->label_colormap->setPixmap(QPixmap::fromImage(showImage));
}

void MainWindow::check_select(){
    if(src.empty()){
        QMessageBox::warning(this,QString("warning"),QString("No image was selected!"));
        return;
    }
    cvtColor(src,dst,CV_BGR2GRAY);
    //imshow("test1",dst);
    //是否是负片
    if(ui->CheckBox_negative->isChecked()){
        Mat enhance_img;
        //Enhance enhance_do;
        //enhance_do.negative(dst,enhance_img);
        Enhance::negative(dst,enhance_img);
        dst=enhance_img.clone();
    }
    //imshow("test2",dst);
    //去噪
    Filter select_filter ;
    Mat filter_img;
    filter_img.create(dst.size(),dst.type());
    switch(btnGroupFilter->checkedId())
    {
    case 0:
        select_filter.Guass_Filter(dst,filter_img);
        //imshow("test3",filter_img);
        //qDebug() << "Guass_Filter";
        break;
    case 1:
        select_filter.median_Filter(dst,filter_img);
        //qDebug() << "median_Filter";
        break;
    case 2:
        select_filter.mean_Filter(dst,filter_img);
        //qDebug() << "mean_Filter";
        break;
    case 3:
        select_filter.bilateral_Filter(dst,filter_img);
        //qDebug() << "bilateral_Filter";
        break;
    case 4:
        select_filter.median_mean_filter(dst,filter_img);
        //qDebug() << "bilateral_Filter";
        break;
    default:
        filter_img=dst;
        break;
    }
    dst=filter_img.clone();
    double psnr_index=EvaluationIndex::PSNR_Index(filter_img);
    qDebug()<<"峰值信噪比:"<<psnr_index;

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


    //imshow("test4",dst);
    //增强
    Enhance select_enhance ;
    Mat enhance_img;
    switch(btnGroupEnhance->checkedId())
    {
    case 0:
        //select_enhance.equalizeHist_Enhance(dst,enhance_img);
        select_enhance.histogram_extension(dst,min,max,enhance_img);
        break;
    case 1:
        select_enhance.Laplace_Enhance(dst,enhance_img);
        break;
    case 2:
        select_enhance.Log_Enhance(dst,enhance_img);
        break;
    case 3:
        select_enhance.Gamma_Enhance(dst,enhance_img);
        break;
    default:
        enhance_img=dst;
        break;
    }
    dst=enhance_img.clone();
    //imshow("test5",dst);
    //锐化
    if(ui->checkBox_sharpen->isChecked())
    {
        Mat sharpen_img;
        Enhance select_sharpen;
        select_sharpen.sharpenImage(dst,sharpen_img);
        dst=sharpen_img.clone();
    }   
    //伪彩    
    Gray2Color gray_color;
    Mat color_map;
    Mat color_img;
    switch(btnGroupColor->checkedId())
    {
    case 0:
        gray_color.Create_map(256,color_map);
        show_colormap(color_map);
        gray_color.Pseudo_Color(dst,color_img,color_map);
        break;
    case 1:
        gray_color.Create_map(128,color_map);
        show_colormap(color_map);
        gray_color.Pseudo_Color(dst,color_img,color_map);
        break;
    case 2:
        gray_color.Create_map(64,color_map);
        show_colormap(color_map);
        gray_color.Pseudo_Color(dst,color_img,color_map);
        break;
    case 3:
        gray_color.Create_map(32,color_map);
        show_colormap(color_map);
        gray_color.Pseudo_Color(dst,color_img,color_map);
        break;
    case 4:
        gray_color.Create_map(16,color_map);
        show_colormap(color_map);
        gray_color.Pseudo_Color(dst,color_img,color_map);
        break;
    case 5:
        gray_color.Create_map(8,color_map);
        show_colormap(color_map);
        gray_color.Pseudo_Color(dst,color_img,color_map);
        break;
    case 6:
        gray_color.Create_map(4,color_map);
        show_colormap(color_map);
        gray_color.Pseudo_Color(dst,color_img,color_map);
        break;
    default:
        color_map=Mat::eye(256,20,CV_8UC3);
        show_colormap(color_map);
        color_img=dst;
        break;
    }
    if(btnGroupColor->checkedId()!=7){
         for(int i=0;i<limit_img.rows;i++)
             for(int j=0;j<limit_img.cols;j++){
                 if((int)limit_img.at<uchar>(i,j)<min)
                     color_img.at<cv::Vec3b>(i,j)=cv::Vec3b(0,0,0);
                 else if((int)limit_img.at<uchar>(i,j)>max)
                     color_img.at<cv::Vec3b>(i,j)=cv::Vec3b(255,255,255);
             }
    }
    dst=color_img.clone();

    //imshow("testdtdstd",color_img);
}

void MainWindow::on_CheckBox_negative_clicked()
{
    check_select();
}

void MainWindow::on_spinBox_graydown_valueChanged(int arg1)
{
    int Max =ui->spinBox_grayup->value();
    ui->spinBox_grayup->setRange(arg1, 255);  // 范围
    ui->spinBox_graydown->setRange(0, Max);  // 范围
    check_select();
}

void MainWindow::on_spinBox_grayup_valueChanged(int arg1)
{
    int Min =ui->spinBox_graydown->value();
    ui->spinBox_grayup->setRange(Min, 255);  // 范围
    ui->spinBox_graydown->setRange(0, arg1);  // 范围
    check_select();
}

void MainWindow::on_temperature_clicked()
{
    scaledW=(float)src.cols/ui->src_img->width();
    scaledH=(float)src.rows/ui->src_img->height();
    Mat temperature_img;
    cvtColor(src,temperature_img,CV_BGR2GRAY);
    int min_tem,middle_tem,max_tem;
    //点温
    if(ui->src_img->p_right_start.rx()!=0&ui->src_img->p_right_start==ui->src_img->p_right_end)
    {
        QPoint p_right_start;
        p_right_start.setX(scaledW*(ui->src_img->p_right_start.x()));
        p_right_start.setY(scaledH*(ui->src_img->p_right_start.y()));

        //qDebug()<<temperature_img.at<uchar>(p_right_start.x(),p_right_start.y());
        min_tem=temperature_img.at<uchar>(p_right_start.x(),p_right_start.y());
        middle_tem=temperature_img.at<uchar>(p_right_start.x(),p_right_start.y());
        max_tem=temperature_img.at<uchar>(p_right_start.x(),p_right_start.y());
    }else if(ui->src_img->p_right_start.rx()!=0&ui->src_img->p_right_start!=ui->src_img->p_right_end){//线温
        QPoint p_right_start;
        QPoint p_right_end;
        if(ui->src_img->p_right_start.x()<=ui->src_img->p_right_end.x()){
            p_right_start.setX(scaledW*(ui->src_img->p_right_start.x()));
            p_right_start.setY(scaledH*(ui->src_img->p_right_start.y()));
            p_right_end.setX(scaledW*(ui->src_img->p_right_end.x()));
            p_right_end.setY(scaledH*(ui->src_img->p_right_end.y()));
        }else{
            p_right_start.setX(scaledW*(ui->src_img->p_right_end.x()));
            p_right_start.setY(scaledH*(ui->src_img->p_right_end.y()));
            p_right_end.setX(scaledW*(ui->src_img->p_right_start.x()));
            p_right_end.setY(scaledH*(ui->src_img->p_right_start.y()));
        }

        //列出直线方程
        int y;
        min_tem,middle_tem,max_tem=0;
        int sum=0;
        for(int p_right_index=p_right_start.x();p_right_index<=p_right_end.x();p_right_index++){
            y=p_right_end.y()+(p_right_index-p_right_end.x())*((p_right_start.y()-p_right_end.y())/(p_right_start.x()-p_right_end.x()));
            min_tem,middle_tem,max_tem=0;
            int temp_value=temperature_img.at<uchar>(p_right_index,y);
            if(temp_value>max_tem){
                max_tem=temp_value;
            }
            if(temp_value<min_tem){
                min_tem=temp_value;
            }
            sum+=temp_value;
        }
        middle_tem=sum/(p_right_end.x()-p_right_start.x());
    }
    /*int rowIndex = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowIndex + 1);//总行数增加1
        ui->tableWidget->setItem(rowIndex,　0,　new　QTableWidgetItem(QString("min_tem")));
        ui->tableWidget->setItem(rowIndex,　1,　new　QTableWidgetItem(QString("middle_tem")));
        ui->tableWidget->setItem(rowIndex,　2,　new　QTableWidgetItem(QString("max_tem")));*/
    int rowIndex = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowIndex);
    QTableWidgetItem *item ;
    item= new QTableWidgetItem(tr("%1").arg(min_tem));
    ui->tableWidget->setItem(rowIndex,0,item);
    item= new QTableWidgetItem(tr("%1").arg(middle_tem));
    ui->tableWidget->setItem(rowIndex,1,item);
    item= new QTableWidgetItem(tr("%1").arg(max_tem));
    ui->tableWidget->setItem(rowIndex,2,item);
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);//垂直滚动条按项移动

}

void MainWindow::on_temperature_region_clicked()
{
    scaledW=(float)src.cols/ui->src_img->width();
    scaledH=(float)src.rows/ui->src_img->height();
    Mat temperature_img;
    cvtColor(src,temperature_img,CV_BGR2GRAY);
    int min_tem,middle_tem,max_tem;
    QPoint p_start;
    QPoint p_end;
    if(ui->src_img->p1.rx()!=0)
    {
        if(ui->src_img->p1.x()<=ui->src_img->p2.x()){
            p_start.setX(scaledW*(ui->src_img->p1.x()));
            p_start.setY(scaledH*(ui->src_img->p1.y()));
            p_end.setX(scaledW*(ui->src_img->p2.x()));
            p_end.setY(scaledH*(ui->src_img->p2.y()));
        }else{
            p_start.setX(scaledW*(ui->src_img->p2.x()));
            p_start.setY(scaledH*(ui->src_img->p2.y()));
            p_end.setX(scaledW*(ui->src_img->p1.x()));
            p_end.setY(scaledH*(ui->src_img->p1.y()));
        }
        min_tem,middle_tem,max_tem=0;
        int sum=0;
        for(int p_index_x=p_start.x();p_index_x<=p_end.x();p_index_x++){
            for(int p_index_y=p_start.y();p_index_y<=p_end.y();p_index_y++){
                int temp_value=temperature_img.at<uchar>(p_index_x,p_index_y);
                if(temp_value>max_tem){
                    max_tem=temp_value;
                }
                if(temp_value<min_tem){
                    min_tem=temp_value;
                }
                sum+=temp_value;
            }
        }
        middle_tem=sum/((p_end.x()-p_start.x())*(p_end.y()-p_start.y()));
        int rowIndex = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowIndex);
        QTableWidgetItem *item ;
        item= new QTableWidgetItem(tr("%1").arg(min_tem));
        ui->tableWidget->setItem(rowIndex,0,item);
        item= new QTableWidgetItem(tr("%1").arg(middle_tem));
        ui->tableWidget->setItem(rowIndex,1,item);
        item= new QTableWidgetItem(tr("%1").arg(max_tem));
        ui->tableWidget->setItem(rowIndex,2,item);
    }

    //ui->tableWidget->setAutoScroll(true);
    // ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);//垂直滚动条按项移动
}
