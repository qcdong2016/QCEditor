#include "QC_GLWidget.h"
#include "QC_GLView.h"

#include <QDesktopWidget>
#include <QApplication>

USING_NS_CC;

static QCGLView* s_instance = nullptr;

QCGLView::QCGLView()
    : _screenScaleFactor(1.0f)
	, _lastWindowHeight(0.0f)
	, _bgColor(0, 0, 0, 1)
	, _box(nullptr)
{
}

QCGLView::~QCGLView()
{

}

bool QCGLView::isOpenGLReady()
{
    return true;
}

void QCGLView::end()
{
	delete s_instance;
    s_instance = nullptr;
}

void QCGLView::swapBuffers()
{
}

void QCGLView::setIMEKeyboardState(bool)
{
}

void QCGLView::setViewName(const char* pszViewName)
{
}

void QCGLView::setFrameSize(float width, float height)
{
	GLView::setFrameSize(width, height);
	setDesignResolutionSize(width, height, ResolutionPolicy::NO_BORDER);
	cocos2d::Director::getInstance()->setProjection(cocos2d::Director::getInstance()->getProjection());
	glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
}

void QCGLView::setBgColor(Color4B &color)
{
	_bgColor = Color4F((float)(color.r) / 255.f, (float)(color.g) / 255.f, (float)(color.b) / 255.f, (float)(color.a) / 255.f);
	glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
}

QCGLView* QCGLView::getInstance()
{
    if (s_instance == nullptr)
    {
        QCGLView *view = new QCGLView();
		s_instance = view;
    }

    return s_instance;
}

void QCGLView::init()
{
	bool ret = initGlew();
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

