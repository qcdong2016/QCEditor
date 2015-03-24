
#ifndef _WINDOW_BOX_H_
#define _WINDOW_BOX_H_

#include "2d/CCNode.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "qobject.h"

USING_NS_CC;

#define NUM_RESIZE_POINTS 8

#define RESIZE_POINT_WN 0   // West-North
#define RESIZE_POINT_N  1   // North
#define RESIZE_POINT_NE 2   // North-East
#define RESIZE_POINT_E  3   // East
#define RESIZE_POINT_ES 4   // East-South
#define RESIZE_POINT_S  5   // South
#define RESIZE_POINT_SW 6   // South-West
#define RESIZE_POINT_W  7   // West

#define NUM_RESIZE_DIRS 4

#define RESIZE_DIR_N  0   // North
#define RESIZE_DIR_E  1   // East
#define RESIZE_DIR_S  2   // South
#define RESIZE_DIR_W  3   // West

#define RESIZE_POINT_NONE -1


class WindowBox : public QObject, public Node
{
	Q_OBJECT
public:

    WindowBox(Node* aWindow, bool aResizable = true);
	bool operator==(WindowBox& aBox);

	void setCurrentNode(Node* node);
    
    void Reset();

	Node* getNode()
    {
        return _boxedNode;
    }

	int getPointAtPosition(float x, float y) const;
	bool isPointInBoxRect(float x, float y) const;

	void updateWindowAreas(float left, float top, float right, float bottom);

	void setNewWindowPosition(const Vec2& newPosition);

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

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	void onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags);

signals:
	void onPositionChanged(const Vec2& vec);

private:


    bool			_locked;
	bool			_resizable;	

	Node* _boxedNode;
	std::vector<Vec3> _resizePoints;
	CustomCommand _drawCmd;
};

#endif // _WINDOW_BOX_H_
