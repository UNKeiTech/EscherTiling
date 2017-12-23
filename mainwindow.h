#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QApplication>
#include <render.h>
#include <renderarea.h>
#include <renderarea2.h>
#include <renderarea3.h>
#include <renderarea4.h>
#include <QtWidgets>
#include "scenec.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=0);
    ~MainWindow();

signals:
    sceneNum(int num);
    saveRen1(QString g);
    saveRen2(QString g);
    saveRen3(QString g);
    saveRen4(QString g);

protected:
    bool saveFile(const QByteArray &fileFormat);

private slots:
    void sceneChange();
    void changeWindowSize();
    void save();
    void penColor();
    void penWidth();

private:
    SceneC *scene;

    QStackedWidget *motherWidget;

    Render *render;

    QWidget *kingWidget;
    RenderArea *renderArea;
    QPushButton *button1;
    QVBoxLayout *layout;

    QWidget *queenWidget;
    RenderArea2 *renderArea2;
    QPushButton *buttonUndo2;
    QPushButton *buttonGoNext2;
    QVBoxLayout *layout2;

    QWidget *boyWidget;
    RenderArea3 *renderArea3;
    QPushButton *ButtonUndo3;
    QPushButton *buttonGoNext3;
    QVBoxLayout *layout3;

    QWidget *girlWidget;
    RenderArea4 *renderArea4;

    void createActionMenu();

    QMenu *saveAsMenu;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penWidthAct;

    QString fileName;

};

#endif // MAINWINDOW_H
