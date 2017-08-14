#include "mypiclabel.h"
int myPicLabel::ROI_X=0;
int myPicLabel::ROI_Y=0;
int myPicLabel::ROI_Width=0;
int myPicLabel::ROI_Height=0;
myPicLabel::myPicLabel(QWidget *parent) :
    QLabel(parent)
{

}
void myPicLabel::mousePressEvent(QMouseEvent *e)
{
    this->setCursor(Qt::CrossCursor);
    if(e->button()==Qt::LeftButton)
    {
        p2=e->pos();
        p1=e->pos();
      // QMessageBox::information(0,tr("提示"),tr("LeftButtonDown"));
    }
    if(e->button()==Qt::RightButton)
    {
        p_right_start=e->pos();
        p_right_end=e->pos();
      // QMessageBox::information(0,tr("提示"),tr("LeftButtonDown"));
    }
    update();

}
void myPicLabel::mouseMoveEvent(QMouseEvent *e)
{

    if(e->buttons()&Qt::LeftButton)
    {
        p2=e->pos();
     //    QMessageBox::information(0,tr("提示"),tr("Move and LeftButtonDown"));
    }
//    update();
    if(e->buttons()&Qt::RightButton){
        p_right_end=e->pos();
    }else{
        /*QLabel *message;
        message->setText("");*/
    }
}
void myPicLabel::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);
    QPainter painter(this);
    painter.setPen(QColor(Qt::red));
    ROI_X=p1.x();
    ROI_Y=p1.y();
    ROI_Width=p2.x()-p1.x();
    ROI_Height=p2.y()-p1.y();
    painter.drawRect(ROI_X,ROI_Y,ROI_Width,ROI_Height);
    if(p_right_start==p_right_end){//划点
        QPen pen;
        pen.setWidth(6);
        pen.setColor(Qt::red); // 设置画笔为黄色
        painter.setPen(pen); // 设置画笔
        painter.drawPoint(p_right_start.x(),p_right_start.y());
        //QMessageBox::information(0,tr("提示"),tr("point"));
        }else{//划线
        //QMessageBox::information(0,tr("提示"),tr("line"));
        painter.drawLine(p_right_start,p_right_end);
        }
}
