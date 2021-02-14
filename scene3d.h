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

#include <QGLWidget>
#include <QDir>
#include <QApplication>
#include <QOpenGLExtraFunctions>
#include <QTimer>
#include <QtMath>

#include <QDebug>

struct TextureStruct
{
	unsigned int texture;
	unsigned char * data;
	int width;
	int height;
	int nrChannels;
};

class Scene3D : public QGLWidget, protected QOpenGLExtraFunctions
{
	Q_OBJECT

public:
	Scene3D(QWidget *parent = 0);
	~Scene3D();

	QSize sizeHint() const override;

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

	void closeEvent(QCloseEvent *event) override;
protected slots:
	
private:
	void compileShaders();

	QSurfaceFormat *mp_glSurface;
	float mp_vertices[N_VERTICES*(N_AXES+N_COLORARGS+N_TEXAXES)];
	unsigned int mp_indices[N_ELEMENTS*N_ELEMENTITEMS];
	const char *m_vertexShaderSource;
	const char *m_fragmentShaderSource;

	unsigned int m_vbo;
	unsigned int m_ebo;
	unsigned int m_vao;
	unsigned int m_shaderProgram;
	QTimer *mp_timer;
	float m_green;

	// unsigned int m_texture;
	// int m_width, m_height, m_nrChannels;
	QList<TextureStruct> m_textures;	// container of textureData
	// unsigned char * mp_textureData;
};

#endif	// SCENE3D_H