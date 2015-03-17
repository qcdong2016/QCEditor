#include "QC_GLWidget.h"
#include "qevent.h"
#include "qlogging.h"
#include "QC_GLView.h"
#include "HelloWorldScene.h"
#include "qtimer.h"

QCGLWidget::QCGLWidget(QWidget* parent /* = nullptr */)
: QGLWidget(QGLFormat(QGL::DoubleBuffer), parent)
, _lastx(0)
, _lasty(0)
, _mousePressed(false)
, _isMouseHoveredBox(false)
, _timer(nullptr)
{
	this->setMouseTracking(true);

}

QCGLWidget::~QCGLWidget()
{
}

void QCGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QGLWidget::mouseMoveEvent(event);

	int x = event->x();
	int y = frameSize().height() - event->y();

	WindowBox* box = QCGLView::getInstance()->getBox();

	float pixelDeltaX = x - _lastx;
	float pixelDeltaY = y - _lasty;

	if (_lastx == x && _lasty == y)
		return;

	_lastx = x;
	_lasty = y;

	if (_isMouseHoveredBox) {
		box->updateWindowAreas(pixelDeltaX, pixelDeltaY, pixelDeltaX, pixelDeltaY);
		return;
	}

	if (_mousePressed)
	{


		if (_hoveredResizePoint != RESIZE_POINT_NONE)
		{
			// The mouse is pressed on a resize point; resize each selected window accordingly.
			switch (_hoveredResizePoint)
			{
			case RESIZE_POINT_WN:
				// Upper left
				box->updateWindowAreas(pixelDeltaX, pixelDeltaY, 0.0f, 0.0f);
				break;
			case RESIZE_POINT_N:
				// Upper middle
				box->updateWindowAreas(0.0f, pixelDeltaY, 0.0f, 0.0f);
				break;
			case RESIZE_POINT_NE:
				// Upper right
				box->updateWindowAreas(0.0f, pixelDeltaY, pixelDeltaX, 0.0f);
				break;
			case RESIZE_POINT_E:
				// Middle right
				box->updateWindowAreas(0.0f, 0.0f, pixelDeltaX, 0.0f);
				break;
			case RESIZE_POINT_ES:
				// Bottom right
				box->updateWindowAreas(0.0f, 0.0f, pixelDeltaX, pixelDeltaY);
				break;
			case RESIZE_POINT_S:
				// Bottom middle
				box->updateWindowAreas(0.0f, 0.0f, 0.0f, pixelDeltaY);
				break;
			case RESIZE_POINT_SW:
				// Bottom left
				box->updateWindowAreas(pixelDeltaX, 0.0f, 0.0f, pixelDeltaY);
				break;
			case RESIZE_POINT_W:
				// Middle left
				box->updateWindowAreas(pixelDeltaX, 0.0f, 0.0f, 0.0f);
				break;
			default:
				break;
			}
		}
	}
	else 
	{
		qDebug("!!");
		_hoveredResizePoint = box->GetPointAtPosition(x, y);

		if (_hoveredResizePoint != -1)
		{
			//todo: update mouse status;
		}
		
	}
}

void QCGLWidget::mousePressEvent(QMouseEvent *event)
{
    QGLWidget::mousePressEvent(event);
	_lasty = frameSize().height() - event->y();
	_lastx = event->x();

	WindowBox* box = QCGLView::getInstance()->getBox();

	if (box->isPointInBoxRect(_lastx, _lasty))
	{
		_isMouseHoveredBox = true;
		emit selectedBox();
	}
	else {
		_mousePressed = true;
	}
}

void QCGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QGLWidget::mouseReleaseEvent(event);
	_mousePressed = false;
	_isMouseHoveredBox = false;
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
	const QSize& sz = evnet->size();
	GLView* v = cocos2d::Director::getInstance()->getOpenGLView();
	if (v) {
		v->setFrameSize(frameSize().width(), frameSize().height());
		HelloWorld* hw = (HelloWorld*)Director::getInstance()->getRunningScene();
		hw->onResize(sz.width(), sz.height());
	}
}

void QCGLWidget::cocos2dDraw()
{
	makeCurrent();
	cocos2d::Director::getInstance()->mainLoop();
	swapBuffers();
}

void QCGLWidget::startCocos2d(int fps)
{
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(cocos2dDraw()));

	int msec = 1.0 / ((float)fps) * 1000.0;

	Director::getInstance()->setAnimationInterval(fps);
	Director::getInstance()->runWithScene(HelloWorld::create());

	_timer->start(msec);
}

