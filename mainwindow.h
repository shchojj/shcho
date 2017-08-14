#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    float scaledW;
    float scaledH;

private slots:
    void on_read_img_clicked();
    void on_checkBox_sharpen_clicked();

    void on_show_PrtSc_clicked();

    void on_CheckBox_negative_clicked();

    void on_spinBox_graydown_valueChanged(int arg1);

    void on_spinBox_grayup_valueChanged(int arg1);

    void on_temperature_clicked();

    void on_temperature_region_clicked();

public slots:
    void onRadioClickFilter();
    void onRadioClickEnhance();
    void onRadioClickColor();

private:
    Ui::MainWindow *ui;
    cv::Mat src;
    cv::Mat dst;
protected:
    void paintEvent(QPaintEvent *);
    void show_image(cv::Mat image);
    void check_select();
    void show_colormap(cv::Mat image);
};

#endif // MAINWINDOW_H
