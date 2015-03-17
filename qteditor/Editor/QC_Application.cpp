#include "QC_Application.h"
#include "QC_GLView.h"

#include <QTimer>
#include "QC_GLWidget.h"
#include "EditorMain.h"
#include "HelloWorldScene.h"


USING_NS_CC;

/* sharedApplication pointer */
ACApplication * ACApplication::sm_pSharedApplication = nullptr;

ACApplication::ACApplication(int argc, char *argv[])
    : QApplication(argc, argv)
{
    _qAnimationInterval = 1.0f / 60.0f * 1000.0f;
    CC_ASSERT(! sm_pSharedApplication);
    sm_pSharedApplication = this;
}

ACApplication::~ACApplication()
{
    CC_ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = nullptr;
}

int ACApplication::run()
{
	_glwindow = new QCGLWidget(&_mainwindow);

	_glwindow->setWindowFlags(_glwindow->windowFlags() & ~Qt::WindowMaximizeButtonHint);
	_glwindow->setMinimumSize(500, 500);

	_mainwindow.setCentralWidget(_glwindow);
	_mainwindow.show();

	applicationDidFinishLaunching();

    return ACApplication::getInstance()->exec();
}

void ACApplication::setAnimationInterval(double interval)
{
}

ACApplication* ACApplication::getInstance()
{
    CC_ASSERT(sm_pSharedApplication);
    return sm_pSharedApplication;
}

bool ACApplication::applicationDidFinishLaunching()
{
	_glwindow->makeCurrent();
	QCGLView* view = QCGLView::getInstance();
	view->init();
	view->setBgColor(Color4B(50, 50, 50, 255));

	auto director = Director::getInstance();

	director->setOpenGLView(view);
	view->setFrameSize(_glwindow->frameSize().width(), _glwindow->frameSize().height());

	_glwindow->startCocos2d(30);

	connect(_glwindow, SIGNAL(selectedBox()), &_mainwindow, SLOT(viewBoxAttr()));

	return true;
}

