#include "renderarea3.h"

#include <QWidget>
#include <QBitmap>
#include <QMouseEvent>

RenderArea3::RenderArea3()
{
    rendering=false;

    image=tile;
    image.createMaskFromColor(255);
}

void RenderArea3::InitTile(){
    image=tile;
}

void RenderArea3::mousePressEvent(QMouseEvent *event)
{
    //マウスが押された時にラインの始点を取得
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
        rendering = true;
    }

}
void RenderArea3::mouseMoveEvent(QMouseEvent *event)
{
    //マウスが動いている間描画し続ける
    if((event->buttons() & Qt::LeftButton) && rendering){
        drawLine(event->pos());
    }
}
void RenderArea3::mouseReleaseEvent(QMouseEvent *event)
{
    //マウス左ボタンが離されると描画終了
    if(event->button() == Qt::LeftButton && rendering){
        drawLine(event->pos());
        rendering = false;
    }
}
void RenderArea3::paintEvent(QPaintEvent *evet)
{
    QPainter painter(this);
    painter.drawImage(0,0,tile);
    QRect dirtyRect = evet->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void RenderArea3::resizeEvent(QResizeEvent *event)
{
    if(width() > image.width() || height() > image.height()){
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image,QSize(newWidth,newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}
void RenderArea3::drawLine(const QPoint &endPoint)
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


void RenderArea3::resizeImage(QImage *image, const QSize &newSize)
{
    if(image->size() == newSize) return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0,0),*image);
    *image = newImage;
}

void RenderArea3::undo(){
    image.fill(255);
}

void RenderArea3::saveScribble(){
    QPixmap pix(this->size());
    this->render(&pix);
    pix.setMask(pix.createHeuristicMask());
    tilePaint=pix.toImage();
}

bool RenderArea3::saveImage(const QString &fileName){
     return image.save(fileName,0,-1);
}
