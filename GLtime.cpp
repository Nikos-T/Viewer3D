#include "GLTime.h"

//#include <QObject>

class Scene3D;
GLTime::GLTime()
{
    mp_timer = new QTimer();
    mp_timer->setSingleShot(false);
    mp_timer->setInterval(1000.0/60.0);
    GLTime::fps = 60;


}
