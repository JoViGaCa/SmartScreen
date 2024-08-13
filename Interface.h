#ifndef INTERFACE_H
#define INTERCACE_H

#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QLabel>

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT  720
#define BUTTON_WIDTH   150
#define BUTTON_HEIGHT   50
#define BUTTON_POS_X  1000
#define BUTTON_POS_Y   100

class QPushButton;
class Interface: public QWidget{
    public:
        explicit Interface(QWidget *parent = nullptr);
        void setCamImg(QImage img);
        void setProcessImg(QImage img);
        void setOutPutImg(QImage img);
        void setBoardImg(QImage img);
    private slots:

    private:
        QPushButton * calibrate_color;
        QPushButton * calibrate_back;
        QLabel * state_text;
        QLabel * img1_text;
        QLabel * img2_text;
        QLabel * img3_text;
        QLabel * img4_text;
        QLabel * cam_img;
        QLabel * pro_img;
        QLabel * out_img;
        QLabel * boa_img;
        
};

#endif