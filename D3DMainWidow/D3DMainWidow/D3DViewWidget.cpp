#include "D3DViewWidget.h"
#include<QtGui/QMouseEvent>
#include<QtCore/QDebug>

#include "D3DEnvironment.h"
#include "D3DCamera.h"

D3DViewWidget::D3DViewWidget(QWidget*parent)
	: QWidget(parent)
{
}

D3DViewWidget::~D3DViewWidget()
{

}

void D3DViewWidget::setEnvironment(D3DEnvironmentPtr envPtr)
{
	m_envirPtr = envPtr;
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
			QSize size = this->size();
			float valw = (float)disPos.x() / (float)size.width();
			float valh = (float)disPos.y() / (float)size.height();
			if (m_envirPtr)
			{
				EulerAngle eular = m_envirPtr->getCamera()->getRotation();
				eular.heading += valw * 180;
				eular.pitch += valh * 180;
				m_envirPtr->getCamera()->setRotation(eular);
			}
		}
		m_perPos = curPos;
	}
}

void D3DViewWidget::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	m_perPos = event->pos();
}

void D3DViewWidget::keyPressEvent(QKeyEvent* event)
{
	//QWidget::keyPressEvent(event);
	qDebug() << "keyPressEvent::" << event->key();

	if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
	{
		//×ó
		m_moveSatus = K_Left;
		m_pressTime = m_current;
	}
	else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
	{
		//ÓÒ
		m_moveSatus = K_Right;
		m_pressTime = m_current;
	}
	else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
	{
		//Ç°
		m_moveSatus = K_Back;
		m_pressTime = m_current;
	}
	else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
	{
		//ºó
		m_moveSatus = K_Back;
		m_pressTime = m_current;
	}
	event->accept();
}

void D3DViewWidget::keyReleaseEvent(QKeyEvent* event)
{
	//QWidget::keyReleaseEvent(event);
	qDebug() << "keyReleaseEvent::" << event->key();

	if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A 
		|| event->key() == Qt::Key_Right || event->key() == Qt::Key_D 
		|| event->key() == Qt::Key_Up || event->key() == Qt::Key_W 
		|| event->key() == Qt::Key_Down || event->key() == Qt::Key_S
		)
	{
		if (m_moveSatus != K_None)
		{
			m_moveSatus = K_None;
			//qDebug() << m_current - m_pressTime;
		}
	}
	event->accept();

}

void D3DViewWidget::timerEvent(QTimerEvent* event)
{
	
	static std::uint64_t frameNumber = 0;
	std::uint64_t val = m_time.elapsed();
	if (m_current == 0)
	{
		m_current = val;
	}
	m_envirPtr->frameMove(frameNumber++, val - m_current);
	m_current = val;
}

void D3DViewWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	if (m_envirPtr)
	{
		m_envirPtr->resize(this->width(), this->height());
	}
}
