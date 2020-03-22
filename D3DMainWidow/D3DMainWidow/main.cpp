#include "D3DMainWidow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	D3DMainWidow w;
	w.show();
	w.init();
	return a.exec();
}
