
#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include "2d/CCNode.h"
USING_NS_CC;

class Serializer
{
public:
	static Node* read(const std::string& fileName);
	static void save(Node* node, const std::string& fileName);
};

#endif//_SERIALIZER_H_