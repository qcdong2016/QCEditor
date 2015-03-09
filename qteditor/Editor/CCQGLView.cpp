#include "CCQGLWidget.h"
#include "CCQGLView.h"

#include <QDesktopWidget>
#include <QApplication>

USING_NS_CC;

static CCQGLView* s_instance = nullptr;

CCQGLView::CCQGLView()
    : _screenScaleFactor(1.0f)
	, _lastWindowHeight(0.0f)
	, _bgColor(0, 0, 0, 1)
{
}

CCQGLView::~CCQGLView()
{

}

bool CCQGLView::isOpenGLReady()
{
    return true;
}

void CCQGLView::end()
{
	delete s_instance;
    s_instance = nullptr;
}

void CCQGLView::swapBuffers()
{
}

void CCQGLView::setIMEKeyboardState(bool)
{
}

void CCQGLView::setViewName(const char* pszViewName)
{
}

void CCQGLView::setFrameSize(float width, float height)
{
	GLView::setFrameSize(width, height);
	setDesignResolutionSize(width, height, ResolutionPolicy::NO_BORDER);
	cocos2d::Director::getInstance()->setProjection(cocos2d::Director::getInstance()->getProjection());
	glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
}

void CCQGLView::setBgColor(Color4B &color)
{
	_bgColor = Color4F((float)(color.r) / 255.f, (float)(color.g) / 255.f, (float)(color.b) / 255.f, (float)(color.a) / 255.f);
	glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
}

CCQGLView* CCQGLView::getInstance()
{
    if (s_instance == nullptr)
    {
        CCQGLView *view = new CCQGLView();
		s_instance = view;
    }

    return s_instance;
}

void CCQGLView::init()
{
	bool ret = initGlew();
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

