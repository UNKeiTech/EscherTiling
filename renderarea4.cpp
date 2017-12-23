#include "renderarea4.h"

#include <QWidget>
#include <QBitmap>
#include <QMouseEvent>

#define ENSIZE 5

RenderArea4::RenderArea4()
{
    initialized=false;
    rendering=false;
    image=tile;
}

void RenderArea4::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    if(initialized){
        /*
        for(i=0;i<=100;i++){
            for(j=0;j<=100;j++){
                painter.drawImage(i*tileXL+j*tileXR,i*tileYL+j*tileYU,tile);
                painter.drawImage(-i*tileXL-j*tileXR,-i*tileYL-j*tileYU,tile);
                painter.drawImage(i*tileXL-j*tileXR,i*tileYL-j*tileYU,tile);
                painter.drawImage(-i*tileXL+j*tileXR,-i*tileYL+j*tileYU,tile);
                painter.drawImage(i*(-tileX),j*(-tileY),tile);
            }
        }
        */
        for(i=100;i>=0;i--){
           for(j=100;j>=0;j--){
               painter.drawImage(-i*tileXL-j*tileXR,-i*tileYL-j*tileYU,tile);
           }
        }
        for(i=0;i<=1000;i++){
           for(j=100;j>=0;j--){
               painter.drawImage(i*tileXL-j*tileXR,i*tileYL-j*tileYU,tile);
}
        }
        for(i=100;i>=0;i--){
           for(j=0;j<=1000;j++){
               painter.drawImage(-i*tileXL+j*tileXR,-i*tileYL+j*tileYU,tile);
           }
        }
        for(i=0;i<=1000;i++){
           for(j=0;j<=1000;j++){
               painter.drawImage(i*tileXL+j*tileXR,i*tileYL+j*tileYU,tile);
}
        }
        QRect dirtyRect = event->rect();
        painter.drawImage(dirtyRect, image, dirtyRect);
    }else{

        QImage imimi;
        imimi=tilePaint;
        QPainter pain(&imimi);
        pain.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        pain.drawImage(0,0,tile);
        tile=imimi;
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        initialized=true;
    }
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void RenderArea4::mousePressEvent(QMouseEvent *event)
{
    //マウスが押された時にラインの始点を取得
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
        rendering = true;
    }

}
void RenderArea4::mouseMoveEvent(QMouseEvent *event)
{
    //マウスが動いている間描画し続ける
    if((event->buttons() & Qt::LeftButton) && rendering){
        drawLine(event->pos());
    }
}
void RenderArea4::mouseReleaseEvent(QMouseEvent *event)
{
    //マウス左ボタンが離されると描画終了
    if(event->button() == Qt::LeftButton && rendering){
        drawLine(event->pos());
        rendering = false;
    }
}
void RenderArea4::drawLine(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint,endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update();
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;
}

void RenderArea4::getTileSize(){
    tileXL=rects[3].x()-rects[0].x();
    tileXR=rects[2].x()-rects[3].x();
    tileYU=rects[1].y()-rects[0].y();
    tileYL=rects[2].y()-rects[1].y();
}

bool RenderArea4::saveImage(const QString &fileName){
    QPixmap pix(this->size());
    this->render(&pix);
    return pix.save(fileName,0,-1);
}
