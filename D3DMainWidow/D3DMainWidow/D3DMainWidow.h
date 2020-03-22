#pragma once

#include <QtWidgets/QMainWindow>
#include<QtCore/QTime>

#include "ui_D3DMainWidow.h"

class D3DMainWidow : public QMainWindow
{
	Q_OBJECT

public:
	D3DMainWidow(QWidget *parent = Q_NULLPTR);
	void init();
	void timerEvent(QTimerEvent *event);
protected:
	virtual void resizeEvent(QResizeEvent *event);
private:
	Ui::D3DMainWidowClass ui;
	QTime m_time;
	std::uint64_t m_current{0};
};
