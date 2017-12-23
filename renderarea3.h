#ifndef RENDERAREA3_H
#define RENDERAREA3_H
#include <QWidget>
#include <QApplication>
#include "render.h"
#include <QImage>

class RenderArea3 : public Render
{
    Q_OBJECT
public:
    RenderArea3();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;//ほぼほぼscribe exampleそのまま利用
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawLine(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

private slots:
    void InitTile();//RenderArea3に移行したとき、今回の被描画対象であるQImage imageを直前までのtileで初期化
    void saveScribble();//RenderArea3の役目が終了したとき、お絵かきを画像として保存（背景を透過）
    bool saveImage(const QString &fileName);
    void undo();

private:
    bool rendering;
    bool scribbling;
    QPoint lastPoint;
    bool modified;

    QImage image;
};
#endif // RENDERAREA3_H
