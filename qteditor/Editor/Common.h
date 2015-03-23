#ifndef _COMMON_H_
#define _COMMON_H_

#include "2d/CCNode.h"
#include "qstring.h"
#include "SceneCtrl.h"
#include "BoxList.h"
#include "EditorMain.h"

USING_NS_CC;

class NewItemData
{
public:
	enum ItemType
	{
		TNode, TSprite, TLayer, TNothing
	};

	Node* parent;
	ItemType type;
	QString name;
};


class Global
{
public:
	static SceneCtrl* sceneCtrl;
	static BoxList* boxList;
	static EditorMain* editorMain;
};

#endif//_COMMON_H_