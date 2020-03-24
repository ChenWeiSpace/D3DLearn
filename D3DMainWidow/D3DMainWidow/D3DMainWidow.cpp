#include "D3DMainWidow.h"
#include "D3DEnvironment.h"
#include "cube/CubeObject.h"
#include "Triangle/TriangleObject.h"

D3DEnvironmentPtr s_event;
D3DMainWidow::D3DMainWidow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void D3DMainWidow::init()
{
	D3DEnvironmentPtr event = std::make_shared<D3DEnvironment>();
	int width = ui.centralWidget->width();
	int height = ui.centralWidget->height();
	WId id = ui.centralWidget->winId();
	event->buildDevice(id,width,height);
	//event->addVisualObject(std::make_shared<TriangleObject>());
	this->startTimer(10);
	s_event = event;
	m_time.start();
}

void D3DMainWidow::timerEvent(QTimerEvent *event)
{
	static std::uint64_t frameNumber = 0;
	std::uint64_t val = m_time.elapsed();
	if (m_current == 0)
	{
		m_current = val;
	}
	s_event->frameMove(frameNumber++, val - m_current);
}

void D3DMainWidow::resizeEvent(QResizeEvent *event)
{
	QMainWindow::resizeEvent(event);
	if (s_event)
	{
		s_event->resize(ui.centralWidget->width(), ui.centralWidget->height());
	}
}
