#pragma once

#include "cocos2d.h"
#include <QtOpenGL/QGLWidget>

class QTimer;

USING_NS_CC;

class CCQGLWidget : public QGLWidget
{
    Q_OBJECT

public:
	CCQGLWidget();
    ~CCQGLWidget();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

	virtual void paintEvent(QPaintEvent* event);
	virtual void resizeEvent(QResizeEvent* evnet);

private:
};

