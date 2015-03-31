
#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include "2d/CCNode.h"
USING_NS_CC;

#ifdef QT_GUI_LIB//lazy
struct NodeTree;
#endif

class Serializer
{
public:
	static Node* read(const std::string& fileName);

#ifdef QT_GUI_LIB//lazy
	static void save(NodeTree* tree, const std::string& fileName);
#endif
};

#endif//_SERIALIZER_H_