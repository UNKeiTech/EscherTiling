#include "scenec.h"
#include <QApplication>

SceneC::SceneC()
{
    myscene=0;
}
void SceneC::sceneChange(){
    myscene++;
}
