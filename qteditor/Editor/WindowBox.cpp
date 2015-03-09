#include "WindowBox.h"

USING_NS_CC;

WindowBox::WindowBox(cocos2d::Node* aWindow, bool aResizable/* = true*/) :
m_boxedWindow(aWindow),
m_resizable(aResizable),
m_locked(false)
{
    Reset ();
}

bool WindowBox::operator==(WindowBox& aBox)
{
	return (m_boxedWindow == aBox.m_boxedWindow);
}

void WindowBox::Reset () 
{
    m_locked = false;
    // Find the amount to convert from relative- to screen coordinates
    // Do this by finding the parent of the element (until no parent)
    const cocos2d::Rect winRc = m_boxedWindow->getBoundingBox();

    // Make boxes 3x3 and always in pixels
    //int width = 7;
    //int height = 7;
    const int paddX = 3;
    const int paddY = 3;
	const cocos2d::Vec2 offset(-paddX, paddY);
	const int height = winRc.size.height;
	const int width = winRc.size.width;

    // Position the "TopLeft" resizer
	m_resizePoints[RESIZE_POINT_WN].origin = winRc.origin + offset;
    // Position the "Top" resizer
	m_resizePoints[RESIZE_POINT_N].origin = winRc.origin + cocos2d::Vec2(width / 2, 0) + offset;
    // Position the "TopRight" resizer
	m_resizePoints[RESIZE_POINT_NE].origin = winRc.origin + cocos2d::Vec2(width, 0) + offset;
    // Position the "Right" resizer
	m_resizePoints[RESIZE_POINT_E].origin = winRc.origin + cocos2d::Vec2(0, width / 2) + offset;
    // Position the "BottomRight" resizer
	m_resizePoints[RESIZE_POINT_ES].origin = winRc.origin + winRc.getWidth() - paddX;
    // Position the "Bottom" resizer
	m_resizePoints[RESIZE_POINT_S].origin = winRc.origin + winRc.getWidth() / 2 - paddX;
    // Position the "BottomLeft" resizer
	m_resizePoints[RESIZE_POINT_SW].origin = winRc.origin - paddX;
    // Position the "Left" resizer
	m_resizePoints[RESIZE_POINT_W].origin = winRc.origin - paddX;

    for (int i = 0; i < NUM_RESIZE_POINTS; ++i) 
    {
		m_resizePoints[i].size = padd * 2;
    }
}

//------------------------------------------------------------------------
wxRect WindowBox::GetPointRect (const int anIndex) const 
{
    const CEGUI::Rect rc = m_resizePoints [anIndex];
    return wxRect (wxPoint(wx_static_cast(int, rc.d_left), wx_static_cast(int, rc.d_top)), wxPoint(wx_static_cast(int, rc.d_right), wx_static_cast(int, rc.d_bottom)));
}

//------------------------------------------------------------------------
wxPoint WindowBox::GetSidePoint(const int anDir, const bool udimOrigin) const
{
    wxASSERT_MSG(m_boxedWindow != NULL, wxT("Boxed Window must exist"));
    CEGUI::UVector2 boxPos;
    CEGUI::UDim hwidth;
    CEGUI::UDim hheight;
    switch (anDir)
    {
    case RESIZE_DIR_N:
        boxPos.d_x = m_boxedWindow->getXPosition();
        boxPos.d_y = m_boxedWindow->getYPosition();
        boxPos.d_x += m_boxedWindow->getWidth()/CEGUI::UDim(2.0f,2.0f);
        if (udimOrigin)
        {
            boxPos.d_y.d_offset = 0;
        }
        break;
    case RESIZE_DIR_W:
        boxPos.d_x = m_boxedWindow->getXPosition();
        boxPos.d_y = m_boxedWindow->getYPosition();
        boxPos.d_y += m_boxedWindow->getHeight()/CEGUI::UDim(2.0f,2.0f);
        if (udimOrigin)
        {
            boxPos.d_x.d_offset = 0;
        }
        break;
    case RESIZE_DIR_S:
        boxPos.d_x = m_boxedWindow->getXPosition();
        boxPos.d_y = m_boxedWindow->getYPosition();
        boxPos.d_x += m_boxedWindow->getWidth()/CEGUI::UDim(2.0f,2.0f);
        boxPos.d_y += m_boxedWindow->getHeight();
        if (udimOrigin)
        {
            boxPos.d_y.d_offset = 0;
        }
        break;
    case RESIZE_DIR_E:
        boxPos.d_x = m_boxedWindow->getXPosition();
        boxPos.d_y = m_boxedWindow->getYPosition();
        boxPos.d_x += m_boxedWindow->getWidth();
        boxPos.d_y += m_boxedWindow->getHeight()/CEGUI::UDim(2.0f,2.0f);
        if (udimOrigin)
        {
            boxPos.d_x.d_offset = 0;
        }
        break;
    default:
        LogError(wxT("Invalid resize direction (%d) supplied"), anDir);
        return wxPoint(-1, -1);
    };
    // Now find it
    CEGUI::Vector2 absPos;
    if (m_boxedWindow->getParent())
    {
        absPos = CEGUI::CoordConverter::windowToScreen(*m_boxedWindow->getParent(), boxPos);
    }
    else
    {
        // hmm what to do here?
        absPos = CEGUI::CoordConverter::windowToScreen(*m_boxedWindow, boxPos);
    }
    return wxPoint(wx_static_cast(int, absPos.d_x), wx_static_cast(int, absPos.d_y));
}

//------------------------------------------------------------------------
wxRect WindowBox::GetWindowRect() const
{
    const CEGUI::Rect rc = m_boxedWindow->getUnclippedOuterRect();
    return wxRect (wxPoint(wx_static_cast(int, rc.d_left), wx_static_cast(int, rc.d_top)), wxPoint(wx_static_cast(int, rc.d_right), wx_static_cast(int, rc.d_bottom)));
}

//------------------------------------------------------------------------
int WindowBox::GetPointAtPosition (const CEGUI::Point& aPoint) const
{
    // If the window is locked or not resizable, we just cheat the Document
    if (m_locked || !m_resizable)
    {
        return RESIZE_POINT_NONE;
    }
    for (int i = 0; i < NUM_RESIZE_POINTS; ++i) 
    {
        if (m_resizePoints [i].isPointInRect (aPoint))
        {
            return i ;
        }    
    }
    return RESIZE_POINT_NONE;
}

//------------------------------------------------------------------------
void WindowBox::SetNewWindowArea(const CEGUI::URect& newArea)
{
    // Apply it to the CEGUI window
    m_boxedWindow->setArea(newArea);

    // Update the resizer positions
    Reset();
}

//------------------------------------------------------------------------
void WindowBox::SetNewWindowPosition(const CEGUI::UVector2& newPosition)
{
    // Apply it to the CEGUI window
    m_boxedWindow->setPosition(newPosition);
    // Update the resizer positions
    Reset();
}
