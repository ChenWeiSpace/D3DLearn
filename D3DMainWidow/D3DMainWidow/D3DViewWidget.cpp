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
	QWidget::keyPressEvent(event);
	if (event->isAutoRepeat())
	{
		return;
	}

	if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
	{
		//×ó
		m_moveSatus = K_Left;
		m_pressTime = m_current;
		m_perCameraPos = m_envirPtr->getCamera()->getWorldPosition();
	}
	else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
	{
		//ÓÒ
		m_moveSatus = K_Right;
		m_pressTime = m_current;
		m_perCameraPos = m_envirPtr->getCamera()->getWorldPosition();
	}
	else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
	{
		//Ç°
		m_moveSatus = K_Fornt;
		m_pressTime = m_current;
		m_perCameraPos = m_envirPtr->getCamera()->getWorldPosition();
	}
	else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
	{
		//ºó
		m_moveSatus = K_Back;
		m_pressTime = m_current;
		m_perCameraPos = m_envirPtr->getCamera()->getWorldPosition();
	}
	event->accept();
}

void D3DViewWidget::keyReleaseEvent(QKeyEvent* event)
{
	QWidget::keyReleaseEvent(event);
	if (event->isAutoRepeat())
	{
		return;
	}

	if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A 
		|| event->key() == Qt::Key_Right || event->key() == Qt::Key_D 
		|| event->key() == Qt::Key_Up || event->key() == Qt::Key_W 
		|| event->key() == Qt::Key_Down || event->key() == Qt::Key_S
		)
	{
		if (m_moveSatus != K_None)
		{
			m_moveSatus = K_None;
		}
	}
	event->accept();

}

void D3DViewWidget::timerEvent(QTimerEvent* event)
{
	if (m_moveSatus!= K_None)
	{
		int dir = 0;
		D3DXVECTOR3 off(0, 0, 0);
		switch (m_moveSatus)
		{
		case K_Right:
		{
			off = m_envirPtr->getCamera()->getRight();
		}
			break;
		case K_Left:
		{
			off = -m_envirPtr->getCamera()->getRight();
		}
			break;
		case K_Back:
		{
			off = -m_envirPtr->getCamera()->getHeading();
		}
			break;
		case K_Fornt:
		{
			off = m_envirPtr->getCamera()->getHeading();
		}
			break;
		}
		m_perCameraPos += off *(m_current - m_pressTime)*0.001;
		m_envirPtr->getCamera()->setWorldPosition(m_perCameraPos);
	}

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
