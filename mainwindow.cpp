#include "mainwindow.h"
#include <render.h>
#include <QtWidgets>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include <QResizeEvent>
#include <QLayoutItem>
#include <QStackedWidget>

#define FULL_WIDTH QSize(1920,1080) //疑似フルスクリーン
#define SMALLEST_WIDTH QSize(20,10)

MainWindow::MainWindow(QWidget *parent) //:
: QMainWindow(parent)
{
    scene=new SceneC();

    fileName="untitled";

    createActionMenu();

    setWindowTitle("Regular Division of the Plane");
    resize(700,700);
    this->setMaximumSize(FULL_WIDTH);
    this->setMinimumSize(SMALLEST_WIDTH);

    QStackedWidget *motherWidget=new QStackedWidget;
    QWidget *kingWidget=new QWidget;
    QWidget *queenWidget=new QWidget;
    QWidget *boyWidget=new QWidget;
    QWidget *girlWidget=new QWidget;
    motherWidget->addWidget(kingWidget);
    motherWidget->addWidget(queenWidget);
    motherWidget->addWidget(boyWidget);
    motherWidget->addWidget(girlWidget);
    motherWidget->setCurrentIndex(0);//0からスタート。
    setCentralWidget(motherWidget);

    Render *render = new Render();
    render->hide();

    RenderArea *renderArea = new RenderArea();
    QPushButton *button1 = new QPushButton(QString(QObject::tr("Next")),this);
    QVBoxLayout *layout=new QVBoxLayout();//
    layout->addWidget(renderArea);
    layout->addWidget(button1);
    kingWidget->setLayout(layout);

    QVBoxLayout *layout2=new QVBoxLayout();
    renderArea2=new RenderArea2();
    QPushButton *buttonUndo2 = new QPushButton(QString(QObject::tr("UnDo")),this);
    QPushButton *buttonGoNext2 = new QPushButton(QString(QObject::tr("Next")),this);
    buttonGoNext2->hide();
    layout2->addWidget(renderArea2);
    layout2->addWidget(buttonUndo2);
    layout2->addWidget(buttonGoNext2);
    queenWidget->setLayout(layout2);

    QVBoxLayout *layout3=new QVBoxLayout();
    renderArea3=new RenderArea3();
    QPushButton *buttonUndo3 = new QPushButton(QString(QObject::tr("Delete Decoration")),this);
    QPushButton *buttonGoNext3 = new QPushButton(QString(QObject::tr("Finish Decoration!")),this);
    layout3->addWidget(renderArea3);
    layout3->addWidget(buttonUndo3);
    layout3->addWidget(buttonGoNext3);
    boyWidget->setLayout(layout3);

    QVBoxLayout *layout4=new QVBoxLayout;
    renderArea4=new RenderArea4;
    layout4->addWidget(renderArea4);
    girlWidget->setLayout(layout4);

     connect(this,SIGNAL(sceneNum(int)),motherWidget,SLOT(setCurrentIndex(int)));

     connect(button1,SIGNAL(clicked()),this,SLOT(sceneChange()));
     connect(button1,SIGNAL(clicked()),renderArea,SLOT(sorting()));

     connect(buttonUndo2,SIGNAL(clicked()),renderArea2,SLOT(undo()));
     connect(buttonUndo2,SIGNAL(clicked()),buttonGoNext2,SLOT(hide()));
     connect(renderArea2,SIGNAL(endToDraw()),buttonGoNext2,SLOT(show()));
     connect(buttonGoNext2,SIGNAL(clicked()),this,SLOT(sceneChange()));
     connect(buttonGoNext2,SIGNAL(clicked()),renderArea2,SLOT(MakeAlphaTile()));
     connect(buttonGoNext2,SIGNAL(clicked()),renderArea3,SLOT(InitTile()));

     connect(buttonUndo3,SIGNAL(clicked()),renderArea3,SLOT(undo()));
     connect(buttonGoNext3,SIGNAL(clicked()),this,SLOT(sceneChange()));
     connect(buttonGoNext3,SIGNAL(clicked()),renderArea3,SLOT(saveScribble()));
     connect(buttonGoNext3,SIGNAL(clicked()),renderArea4,SLOT(getTileSize()));
     connect(buttonGoNext3,SIGNAL(clicked()),this,SLOT(changeWindowSize()));
     connect(buttonGoNext3,SIGNAL(clicked()),this,SLOT(showMaximized()));

     connect(this,SIGNAL(saveRen1(QString)),renderArea,SLOT(saveImage(QString)));
     connect(this,SIGNAL(saveRen2(QString)),renderArea2,SLOT(saveImage(QString)));
     connect(this,SIGNAL(saveRen3(QString)),renderArea3,SLOT(saveImage(QString)));
     connect(this,SIGNAL(saveRen4(QString)),renderArea4,SLOT(saveImage(QString)));
}
MainWindow::~MainWindow()
{
}

void MainWindow::sceneChange(){
    scene->sceneChange();
    emit sceneNum(scene->scene());
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(render->penColor());
    if(newColor.isValid()){
        render->setPenColor(newColor);
    }
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this,tr(" Rendering: "),tr("Select pen Width:"),render->penWidth(),1,50,1,&ok);


    if(ok) {
        render->setPenWidth(newWidth);
    }
}

void MainWindow::createActionMenu()
{

    foreach(QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action,SIGNAL(triggered()),this,SLOT(save()));
        saveAsActs.append(action);
    }

    saveAsMenu = new QMenu(tr("&Save"),this);
    foreach(QAction *action, saveAsActs)
    saveAsMenu->addAction(action);

    exitAct = new QAction(tr("&Exit"),this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));

    penColorAct = new QAction(tr("Pen_&Color"),this);
    connect(penColorAct,SIGNAL(triggered()),this,SLOT(penColor()));

    penWidthAct = new QAction(tr("Pen_&Width"),this);
    connect(penWidthAct,SIGNAL(triggered()),this,SLOT(penWidth()));

    menuBar()->addMenu(saveAsMenu);
    menuBar()->addAction(penColorAct);
    menuBar()->addAction(penWidthAct);
    menuBar()->addSeparator();
    menuBar()->addAction(exitAct);
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        switch (scene->scene()) {
        case 0:{
            emit saveRen1(fileName);
            break;}
        case 1:
            emit saveRen2(fileName);
            break;
        case 2:
            emit saveRen3(fileName);
        break;
        case 3:
            emit saveRen4(fileName);
        break;
        }
    return true;
    }
}

void MainWindow::changeWindowSize(){
this->setFixedSize(FULL_WIDTH);
}
