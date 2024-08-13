#include "Interface.h"
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QImage>
#include <QPixmap>

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
    calibrate_back = new QPushButton("Calibrate Background", this);

    calibrate_color->setGeometry(BUTTON_POS_X,BUTTON_POS_Y,BUTTON_WIDTH,BUTTON_HEIGHT);
    calibrate_back->setGeometry(BUTTON_POS_X,BUTTON_POS_Y + 100,BUTTON_WIDTH,BUTTON_HEIGHT);


    // Create the text streams
    state_text = new QLabel("Não calibrado", this);
    img1_text = new QLabel("Imagem real", this);
    img2_text = new QLabel("Imagem processada", this);
    img3_text = new QLabel("Imagem Final", this);
    img4_text = new QLabel("Quadro branco", this);
    cam_img = new QLabel("", this);
    pro_img = new QLabel("", this);
    out_img = new QLabel("", this);
    boa_img = new QLabel("", this);

    // Set the positions
    state_text->move(1000, 50);
    img1_text->move(100, 50);
    img2_text->move(600, 50);
    img3_text->move(100, 400);
    img4_text->move(600, 400);
    cam_img->move(120,50);
    pro_img->move(620,50);
    out_img->move(120,400);
    boa_img->move(620,400);
    

}


void Interface::setCamImg(QImage img){
    cam_img->setPixmap(QPixmap::fromImage(img));
}

void Interface::setProcessImg(QImage img){
    pro_img->setPixmap(QPixmap::fromImage(img));
}

void Interface::setOutPutImg(QImage img){
    out_img->setPixmap(QPixmap::fromImage(img));
}

void Interface::setBoardImg(QImage img){
    boa_img->setPixmap(QPixmap::fromImage(img));
}


