#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle(tr("Viewer 3D"));

	mp_scene3D = new Scene3D(this);
	setCentralWidget(mp_scene3D);

	QDockWidget *dock = new QDockWidget(this);
	QTextEdit *log = new QTextEdit(dock);
	dock->setWidget(log);
	addDockWidget(Qt::BottomDockWidgetArea, dock);
}

MainWindow::~MainWindow()
{
}

