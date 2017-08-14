#ifndef REGIONALENHANCE_H
#define REGIONALENHANCE_H

#include <QDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
namespace Ui {
class RegionalEnhance;
}

class RegionalEnhance : public QDialog
{
    Q_OBJECT

public:
    explicit RegionalEnhance(cv::Mat src_Regional_image,QWidget *parent = 0);
    ~RegionalEnhance();
    void show_image(Mat image);
    void check_select();
private slots:

    void on_CheckBox_denoise_clicked();

    void on_CheckBox_Enhance_clicked();

    void on_CheckBox_sharpen_clicked();

    void on_DoubleSpinBox_Scale_valueChanged(double arg1);

    void on_CheckBox_negative_clicked();

    void on_CheckBox_pseudoColor_clicked();

    void on_spinBox_graydown_valueChanged(int arg1);

    void on_spinBox_grayup_valueChanged(int arg1);

private:
    Ui::RegionalEnhance *ui;
    cv::Mat Regional_image;
    cv::Mat dst;
};

#endif // REGIONALENHANCE_H
