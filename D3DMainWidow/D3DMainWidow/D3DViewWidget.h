#ifndef D3DViewWidget_H
#define D3DViewWidget_H

#include <QWidget>
#include<QTime>
#include <memory>

#include <d3dx9.h>
typedef std::shared_ptr<class D3DEnvironment> D3DEnvironmentPtr;

enum CameraMoveStatus
{
	K_None,
	K_Right,
	K_Left,
	K_Back,
	K_Fornt
};

class D3DViewWidget : public QWidget
{
	Q_OBJECT

public:
	D3DViewWidget(QWidget* parent);
	~D3DViewWidget();
	void setEnvironment(D3DEnvironmentPtr envPtr);
	inline  QTime &  getTime() {
		return m_time;
	}
protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	void timerEvent(QTimerEvent* event);
	virtual void resizeEvent(QResizeEvent* event);
private:
	QPoint m_perPos{ 0,0 };
	D3DEnvironmentPtr m_envirPtr;
	CameraMoveStatus m_moveSatus{ K_None };
	std::uint64_t m_current{ 0 };
	QTime m_time;
	std::uint64_t m_pressTime{ 0 };
	D3DXVECTOR3 m_perCameraPos{0,0,0};
};

#endif // !D3DViewWidget_H


