#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle(tr("Viewer 3D"));

	mp_scene3D = new Scene3D(this);

	setCentralWidget(mp_scene3D);
}

MainWindow::~MainWindow()
{
}

