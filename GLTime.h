#ifndef GLTIME_H
#define GLTIME_H
#include <QTimer>
#include <QGLWidget>

class Scene3D;

class GLTime
{
public:
    GLTime();
    static int delta();
    static int getFPS();
    static int timeSinceStart();
    void setFPS(int fps);
    //~GLTime();
    int fps;
     QTimer *mp_timer;
private:



};

#endif // GLTIME_H
