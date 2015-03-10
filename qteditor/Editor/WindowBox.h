
#ifndef _WINDOW_BOX_H_
#define _WINDOW_BOX_H_

#include "2d\CCNode.h"
#include "renderer\CCRenderer.h"

USING_NS_CC;

/** The resize box exists of 8 points.*/
#define NUM_RESIZE_POINTS 8

/** Locations of the points.*/
#define RESIZE_POINT_WN 0   // West-North
#define RESIZE_POINT_N  1   // North
#define RESIZE_POINT_NE 2   // North-East
#define RESIZE_POINT_E  3   // East
#define RESIZE_POINT_ES 4   // East-South
#define RESIZE_POINT_S  5   // South
#define RESIZE_POINT_SW 6   // South-West
#define RESIZE_POINT_W  7   // West

/** The resize box has 4 cardinal directions.*/
#define NUM_RESIZE_DIRS 4

/** Locations of the directions.*/
#define RESIZE_DIR_N  0   // North
#define RESIZE_DIR_E  1   // East
#define RESIZE_DIR_S  2   // South
#define RESIZE_DIR_W  3   // West

/** Value indicating that the mouse is not hovering on a resize point.*/
#define RESIZE_POINT_NONE -1

/** Class which wraps a selected Window. Provides for resizing and repositioning of that window,
* besides it holds whether the window's properties may be updated (the 'locked' member.)*/
class WindowBox
{

public:

    WindowBox(cocos2d::Node* aWindow, bool aResizable = true);

	// Operator to help STL::list::remove
	bool operator==(WindowBox& aBox);
    
    void Reset();

	cocos2d::Node* GetWindow()
    {
        return _boxedWindow;
    }

    /** Checks if the mouse is within a resize point. If so, it returns the point's index (0-7).*/
	int GetPointAtPosition(const cocos2d::Vec2& point) const;

	void SetNewWindowArea(const cocos2d::Rect& newArea);
	void SetNewWindowPosition(const cocos2d::Vec2& newPosition);

    void Lock(bool lock = true)
    {
        _locked = lock;
    }

    bool IsLocked()
    {
        return _locked;
    }

    bool IsResizable()
    {
        return _resizable;
    }

	void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
private:

	cocos2d::Node* _boxedWindow;
	std::vector<Rect> _resizePoints;

    bool			_locked;
	bool			_resizable;	
};

#endif // _WINDOW_BOX_H_
