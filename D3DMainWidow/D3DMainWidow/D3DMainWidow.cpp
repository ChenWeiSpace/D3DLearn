#include "D3DMainWidow.h"
#include "D3DEnvironment.h"
#include "cube/CubeObject.h"
#include "Triangle/TriangleObject.h"
#include "Rectangle/Rectangle.h"

D3DEnvironmentPtr s_event;
D3DMainWidow::D3DMainWidow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void D3DMainWidow::init()
{
	D3DEnvironmentPtr event = std::make_shared<D3DEnvironment>();
	ui.centralWidget->setEnvironment(event);
	int width = ui.centralWidget->width();
	int height = ui.centralWidget->height();
	WId id = ui.centralWidget->winId();
	event->buildDevice(id,width,height);
	event->addVisualObject(std::make_shared<CubeObject>());
	event->addVisualObject(std::make_shared<DrawRectangle>());

	ui.centralWidget->startTimer(10);
	s_event = event;
	ui.centralWidget->getTime().start();
}
