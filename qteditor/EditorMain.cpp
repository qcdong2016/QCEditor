#include "EditorMain.h"
#include "cocos2d/cocos/base/CCDirector.h"
#include "BoxList.h"

EditorMain::EditorMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	_boxlist = new BoxList(NULL);
	ui.widgetDock->setWidget(_boxlist);
}

EditorMain::~EditorMain()
{
}

void EditorMain::closeEvent(QCloseEvent *)
{
	cocos2d::Director::getInstance()->end();
	qApp->quit();
}
