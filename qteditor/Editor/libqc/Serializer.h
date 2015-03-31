
#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include "2d/CCNode.h"
USING_NS_CC;

#ifdef QC_EDITOR
struct NodeTree;
#endif

class Serializer
{
public:
	static Node* read(const std::string& fileName);

#ifdef QC_EDITOR
	static void save(NodeTree* tree, const std::string& fileName);
#endif
};

#endif//_SERIALIZER_H_