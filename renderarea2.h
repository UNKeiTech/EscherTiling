#ifndef RENDERAREA2_H
#define RENDERAREA2_H
#include <QWidget>
#include <QApplication>
#include "render.h"
#include <QImage>

class RenderArea2 : public Render
{
    Q_OBJECT
public:
    RenderArea2();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;//以下４つはQt公式のscribble exampleの改変
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawLineTo(const QPoint &endPoint);

signals:
    void endToDraw();//RenderArea3に進めるようになるフラグ

private slots:
    bool saveImage(const QString &fileName);//画像セーブ
    void undo();//描画やり直し処理
    void MakeAlphaTile();//RenderArea2の役目が終了したとき、tileの背景を透過させる

private:
    bool scribbling;//以下４つはscribe（ペイントソフト）部分で使う変数
    QPoint latestPoint;
    bool modified;
    QImage image;

    QImage imageSaveH;//以下２つ、書いてもらったフリーハンド線をHとVでそれぞれ一本ずつ把持。
    QImage imageSaveV;
    bool switchH;//横線、左上の点から右上の点への線
    bool switchV;//縦線、左上の点から左下への線
    QPoint startPoint;//縦なのか横なのか分からん時点でのstartPointHまたはVの値を保持
    QPoint startPointH;//以下４つ、書いてもらったフリーハンド線の始点と終点を把持。タイルの輪郭線を閉曲線にするのに使用
    QPoint lastPointH;
    QPoint startPointV;
    QPoint lastPointV;

    QImage imageSaveOnFile;//画像save用
};

#endif // RENDERAREA2_H
