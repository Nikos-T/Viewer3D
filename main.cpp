#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
	QTranslator translator;
	translator.load(QString("Viewer3D_el_GR.qm"));


	QApplication a(argc, argv);

	//OpenGL
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);


	//Translator
	a.installTranslator(&translator);
	MainWindow w;
	w.show();
	return a.exec();
}
