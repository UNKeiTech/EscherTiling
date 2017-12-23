#include "renderarea.h"
#include <QPainter>
#include <QWidget>
#include <QBitmap>
#include <QPaintEvent>

#define ENSIZE 15 //円のサイズでえんさいず
#define FULL_WIDTH QSize(1920,1080) //疑似フルスクリーン

RenderArea::RenderArea()
{
    rendering=false;
}
void RenderArea::paintEvent(QPaintEvent *event)
{
    event->isAccepted();//エラー除去のための無意味なコード

    QPainter painter(&tile);//painterはQImageたるtileに描画する用のQPainter
    tile.fill(255);//tile初期化
    painter.setPen(QPen(Qt::black,3,Qt::DotLine));
    painter.drawConvexPolygon(rects,4);//四角形の辺
    painter.setPen(QPen(Qt::black,1,Qt::SolidLine));
    painter.setBrush(Qt::black);
    int i;
    for(i=0;i<=3;i++){
       painter.drawEllipse(rects[i],ENSIZE,ENSIZE);//四角形の頂点
    }
    if (rendering==true){
        QPainter painterDash(this);
        painterDash.setPen(QPen(Qt::red,5));
        painterDash.drawEllipse(curpos.x()-25,curpos.y()-25,50,50);//超絶オシャンティー機能
       }
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    QPainter draw(this);//drawは画面（正確にはRenderArea）に描画する用のQPainter
    draw.drawImage(0,0,tile);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void RenderArea::mousePressEvent(QMouseEvent *event){
    if((event->button()==Qt::LeftButton)){
        rendering=true;
        curpos=event->pos();
}
    int i;
    for(i=0;i<=3;i++){
        if(rects[i].x()-ENSIZE<=event->pos().x()&&event->pos().x()<=rects[i].x()+ENSIZE){
            if(rects[i].y()-ENSIZE<=event->pos().y()&&event->pos().y()<=rects[i].y()+ENSIZE){
                rectsDraged[i]=true;
        }
    }
}

}
void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && rendering==true){
        curpos=event->pos();
    }
    int i;
    for(i=0;i<=3;i++){
        if(rectsDraged[i]){

            switch (i) {
            case 0:
                rects[2].setX(rects[2].x()-(event->pos().x()-rects[i].x()));
                rects[2].setY(rects[2].y()-(event->pos().y()-rects[i].y()));
                break;
            case 1:
                rects[3].setX(rects[3].x()-(event->pos().x()-rects[i].x()));
                rects[3].setY(rects[3].y()-(event->pos().y()-rects[i].y()));
                break;
            case 2:
                rects[0].setX(rects[0].x()-(event->pos().x()-rects[i].x()));
                rects[0].setY(rects[0].y()-(event->pos().y()-rects[i].y()));
                break;
            case 3:
                rects[1].setX(rects[1].x()-(event->pos().x()-rects[i].x()));
                rects[1].setY(rects[1].y()-(event->pos().y()-rects[i].y()));
                break;
            default:
                break;
            }
            rects[i].setX(event->pos().x());
            rects[i].setY(event->pos().y());
        }
    }
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button()==Qt::LeftButton)&&rendering==true){
        rendering=false;
        int i;
        for(i=0;i<=3;i++){
            rectsDraged[i]=false;
        }
    }
}
void RenderArea::resizeEvent(QResizeEvent *event){//resizeEventのオーバライド消すと原点が中央に来なくなるので注意
    event->isAccepted();//エラー除去のための無意味なコード
    Render::resizeDraw();
}
void RenderArea::sorting(){
    QPoint sortRect;
    //rects[0]とrects[2]さえソートできてしまえば1と3の区別要らない
    //そもそもソートなんてしなくてよいのかもしれない
    int i;
    for(i=1;i<=3;i++){
        if(rects[i].x()<=rects[0].x()&&rects[i].y()<=rects[0].y()){
            sortRect=rects[0];
            rects[0]=rects[i];
            rects[i]=sortRect;
        }
    }
    for(i=1;i<=3;i++){
        if(rects[2].x()<=rects[i].x()&&rects[2].y()<=rects[i].y()){
            sortRect=rects[2];
            rects[2]=rects[i];
            rects[i]=sortRect;
        }
     }
}

bool RenderArea::saveImage(const QString &fileName){
    return tile.save(fileName,0,-1);
}
