#ifndef _SCENECTRL_H_
#define _SCENECTRL_H_

#include "qobject.h"
#include "2d/CCScene.h"
#include "Common.h"

USING_NS_CC;
class WindowBox;
class NewItemData;
class NodeInfo;

class SceneCtrl : public QObject, public Scene
{
	Q_OBJECT

public:
	SceneCtrl();

	void mouseMoveEvent(float x, float y);
	void mousePressEvent(float x, float y);
	void mouseReleaseEvent(float x, float y);

	Node* getUiRoot();
	WindowBox* getBox() { return _boxesNode; }

	void onResize(float frameWidth, float frameHeight);
	bool init(float frameWidth, float frameHeight);
	static SceneCtrl* create(float frameWidth, float frameHeight);

	std::string getNodeType(Node* node);

signals:
	void selectedBox();

public slots:
	void setCurrentNode(Node*);
	void registerNode(NodeInfo* info);

private:


	int _hoveredResizePoint;
	bool _isMouseHoveredBox;
	bool _mousePressed;

	float _lastx;
	float _lasty;

	Node* _rootNode;
	WindowBox* _boxesNode;
	std::map<Node*, std::string> _nodeTypeMapping;
};

#endif//_SCENECTRL_H_