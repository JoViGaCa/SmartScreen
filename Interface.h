#ifndef INTERFACE_H
#define INTERCACE_H

#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <iostream>
#include <sstream>
#include <QGridLayout>

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT  720
#define BUTTON_WIDTH   150
#define BUTTON_HEIGHT   50
#define BUTTON_POS_X  1000
#define BUTTON_POS_Y   100

class QPushButton;
class Interface: public QWidget{
    //Q_OBJECT
    public:
        explicit Interface(QWidget *parent = nullptr);
        void updateCamImg(QImage img);
        void updateProcessImg(QImage img);
        void updateOutPutImg(QImage img);
        void updateBoardImg(QImage img);
        void setCalibrate(bool value);
        void setRemove(bool value);
        bool getCalibrate();
        bool getRemove();
    private slots:
        void calibrateButtonClicked(bool checked);
        void removeButtonClicked(bool checked);
    private:
        QGroupBox * layout_img;
        QGroupBox * layout_btt;
        QPushButton * calibrate_color;
        QPushButton * remove_back;
        QLabel * calibrate_text;
        QLabel * remove_text;
        QLabel * img1_text;
        QLabel * img2_text;
        QLabel * img3_text;
        QLabel * img4_text;
        QLabel * cam_img;
        QLabel * pro_img;
        QLabel * out_img;
        QLabel * boa_img;
        bool remove = false;
        bool calibrate = false;
        
};

#endif