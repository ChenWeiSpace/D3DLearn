#include "D3DMainWidow.h"
#include "D3DEnvironment.h"
#include "cube/CubeObject.h"
#include "Triangle/TriangleObject.h"
#include "Rectangle/Rectangle.h"
#include "GlobalInstance.h"

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
	std::vector < AbstractVisualObjectPtr> objs = GlobalInstance::GetInstance()->getObjs();
	std::for_each(objs.begin(), objs.end(), [event](auto obj) {event->addVisualObject(obj); });
	//event->addVisualObject(std::make_shared<DrawRectangle>());

	ui.centralWidget->startTimer(10);
	s_event = event;
	ui.centralWidget->getTime().start();
}
