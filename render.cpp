#include "render.h"
#include <QPainter>
#include <QWidget>
#include <QBitmap>
#include <QPaintEvent>

#define FULL_WIDTH QSize(1920,1080) //疑似フルスクリーン

//staticの実体を与える部分
QPoint Render::rects[4]={
    QPoint(-100,-100),
    QPoint(-100,100),
    QPoint(100,100),
    QPoint(100,-100)
};
QPoint Render::center(0,0);

QImage *Render::initImage;
QImage Render::tile;
QImage Render::tilePaint;

QColor Render::myPenColor(Qt::black);
int Render::myPenWidth(5);

Render::Render(){
    QImage *initImage=new QImage(FULL_WIDTH,QImage::Format_ARGB32_Premultiplied);
    initImage->fill(255);
    tile=*initImage;
}

Render::Render(QWidget *parent)
    :QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);

    rects[0]=QPoint(-100,-100);
    rects[1]=QPoint(-100,100);
    rects[2]=QPoint(100,100);
    rects[3]=QPoint(100,-100);

    modified=true;
}

void Render::resizeEvent(QResizeEvent *event){
    event->isAccepted();//エラー除去のための無意味なコード
    resizeDraw();
}

void Render::resizeDraw(){
    QPoint oldCenter=center;
    center.setX(this->size().width()/2);
    center.setY(this->size().height()/2);
    int i;
    for(i=0;i<=3;i++){
        if(oldCenter.x()!=center.x()){
            rects[i].setX(center.x()+(rects[i].x()-oldCenter.x()));
        }
        if(oldCenter.y()!=center.y()){
            rects[i].setY(center.y()+(rects[i].y()-oldCenter.y()));
        }
    }
}

void Render::setPenColor(const QColor &newColor)
{
    myPenColor= newColor;
}

void Render::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

