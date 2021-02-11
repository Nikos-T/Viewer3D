#ifndef SCENE3D_H
#define SCENE3D_H
#ifndef GLDEBUG
#define GLDEBUG
#endif
#ifndef NUMBERS
#define NUMBERS
#define N_AXES (3)
#define N_ELEMENTS (2)
#define N_ELEMENTITEMS (3)
#define N_VERTICES (4)
#define N_COLORARGS (3)
#define N_TEXAXES (2)
#endif
#include <QDebug>

#include <QGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTimer>
#include <QtMath>
#include "shaderprogram.h"
#include "texture.h"

class Scene3D : public QGLWidget, protected QOpenGLExtraFunctions
{
	Q_OBJECT

public:
	Scene3D(QWidget *parent = 0);
	~Scene3D();

	QSize sizeHint() const override;
    void paintGL() override;
protected:
	void initializeGL() override;

	void resizeGL(int w, int h) override;

	void closeEvent(QCloseEvent *event) override;
protected slots:
	
private:
	QSurfaceFormat *mp_glSurface;
	float mp_vertices[N_VERTICES*(N_AXES+N_COLORARGS+N_TEXAXES)];
	unsigned int mp_indices[N_ELEMENTS*N_ELEMENTITEMS];
	const char *m_vertexShaderSource;
	const char *m_fragmentShaderSource;

	unsigned int m_vbo;
	unsigned int m_ebo;
	unsigned int m_vao;
    ShaderProgram shader;
    QTimer *mp_timer;
    float deltaTime();
	float m_green;

    Texture *texture;
	unsigned char * mp_textureData;
};

#endif	// SCENE3D_H
