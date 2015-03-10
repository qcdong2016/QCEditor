#include "CCQGLWidget.h"
#include "qevent.h"
#include "qlogging.h"
#include "CCQGLView.h"

CCQGLWidget::CCQGLWidget() 
: QGLWidget(QGLFormat(QGL::DoubleBuffer))
{

}

CCQGLWidget::~CCQGLWidget()
{
}
void CCQGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QGLWidget::mouseMoveEvent(event);

	int x = event->x();
	int y = frameSize().height()-event->y();

	Vec2 pos(x, y);

	int i = CCQGLView::getInstance()->getBox()->GetPointAtPosition(pos);

	qDebug("hello%d,%d,%d", i, x, y);
}

void CCQGLWidget::mousePressEvent(QMouseEvent *event)
{
    QGLWidget::mousePressEvent(event);
}

void CCQGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QGLWidget::mouseReleaseEvent(event);
}

void CCQGLWidget::wheelEvent(QWheelEvent *event)
{
	QGLWidget::wheelEvent(event);
}

void CCQGLWidget::keyPressEvent(QKeyEvent *event)
{
	QGLWidget::keyPressEvent(event);
}

void CCQGLWidget::keyReleaseEvent(QKeyEvent *event)
{
	QGLWidget::keyReleaseEvent(event);
}

void CCQGLWidget::paintEvent(QPaintEvent* event)
{
	QGLWidget::paintEvent(event);

	makeCurrent();
	cocos2d::Director::getInstance()->mainLoop();
	swapBuffers();
}

void CCQGLWidget::resizeEvent(QResizeEvent* evnet)
{
	QGLWidget::resizeEvent(evnet);
	const QSize& sz = evnet->size();
	GLView* v = cocos2d::Director::getInstance()->getOpenGLView();
	if (v)
		v->setFrameSize(frameSize().width(), frameSize().height());
}

