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
private:
	Ui::D3DMainWidowClass ui;   
};
