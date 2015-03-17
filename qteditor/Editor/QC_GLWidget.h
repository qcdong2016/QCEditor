#pragma once

#include "cocos2d.h"
#include <QtOpenGL/QGLWidget>

class QTimer;

USING_NS_CC;

class QCGLWidget : public QGLWidget
{
    Q_OBJECT

public:
	QCGLWidget(QWidget* parent = nullptr);
    ~QCGLWidget();

public slots:
	void cocos2dDraw();
	void startCocos2d(int fps);

signals:
	void selectedBox();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

	virtual void paintEvent(QPaintEvent* event);
	virtual void resizeEvent(QResizeEvent* evnet);


	int _hoveredResizePoint;
	bool _isMouseHoveredBox;
	bool _mousePressed;

	float _lastx;
	float _lasty;

	QTimer* _timer;
};

