#pragma once

#include "cocos2d.h"
#include <QtOpenGL/QGLWidget>
#include "WindowBox.h"

class QTimer;
class SceneCtrl;

USING_NS_CC;

class QCGLWidget : public QGLWidget
{
    Q_OBJECT

public:
	QCGLWidget(QWidget* parent = nullptr);
    ~QCGLWidget();

	SceneCtrl* createCocos2dSceneCtrl();

public slots:
	void cocos2dDraw();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

	virtual void wheelEvent(QWheelEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

	virtual void paintEvent(QPaintEvent* event);
	virtual void resizeEvent(QResizeEvent* evnet);

	QTimer* _timer;
	SceneCtrl* _sceneCtrl;
};

