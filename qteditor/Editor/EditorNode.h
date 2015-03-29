#ifndef _EDITOR_NODE_H_
#define _EDITOR_NODE_H_

#include "renderer/CCCustomCommand.h"
#include "2d/CCNode.h"
USING_NS_CC;

class EditorNode : public Node
{
public:
	virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
	CREATE_FUNC(EditorNode);
private:
	CustomCommand _drawCmd;
};

#endif//_EDITOR_NODE_H_