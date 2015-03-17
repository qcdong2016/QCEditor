#include "QC_GLWidget.h"
#include "QC_GLView.h"
#include "qevent.h"
#include "qlogging.h"
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

	float pixelDeltaX = x - _lastx;
	float pixelDeltaY = y - _lasty;

	if (_lastx == x && _lasty == y)
		return;

	_lastx = x;
	_lasty = y;

	if (_isMouseHoveredBox) {
		qDebug("mm%f,%f", _lasty, _lastx);
		_boxesNode->updateWindowAreas(pixelDeltaX, pixelDeltaY, pixelDeltaX, pixelDeltaY);
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
				_boxesNode->updateWindowAreas(pixelDeltaX, pixelDeltaY, 0.0f, 0.0f);
				break;
			case RESIZE_POINT_N:
				// Upper middle
				_boxesNode->updateWindowAreas(0.0f, pixelDeltaY, 0.0f, 0.0f);
				break;
			case RESIZE_POINT_NE:
				// Upper right
				_boxesNode->updateWindowAreas(0.0f, pixelDeltaY, pixelDeltaX, 0.0f);
				break;
			case RESIZE_POINT_E:
				// Middle right
				_boxesNode->updateWindowAreas(0.0f, 0.0f, pixelDeltaX, 0.0f);
				break;
			case RESIZE_POINT_ES:
				// Bottom right
				_boxesNode->updateWindowAreas(0.0f, 0.0f, pixelDeltaX, pixelDeltaY);
				break;
			case RESIZE_POINT_S:
				// Bottom middle
				_boxesNode->updateWindowAreas(0.0f, 0.0f, 0.0f, pixelDeltaY);
				break;
			case RESIZE_POINT_SW:
				// Bottom left
				_boxesNode->updateWindowAreas(pixelDeltaX, 0.0f, 0.0f, pixelDeltaY);
				break;
			case RESIZE_POINT_W:
				// Middle left
				_boxesNode->updateWindowAreas(pixelDeltaX, 0.0f, 0.0f, 0.0f);
				break;
			default:
				break;
			}
		}
	}
	else 
	{
		_hoveredResizePoint = _boxesNode->getPointAtPosition(x, y);

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

	if (_boxesNode->isPointInBoxRect(_lastx, _lasty))
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

	GLView* v = cocos2d::Director::getInstance()->getOpenGLView();
	if (v)
		v->setFrameSize(frameSize().width(), frameSize().height());

	const Size& sz = _rootNode->getContentSize();
	_rootNode->setPosition(frameSize().width() / 2 - sz.width / 2, frameSize().height() / 2 - sz.height / 2);
	_boxesNode->Reset();
}

void QCGLWidget::cocos2dDraw()
{
	makeCurrent();
	cocos2d::Director::getInstance()->mainLoop();
	swapBuffers();
}

void QCGLWidget::startCocos2d()
{
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(cocos2dDraw()));

	Director::getInstance()->setAnimationInterval(60);

	_scene = Scene::create();

	_rootNode = Node::create();
	_rootNode->setContentSize(Size(400, 400));

	_scene->addChild(_rootNode);

	//remove me
	Sprite* sp = Sprite::create("HelloWorld.png");
	_rootNode->addChild(sp);

	_boxesNode = new WindowBox(sp, true);
	_boxesNode->autorelease();
	_boxesNode->setGlobalZOrder(99999);
	_boxesNode->setLocalZOrder(99999);
	_scene->addChild(_boxesNode);

	const Size& sz = _rootNode->getContentSize();
	_rootNode->setPosition(frameSize().width() / 2 - sz.width / 2, frameSize().height() / 2 - sz.height / 2);
	_boxesNode->Reset();

	Director::getInstance()->runWithScene(_scene);

	_timer->start(1);
}

