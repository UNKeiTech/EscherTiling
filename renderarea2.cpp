#include "renderarea2.h"

#include <QWidget>
#include <QBitmap>
#include <QMouseEvent>

#define ENSIZE 6

RenderArea2::RenderArea2()
{
switchH=true;
switchV=true;
scribbling=false;

image=tile;
image.createMaskFromColor(255);
}
void RenderArea2::paintEvent(QPaintEvent *event){
    QPainter painter(&tile);//painterはQImageたるtileに描画する用のQPainter
    tile.fill(255);

    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
    painter.drawImage(dirtyRect, imageSaveH, dirtyRect);//Horizontal
    painter.drawImage(QRect(dirtyRect.x()+(rects[1].x()-rects[0].x()),dirtyRect.y()+(rects[1].y()-rects[0].y()),dirtyRect.width(),dirtyRect.height()), imageSaveH, dirtyRect);//Horizontal
    painter.drawImage(dirtyRect, imageSaveV, dirtyRect);//Vertical
    painter.drawImage(QRect(dirtyRect.x()+(rects[3].x()-rects[0].x()),dirtyRect.y()+(rects[3].y()-rects[0].y()),dirtyRect.width(),dirtyRect.height()), imageSaveV, dirtyRect);//Vertical

    painter.setPen(QPen(Qt::black,5,Qt::DotLine));
    if(switchH){
        painter.drawLine(rects[0],rects[3]);
        painter.drawLine(rects[1],rects[2]);
        painter.setBrush(Qt::green);
       // painter.drawEllipse(rects[3],ENSIZE+5,ENSIZE+5);
    }
    if(switchV){
        painter.drawLine(rects[0],rects[1]);
        painter.drawLine(rects[2],rects[3]);
        painter.setBrush(Qt::green);
        painter.drawEllipse(rects[1],ENSIZE+4,ENSIZE+4);
    }
    if(switchH){//見栄えをよくするため描画順序変更
        painter.drawEllipse(rects[3],ENSIZE+4,ENSIZE+4);
    }
    if(switchH||switchV){
        painter.setBrush(Qt::red);
        painter.drawEllipse(rects[0],ENSIZE+4,ENSIZE+4);
    }
    if(!switchH&&!switchV){//閉曲面にするために
        painter.setPen(Qt::SolidLine);
        painter.setPen(myPenWidth);
        painter.setPen(myPenColor);
        painter.drawLine(startPointH,startPointV);
        painter.drawLine(lastPointH,startPointV+(rects[3]-rects[0]));
        painter.drawLine(startPointH+(rects[1]-rects[0]),lastPointV);
        painter.drawLine(lastPointH+(rects[1]-rects[0]),lastPointV+(rects[3]-rects[0]));
    }
    painter.setPen(Qt::DotLine);
//       painter.setBrush(Qt::green);

    QPainter draw(this);//画面にtile描画
    draw.drawImage(0,0,tile);
    imageSaveOnFile=tile;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void RenderArea2::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(switchH!=false||switchV!=false){
            if(rects[0].x()-ENSIZE<=event->pos().x()&&event->pos().x()<=rects[0].x()+ENSIZE){
                if(rects[0].y()-ENSIZE<=event->pos().y()&&event->pos().y()<=rects[0].y()+ENSIZE){
        latestPoint = event->pos();
        startPoint=latestPoint;
        scribbling = true;
                }}}
    }
}

void RenderArea2::mouseMoveEvent(QMouseEvent *event)
{
    if (scribbling){
        drawLineTo(event->pos());
    }
    if(switchH!=false&&scribbling){
        if(rects[3].x()-ENSIZE<=event->pos().x()&&event->pos().x()<=rects[3].x()+ENSIZE){
            if(rects[3].y()-ENSIZE<=event->pos().y()&&event->pos().y()<=rects[3].y()+ENSIZE){
                drawLineTo(event->pos());
                scribbling = false;
                switchH=false;
                startPointH=startPoint;
                lastPointH=event->pos();
                if(!switchV){
                    emit endToDraw();
                }
                imageSaveH=image;
                image.fill(255);
                update();
            }
        }
    }
    if(switchV!=false&&scribbling){
        if(rects[1].x()-ENSIZE<=event->pos().x()&&event->pos().x()<=rects[1].x()+ENSIZE){
            if(rects[1].y()-ENSIZE<=event->pos().y()&&event->pos().y()<=rects[1].y()+ENSIZE){
                drawLineTo(event->pos());
                scribbling = false;
                switchV=false;
                if(!switchH){
                    emit endToDraw();
                }
                imageSaveV=image;
                image.fill(255);
                update();
                startPointV=startPoint;
                lastPointV=event->pos();
            }
        }
    }
}

void RenderArea2::mouseReleaseEvent(QMouseEvent *event)
{
    //このイベントにたどり着いたということはmouseMoveEventで終了しなかったということなので、imageで書いた画像を消去してやり直させる
    if (event->button() == Qt::LeftButton && scribbling) {
        scribbling = false;
        image.fill(255);
    }
}
void RenderArea2::resizeEvent(QResizeEvent *event){
    event->isAccepted();//エラー除去のための無意味なコード
    Render::resizeDraw();
}
void RenderArea2::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(latestPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(latestPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    latestPoint = endPoint;
}
void RenderArea2::undo(){
    image.fill(255);
    imageSaveV.fill(255);
    imageSaveH.fill(255);
    switchH=true;
    switchV=true;
}

void RenderArea2::MakeAlphaTile(){//透過処理
    QPixmap pix(this->size());
    this->render(&pix);
    pix.setMask(pix.createHeuristicMask());
    QImage tileDash=pix.toImage();
    tile.fill(255);
    QPainter painter(&tile);
    painter.drawImage(0,0,tileDash);
}

bool RenderArea2::saveImage(const QString &fileName){
    return imageSaveOnFile.save(fileName,0,-1);
}
