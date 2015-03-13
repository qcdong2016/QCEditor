#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "WindowBox.h"

class HelloWorld : public cocos2d::Scene
{
public:
	HelloWorld();


	Node* getRoot() { return _root; }
	WindowBox* getBox() { return _boxNode; }

	void onResize(int width, int height);

	virtual bool init();

	CREATE_FUNC(HelloWorld);
private:
	WindowBox* _boxNode;
	Node* _root;
};

#endif // __HELLOWORLD_SCENE_H__
