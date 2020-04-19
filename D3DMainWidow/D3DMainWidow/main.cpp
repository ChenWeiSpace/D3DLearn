#include "D3DMainWidow.h"
#include <QtWidgets/QApplication>
#include<QtCore/QDebug>
#include<QtCore/QDir>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	D3DMainWidow w;
	w.show();
	w.init();
	return a.exec();
}


std::string getResouseDir()
{
	static  std::string ss;
	if (ss.empty())
	{
		ss = QDir::currentPath().toLocal8Bit().data();
	}
	return ss;
}