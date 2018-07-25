#include "QC_GLWidget.h"
#include "QC_GLView.h"
#include "qevent.h"
#include "qlogging.h"
#include "qtimer.h"
#include "SceneCtrl.h"

QCGLWidget::QCGLWidget(QWidget* parent /* = nullptr */)
: QGLWidget(QGLFormat(QGL::DoubleBuffer), parent)
, _sceneCtrl(nullptr)
{
	this->setMouseTracking(true);

}

QCGLWidget::~QCGLWidget()
{
}

void QCGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QGLWidget::mouseMoveEvent(event);

	_sceneCtrl->mouseMoveEvent(event->x(), frameSize().height() - event->y());
}

void QCGLWidget::mousePressEvent(QMouseEvent *event)
{
    QGLWidget::mousePressEvent(event);
	_sceneCtrl->mousePressEvent(event->x(), frameSize().height() - event->y());

}

void QCGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QGLWidget::mouseReleaseEvent(event);
	_sceneCtrl->mouseReleaseEvent(event->x(), frameSize().height() - event->y());
}

void QCGLWidget::wheelEvent(QWheelEvent *event)
{
	QGLWidget::wheelEvent(event);
}

void QCGLWidget::keyPressEvent(QKeyEvent *event)
{
	QGLWidget::keyPressEvent(event);
}

void QCGLWidget::keyReleaseEvent(QKeyEvent *event)
{
	QGLWidget::keyReleaseEvent(event);
}

void QCGLWidget::paintEvent(QPaintEvent* event)
{
	QGLWidget::paintEvent(event);
}

void QCGLWidget::resizeEvent(QResizeEvent* evnet)
{
	QGLWidget::resizeEvent(evnet);

	GLView* v = cocos2d::Director::getInstance()->getOpenGLView();
	if (v)
		v->setFrameSize(frameSize().width(), frameSize().height());

    if (_sceneCtrl)
	    _sceneCtrl->onResize(frameSize().width(), frameSize().height());
}

void QCGLWidget::cocos2dDraw()
{
	makeCurrent();
	cocos2d::Director::getInstance()->mainLoop();
	swapBuffers();
}

SceneCtrl* QCGLWidget::createCocos2dSceneCtrl()
{
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(cocos2dDraw()));

	Director::getInstance()->setAnimationInterval(60);

	_sceneCtrl = SceneCtrl::create(frameSize().width(), frameSize().height());
	Director::getInstance()->runWithScene(_sceneCtrl);

	_timer->start(1);

	return _sceneCtrl;
}

