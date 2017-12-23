#ifndef RENDERAREA4_H
#define RENDERAREA4_H
#include <QWidget>
#include <QApplication>
#include "render.h"
#include <QImage>

class RenderArea4 : public Render
{
    Q_OBJECT

public:
    RenderArea4();

protected:
    void paintEvent(QPaintEvent *event)override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawLine(const QPoint &endPoint);

private slots:
    bool saveImage(const QString &fileName/*,const char *fileFormat*/);
    void getTileSize();

private:
    bool rendering;
    QPoint lastPoint;
    bool modified;

    QImage image;
    bool initialized;
    int i;
    int j;
    int tileXL;//left
    int tileXR;//right
    int tileYU;//upper
    int tileYL;//lower

    QImage imimi;
};

#endif // RENDERAREA4_H
