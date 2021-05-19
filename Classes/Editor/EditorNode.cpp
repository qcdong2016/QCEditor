#include "EditorNode.h"
#include "base/CCDirector.h"
#include "2d/CCDrawingPrimitives.h"
#include "renderer/CCRenderer.h"


static void editor_draw(const Mat4 &transform, Size size)
{

	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	//todo: draw name, draw Anchor Point?
	DrawPrimitives::setDrawColor4F(0, 1, 0, 1);
	DrawPrimitives::drawRect(Vec2(0, 0), size);
	DrawPrimitives::setDrawColor4F(1, 1, 1, 1);

	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void EditorNode::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
	_drawCmd.init(_globalZOrder, transform, flags);
	_drawCmd.func = std::bind(editor_draw, transform, getContentSize());
	renderer->addCommand(&_drawCmd);
}
