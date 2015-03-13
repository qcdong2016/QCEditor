#include "WindowBox.h"
#include "2d/CCDrawingPrimitives.h"
#include "base/CCDirector.h"
#include "qlogging.h"

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
	const Mat4& worldMat = _boxedWindow->getNodeToWorldTransform();
	const Size& size = _boxedWindow->getContentSize();
	const int height = size.height;
	const int width = size.width;

	Vec3 ofset;
	worldMat.getTranslation(&ofset);

	_resizePoints.clear();

	_resizePoints.push_back(Vec3(0, 0, 0));
	_resizePoints.push_back(Vec3(width / 2, 0, 0));
	_resizePoints.push_back(Vec3(width, 0, 0));
	_resizePoints.push_back(Vec3(width, -height / 2, 0));
	_resizePoints.push_back(Vec3(width, -height, 0));
	_resizePoints.push_back(Vec3(width / 2, -height, 0));
	_resizePoints.push_back(Vec3(0, -height, 0));
	_resizePoints.push_back(Vec3(0, -height / 2, 0));

	for (size_t i = 0; i < _resizePoints.size(); i++) {
		_resizePoints[i].y += height;
		worldMat.transformVector(&_resizePoints[i]);
		_resizePoints[i] += ofset;
	}
}

//------------------------------------------------------------------------
int WindowBox::GetPointAtPosition (const cocos2d::Point& aPoint) const
{
    if (_locked || !_resizable)
        return RESIZE_POINT_NONE;

	Vec3 mousePos = Vec3(aPoint.x, aPoint.y, 0);

    for (int i = 0; i < NUM_RESIZE_POINTS; ++i) 
    {
		if (_resizePoints[i].distance(mousePos) <= 6)//
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
// 	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
// 	Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
// 	
// 	DrawPrimitives::drawRect(_resizePoints[0], _resizePoints[4]);
// 	const Color4F color(1, 0, 0, 1);
// 
// 	for (int i = 0; i < NUM_RESIZE_POINTS; i++) {
// 		DrawPrimitives::drawSolidCircle(_resizePoints[i], 6, 0, 10);
// 	}
// 	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	Vec2 pos2d[8];

	for (int i = 0; i < NUM_RESIZE_POINTS; ++i)
	{
		pos2d[i].x = _resizePoints[i].x;
		pos2d[i].y = _resizePoints[i].y;
	}

	for (int i = 0; i < NUM_RESIZE_POINTS; ++i)
	{
		DrawPrimitives::drawSolidCircle(pos2d[i], 6, 0, 10);
	}

	DrawPrimitives::drawLine(pos2d[0], pos2d[2]);
	DrawPrimitives::drawLine(pos2d[2], pos2d[4]);
	DrawPrimitives::drawLine(pos2d[4], pos2d[6]);
	DrawPrimitives::drawLine(pos2d[6], pos2d[0]);
}

void WindowBox::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	_drawCmd.init(_globalZOrder, transform, flags);
	_drawCmd.func = CC_CALLBACK_0(WindowBox::onDraw, this, renderer, _boxedWindow->getNodeToWorldTransform(), flags);
	renderer->addCommand(&_drawCmd);
}
