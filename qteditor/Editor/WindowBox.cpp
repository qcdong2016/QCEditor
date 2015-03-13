#include "WindowBox.h"
#include "2d/CCDrawingPrimitives.h"
#include "base/CCDirector.h"

USING_NS_CC;

WindowBox::WindowBox(cocos2d::Node* aWindow, bool aResizable/* = true*/) :
_boxedWindow(aWindow),
_resizable(aResizable),
_locked(false)
{
    Reset ();
}

bool WindowBox::operator==(WindowBox& aBox)
{
	return (_boxedWindow == aBox._boxedWindow);
}

void WindowBox::Reset () 
{
	const Size& size = _boxedWindow->getContentSize();
	const int height = size.height;
	const int width = size.width;

	_resizePoints.clear();

	_resizePoints.push_back(Rect(0, 0, 6, 6));
	_resizePoints.push_back(Rect(width / 2, 0, 6, 6));
	_resizePoints.push_back(Rect(width, 0, 6, 6));
	_resizePoints.push_back(Rect(width, -height / 2, 6, 6));
	_resizePoints.push_back(Rect(width, -height, 6, 6));
	_resizePoints.push_back(Rect(width / 2, -height, 6, 6));
	_resizePoints.push_back(Rect(0, -height, 6, 6));
	_resizePoints.push_back(Rect(0, -height / 2, 6, 6));

	const Vec2 offset(3, 3 - height);

	for (size_t i = 0; i < _resizePoints.size(); i++) {
		_resizePoints[i].origin -= offset;
	}
}

//------------------------------------------------------------------------
int WindowBox::GetPointAtPosition (const cocos2d::Point& aPoint) const
{
    // If the window is locked or not resizable, we just cheat the Document
    if (_locked || !_resizable)
    {
        return RESIZE_POINT_NONE;
    }
    for (int i = 0; i < NUM_RESIZE_POINTS; ++i) 
    {
        if (_resizePoints [i].containsPoint (aPoint))
        {
            return i ;
        }    
    }
    return RESIZE_POINT_NONE;
}

//------------------------------------------------------------------------
void WindowBox::SetNewWindowArea(const cocos2d::Rect& newArea)
{
//     m_boxedWindow->setArea(newArea);
// 
//     // Update the resizer positions
//     Reset();
}

//------------------------------------------------------------------------
void WindowBox::SetNewWindowPosition(const cocos2d::Vec2& newPosition)
{
    _boxedWindow->setPosition(newPosition);
    // Update the resizer positions
    Reset();
}

void WindowBox::onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
	
	DrawPrimitives::drawRect(_resizePoints[0].origin + _resizePoints[0].size / 2, _resizePoints[4].origin + _resizePoints[4].size / 2);
	const Color4F color(1, 0, 0, 1);

	for (int i = 0; i < NUM_RESIZE_POINTS; i++) {
		const Rect& rc = _resizePoints[i];
		DrawPrimitives::drawSolidRect(rc.origin, rc.origin + rc.size, color);
	}
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
void WindowBox::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	_drawCmd.init(_globalZOrder, transform, flags);
	_drawCmd.func = CC_CALLBACK_0(WindowBox::onDraw, this, renderer, _boxedWindow->getNodeToWorldTransform(), flags);
	renderer->addCommand(&_drawCmd);
}
