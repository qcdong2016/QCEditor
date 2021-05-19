#include "QC_GLWidget.h"
#include "qevent.h"
#include "qlogging.h"
#include "qtimer.h"
#include "SceneCtrl.h"

QCGLWidget::QCGLWidget(QWidget* parent /* = nullptr */)
: QOpenGLWidget(parent)
, _sceneCtrl(nullptr)
{
	this->setMouseTracking(true);
}

QCGLWidget::~QCGLWidget()
{
}

void QCGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    Vec2 pos = Director::getInstance()->convertToGL(Vec2(event->x(), event->y()));
	_sceneCtrl->mouseMoveEvent(pos.x, pos.y);
}

void QCGLWidget::mousePressEvent(QMouseEvent *event)
{
    Vec2 pos = Director::getInstance()->convertToGL(Vec2(event->x(), event->y()));
	_sceneCtrl->mousePressEvent(pos.x, pos.y);

}

void QCGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Vec2 pos = Director::getInstance()->convertToGL(Vec2(event->x(), event->y()));
	_sceneCtrl->mouseReleaseEvent(pos.x, pos.y);
}

void QCGLWidget::wheelEvent(QWheelEvent *event)
{
}

void QCGLWidget::keyPressEvent(QKeyEvent *event)
{
}

void QCGLWidget::keyReleaseEvent(QKeyEvent *event)
{
}

void QCGLWidget::initializeGL() {
    initGlew();
    auto width  = frameSize().width();
    auto height = frameSize().height();

    //glClearColor(0, 0, 0, 0);
    //this->makeCurrent();

//    m_devicePixelRatio = devicePixelRatio();

    auto director = cocos2d::Director::getInstance();
    auto glView   = director->getOpenGLView();
    
    if (!glView) {
        director->setOpenGLView(this);

        cocos2d::FileUtils::getInstance()->addSearchPath("Resources/");
        _timer = new QTimer(this);
        connect(_timer, &QTimer::timeout, this, [this]() {
            this->repaint();
        });
        _timer->start(std::chrono::milliseconds(10));
    }

    this->setFrameSize(width, height);
    this->setDesignResolutionSize(width, height, ResolutionPolicy::NO_BORDER);
    
    _sceneCtrl = SceneCtrl::create(frameSize().width(), frameSize().height());
    Director::getInstance()->runWithScene(_sceneCtrl);
    
    afterInit();
}

void QCGLWidget::paintGL() {
    cocos2d::Director::getInstance()->mainLoop();
}

void QCGLWidget::resizeGL(int width, int height) {
//    this->setRetinaFactor(devicePixelRatioF());
    this->setFrameSize(width, height);
    this->setDesignResolutionSize(width, height, ResolutionPolicy::NO_BORDER);

    cocos2d::Director::getInstance()->setViewport();
}
