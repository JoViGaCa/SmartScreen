#include "Interface.h"
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <iostream>
#include <sstream>


#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT  720
#define BUTTON_WIDTH   150
#define BUTTON_HEIGHT   50
#define BUTTON_POS_X  1000
#define BUTTON_POS_Y   100

Interface::Interface(QWidget *parent) :
    QWidget(parent){
    // Set size of the window
    setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);

    // Create and position the button
    calibrate_color = new QPushButton("Calibrate Color", this);
    remove_back = new QPushButton("Remove Background", this);

    calibrate_color->setGeometry(BUTTON_POS_X,BUTTON_POS_Y,BUTTON_WIDTH,BUTTON_HEIGHT);
    remove_back->setGeometry(BUTTON_POS_X,BUTTON_POS_Y + 100,BUTTON_WIDTH,BUTTON_HEIGHT);

    // Create the layout groub box
    layout_img = new QGroupBox(this);
    layout_btt = new QGroupBox(this);
    

    // Create the text streams
    calibrate_text = new QLabel("Não calibrado", this);
    remove_text = new QLabel("Com background", this);
    img1_text = new QLabel("Imagem real", this);
    img2_text = new QLabel("Imagem processada", this);
    img3_text = new QLabel("Imagem Final", this);
    img4_text = new QLabel("Quadro branco", this);
    cam_img = new QLabel(this);
    pro_img = new QLabel(this);
    out_img = new QLabel(this);
    boa_img = new QLabel(this);

    // add the img to the layout
    QGridLayout * layout_img_single = new QGridLayout;
    layout_img_single->addWidget(img1_text,0,0);
    layout_img_single->addWidget(cam_img,1,0);
    layout_img_single->addWidget(img2_text,0,1);
    layout_img_single->addWidget(pro_img,1,1);
    layout_img_single->addWidget(img3_text,2,0);
    layout_img_single->addWidget(out_img,3,0);
    layout_img_single->addWidget(img4_text,2,1);
    layout_img_single->addWidget(boa_img,3,1);

    // add the buttons to the layout
    QGridLayout * layout_btt_single = new QGridLayout;
    layout_btt_single->addWidget(calibrate_text);
    layout_btt_single->addWidget(remove_text);
    layout_btt_single->addWidget(calibrate_color);
    layout_btt_single->addWidget(remove_back);

    // configure layout
    layout_img->setLayout(layout_img_single);
    layout_img->move(0,0);
    layout_img->resize(1000,720);
    layout_btt->setLayout(layout_btt_single);
    layout_btt->move(1000,0);
    layout_btt->resize(280,360);

    // create the connection to buttons
    calibrate_color->setCheckable(true);
    remove_back->setCheckable(true);

    connect(calibrate_color, &QPushButton::clicked, this, &Interface::calibrateButtonClicked);
    connect(remove_back, &QPushButton::clicked, this, &Interface::removeButtonClicked);

}


void Interface::updateCamImg(QImage img){
    cam_img->setPixmap(QPixmap::fromImage(img));
}

void Interface::updateProcessImg(QImage img){
    pro_img->setPixmap(QPixmap::fromImage(img));
}

void Interface::updateOutPutImg(QImage img){
    out_img->setPixmap(QPixmap::fromImage(img));
}

void Interface::updateBoardImg(QImage img){
    boa_img->setPixmap(QPixmap::fromImage(img));
}

void Interface::calibrateButtonClicked(bool checked){
    if(checked){
        calibrate_text->setText("Calibração realizada");
        calibrate = true;
    }
}

void Interface::removeButtonClicked(bool checked){
    if(checked){
        remove_text->setText("Background removido");
        remove = true;
    }
}

bool Interface::getRemove(){
    return remove;
}

bool Interface::getCalibrate(){
    return calibrate;
}

void Interface::setRemove(bool value){
    remove = value;
}

void Interface::setCalibrate(bool value){
    calibrate = value;
}