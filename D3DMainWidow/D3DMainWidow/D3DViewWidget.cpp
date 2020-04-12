#include "D3DViewWidget.h"
#include<QtGui/QMouseEvent>
#include<QtCore/QDebug>

D3DViewWidget::D3DViewWidget(QWidget*parent)
	: QWidget(parent)
{
}

D3DViewWidget::~D3DViewWidget()
{

}

void D3DViewWidget::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);
	if (Qt::LeftButton & event->buttons())
	{
		QPoint curPos = event->pos();
		QPoint disPos = curPos - m_perPos;
		if (!(disPos).isNull())
		{
			
		}
		m_perPos = curPos;
	}
}

void D3DViewWidget::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	m_perPos = event->pos();
}
