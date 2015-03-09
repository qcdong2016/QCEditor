#include "2d\CCNode.h"


#ifndef _WINDOW_BOX_H_
#define _WINDOW_BOX_H_



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

    /************************************************************************
     * CONSTRUCTORS/DESTRUCTORS
     ************************************************************************/
    
    /** Constructor, accepts a window and a setting whether it can be resized.*/
    WindowBox(cocos2d::Node* aWindow, bool aResizable = true);

    /************************************************************************
     * PUBLIC FUNCTIONS
     ************************************************************************/

	// Operator to help STL::list::remove
	bool operator==(WindowBox& aBox);
    
    /** Re-positions the resize-points. Used when the resizer is initialized or when the 
    * contained GuiElement has been resized.*/
    void Reset();

    /** Returns the window to which this resizer is attached.*/
	cocos2d::Node* GetWindow()
    {
        return m_boxedWindow;
    }

    /** Checks if the mouse is within a resize point. If so, it returns the point's index (0-7).*/
    int GetPointAtPosition(const cocos2d::Point& point) const;

    /** Returns an absolute rectangle for the given point.
     *
     * @param iIndex the point (0-7).
     * @return absolute rectangle.
     */
	cocos2d::Rect GetPointRect(const int anIndex) const;

    /** Returns a point for the given side.
     * If udimOrigin is set we actually return the point corresponding
     * to the udim position with 0 scale.  That point is of particular
     * significance because it is the point we 'offset' from.
     *
     * @param anDir the point (0-3).
     * @param udimOrigin if true, finds the udim origin position
     * @return absolute point.
     */
	cocos2d::Point GetSidePoint(const int anDir, const bool udimOrigin) const;

    /** Returns an absolute rectangle for the boxed Window.
     *
     * @return absolute rectangle.
     */
	cocos2d::Rect GetWindowRect() const;

    /** Applies a new rectangle to the boxed window, and then updates the box itself.*/
	void SetNewWindowArea(const cocos2d::Rect& newArea);

    /** Applies a new position to the boxed window, and then updates the box itself.*/
	void SetNewWindowPosition(const cocos2d::Vec2& newPosition);

    /** Locks or unlocks the boxed window from allowing its properties to be changed.*/
    void Lock(bool lock = true)
    {
        m_locked = lock;
    }

    /** Returns the lock state of the boxed window.*/
    bool IsLocked()
    {
        return m_locked;
    }

	/** Returns the resize state of the boxed window.*/
    bool IsResizable()
    {
        return m_resizable;
    }
private:

    /************************************************************************
     * PRIVATE VARIABLES
     ************************************************************************/
    
    /** The windows around which this resize box resides.*/
	cocos2d::Node* m_boxedWindow;

    /** 8 CEGUI rectangles with absolute and unclipped positions and sizes per point.*/
	cocos2d::Rect       m_resizePoints[NUM_RESIZE_POINTS];

    /** Holds whether the boxed window is locked. When true, its position and size can not be changed.
     * in the future, each window should be wrapped in this box so that locking information can also be remembered,
     * when a window is not selected anymore. Then add methods for "setSelected" and such.*/
    bool			m_locked;
	bool			m_resizable;	// False when selected but not resizable. Happens when a parent is selected.
};

#endif // _WINDOW_BOX_H_
