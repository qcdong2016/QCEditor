
#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include "2d/CCNode.h"
USING_NS_CC;

#ifdef QC_EDITOR
struct NodeInfo;
#endif

class Serializer
{
public:
#ifdef QC_EDITOR
	static Node* read(const std::string& fileName, NodeInfo* tree);
	static void save(NodeInfo* tree, const std::string& fileName);
#else
	static Node* read(const std::string& fileName);
#endif
};

#endif//_SERIALIZER_H_