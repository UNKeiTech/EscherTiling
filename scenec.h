#ifndef SCENEC_H
#define SCENEC_H

#include <QApplication>


class SceneC : public QObject

{
    Q_OBJECT
public:
    SceneC();
    int scene() const{return myscene;}
    void sceneChange();

private:
    int myscene;
};

#endif // SCENEC_H
