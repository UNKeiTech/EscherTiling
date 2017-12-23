#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QPainter>
#include <QWidget>
#include <render.h>
class QPaintEvent;
class QImage;

class QPainter;

class RenderArea : public Render
{
    Q_OBJECT

public:
    RenderArea();

protected:
    void paintEvent(QPaintEvent *event) override;//描画には必須
    void mousePressEvent(QMouseEvent *event) override;//以下４つはQt公式のscribble exampleの改変
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void sorting();//頂点をソート（要らない気もする）
    bool saveImage(const QString &fileName);//画像セーブ

private:
    bool rendering;//お洒落な赤わく生成

    QPoint curpos;//現在のマウス位置
    bool rectsDraged[4]={//頂点がドラッグされているときtrue
        false,
        false,
        false,
        false
    };
};

#endif // RENDERAREA_H
