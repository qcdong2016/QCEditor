#include "CCQGLWidget.h"
#include "CCQGLView.h"

#include <QDesktopWidget>
#include <QApplication>

USING_NS_CC;

static CCQGLView* s_instance = nullptr;

// static void mouseMove(QMouseEvent *event)
// {
//     s_instance->mouseMove(event);
// }
// 
// static void mousePress(QMouseEvent *event)
// {
//     s_instance->mousePress(event);
// }
// 
// static void mouseRelease(QMouseEvent *event)
// {
//     s_instance->mouseRelease(event);
// }
// 
// static void wheel(QWheelEvent *event)
// {
// 	s_instance->wheel(event);
// }

CCQGLView::CCQGLView()
    : _screenScaleFactor(1.0f)
	, _lastWindowHeight(0.0f)
	, _bgColor(0, 0, 0, 1)
{
	//setViewName("cocos2d-x QT Host Win32");
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
   // CC_SAFE_DELETE(_window);
	delete s_instance;
    s_instance = nullptr;
}

void CCQGLView::swapBuffers()
{
//     if (_isInit)
//     {
//         /* Swap buffers */
// 
//     }
}

void CCQGLView::setIMEKeyboardState(bool /*bOpen*/)
{
	/* Open or close IME keyboard */
}

void CCQGLView::setViewName(const char* pszViewName)
{
	//GLViewImpl::setViewName(pszViewName);
	//_window->setWindowTitle(getViewName().c_str());
}

void CCQGLView::setFrameSize(float width, float height)
{
	GLView::setFrameSize(width, height);
	setDesignResolutionSize(width, height, ResolutionPolicy::NO_BORDER);
	//glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
}

void CCQGLView::setBgColor(Color4B &color)
{
	_bgColor = Color4F((float)(color.r) / 255.f, (float)(color.g) / 255.f, (float)(color.b) / 255.f, (float)(color.a) / 255.f);
	//glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
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

// void CCQGLView::mouseMove(QMouseEvent *event)
// {
//     if (!_captured)
//         return;
// 
// 	_mouseX = (float)(event->x()) / _screenScaleFactor;
// 	_mouseY = (float)(event->y()) / _screenScaleFactor;
// 
// 	_mouseX /= this->getFrameZoomFactor();
// 	_mouseY /= this->getFrameZoomFactor();
// 
// 	if (_isInRetinaMonitor)
// 	{
// 		if (_retinaFactor == 1)
// 		{
// 			_mouseX *= 2;
// 			_mouseY *= 2;
// 		}
// 	}
// 
// 	if (_captured)
// 	{
// 		intptr_t id = 0;
// 		this->handleTouchesMove(1, &id, &_mouseX, &_mouseY);
// 	}
// 
// 	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
// 	float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
// 	float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;
// 
// 	EventMouse ccEvent(EventMouse::MouseEventType::MOUSE_MOVE);
// 	// Set current button
// 	if (event->button() == Qt::LeftButton)
// 	{
// 		ccEvent.setMouseButton(GLFW_MOUSE_BUTTON_LEFT);
// 	}
// 	else if (event->button() == Qt::RightButton)
// 	{
// 		ccEvent.setMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
// 	}
// 	else if (event->button() == Qt::MiddleButton)
// 	{
// 		ccEvent.setMouseButton(GLFW_MOUSE_BUTTON_MIDDLE);
// 	}
// 	ccEvent.setCursorPosition(cursorX, cursorY);
// 	Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
// }
// 
// void CCQGLView::mousePress(QMouseEvent *event)
// {
//     if (event->button() != Qt::LeftButton)
//         return;
// 
//     _captured = true;
// 	_mouseX = (float)(event->x()) / _screenScaleFactor;
// 	_mouseY = (float)(event->y()) / _screenScaleFactor;
// 
// 	if (this->getViewPortRect().equals(Rect::ZERO) || this->getViewPortRect().containsPoint(Vec2(_mouseX, _mouseY)))
// 	{
// 		intptr_t id = 0;
// 		this->handleTouchesBegin(1, &id, &_mouseX, &_mouseY);
// 	}
// 
// 	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
// 	float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
// 	float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;
// 
// 	EventMouse ccEvent(EventMouse::MouseEventType::MOUSE_DOWN);
// 	ccEvent.setCursorPosition(cursorX, cursorY);
// 	ccEvent.setMouseButton(event->button());
// 	Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
// }
// 
// void CCQGLView::mouseRelease(QMouseEvent *event)
// {
//     if (event->button() != Qt::LeftButton)
//         return;
// 
// 	if (_captured)
// 	{
// 		_captured = false;
// 		intptr_t id = 0;
// 		this->handleTouchesEnd(1, &id, &_mouseX, &_mouseY);
// 	}
// 
// 	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
// 	float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
// 	float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;
// 
// 	EventMouse ccEvent(EventMouse::MouseEventType::MOUSE_UP);
// 	ccEvent.setCursorPosition(cursorX, cursorY);
// 	ccEvent.setMouseButton(event->button());
// 	Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
// }
// 
// void CCQGLView::wheel(QWheelEvent *event)
// {
// 	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
// 	float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
// 	float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;
// 
// 	EventMouse ccEvent(EventMouse::MouseEventType::MOUSE_SCROLL);
// 	ccEvent.setScrollData((float)event->x(), -(float)event->y());
// 	ccEvent.setCursorPosition(cursorX, cursorY);
// 	Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
// }


void CCQGLView::init()
{
	bool ret = initGlew();
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

