#ifndef RENDER_H
#define RENDER_H

#include <QPainter>
#include <QWidget>
class QPaintEvent;
class QImage;
class QPainter;

class Render : public QWidget//RenderArea,同2,3,4を子に持つ。彼ら子どもたちはQWidgetの成分としてMainWindowのQStackedWindowに配列的な感じで入れられ、QButtonの押下によりシーンが切り替えされる毎に番号順で画面に表示される。
{
    Q_OBJECT

public:
    Render();
    Render(QWidget *parent);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }
    bool isModified() const { return modified; }

protected:
    void resizeEvent(QResizeEvent *event) override;
    void resizeDraw();

    static QColor myPenColor;
    static int myPenWidth;

    static QPoint center;
    static QPoint rects[];
    bool rectsDraged[4]={
        false,
        false,
        false,
        false
    };

    static QImage *initImage;
    static QImage tile;
    static QImage tilePaint;

private:
    bool resized=false;
    bool modified;
};

#endif // RENDERAREA_H
