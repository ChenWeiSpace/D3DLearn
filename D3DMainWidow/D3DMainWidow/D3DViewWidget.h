#ifndef D3DViewWidget_H
#define D3DViewWidget_H

#include <QWidget>

class D3DViewWidget : public QWidget
{
	Q_OBJECT

public:
	D3DViewWidget(QWidget* parent);
	~D3DViewWidget();
protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
private:
	QPoint m_perPos{ 0,0 };
};

#endif // !D3DViewWidget_H


