#include "EditorMain.h"
#include "cocos2d/cocos/base/CCDirector.h"

EditorMain::EditorMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

EditorMain::~EditorMain()
{

}

void EditorMain::closeEvent(QCloseEvent *)
{
	cocos2d::Director::getInstance()->end();
	qApp->quit();
}
