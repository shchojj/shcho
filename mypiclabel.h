#ifndef MYPICLABEL_H
#define MYPICLABEL_H


#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QMessageBox>
#include <QPoint>
class myPicLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myPicLabel(QWidget *parent = 0);
     QPoint p1;
     QPoint p2;
     QPoint p_right_start;//划线的其实
     QPoint p_right_end;//划线的其实
     static int ROI_X;
     static int ROI_Y;
     static int ROI_Width;
     static int ROI_Height;
signals:

public slots:
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *);
};

#endif // MYPICLABEL_H
