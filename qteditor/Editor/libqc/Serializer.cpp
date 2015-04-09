#include "Serializer.h"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"

#include "AttributeAccessor.h"
#include <iostream>
#include <fstream>
#include "CCFileUtils.h"

#ifdef QC_EDITOR
#include "Editor/Common.h"
#include "Editor/SceneCtrl.h"

static void saveTreeByGroup(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* xmlnode, const std::string& type, AccessorGroup* gp, NodeInfo* node)
{

	for (AccessorGroup::AAInfoList::iterator iter = gp->infolist.begin(); iter != gp->infolist.end(); iter++)
	{
		AAInfo* info = *iter;
		AttributeAccessor* aa = info->accessor;
		Variant v;
		aa->get(node->self, v);
		if (v == info->defaultValue)
			continue;

		std::string s;
		aa->toString(node->self, s);

		rapidxml::xml_node<>* attrnode = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Property"));
		rapidxml::xml_attribute<>* attrname = doc.allocate_attribute("name", doc.allocate_string(aa->getName().c_str()));
		rapidxml::xml_attribute<>* attrvalue = doc.allocate_attribute("value", doc.allocate_string(s.c_str()));
		attrnode->append_attribute(attrname);
		attrnode->append_attribute(attrvalue);
		xmlnode->append_node(attrnode);
	}
}

static void saveTree(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent, NodeInfo* node)
{
	const std::string& type = Global::sceneCtrl->getNodeType(node->self);
	AccessorGroup* gp = AAManager::getInstance().getGroup(type);
	rapidxml::xml_node<>* xmlnode = doc.allocate_node(rapidxml::node_element, doc.allocate_string(type.c_str()));
	parent->append_node(xmlnode);

	saveTreeByGroup(doc, xmlnode, type, gp, node);

	while (gp->parent != nullptr)
	{
		gp = gp->parent;
		saveTreeByGroup(doc, xmlnode, type, gp, node);
	}

	for (auto iter : node->children)
	{
		saveTree(doc, xmlnode, &iter);
	}
}

void Serializer::save(NodeInfo* tree, const std::string& fileName)
{
	rapidxml::xml_document<> doc;

	rapidxml::xml_node<>* rot = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version='1.0' encoding='utf-8'"));
	doc.append_node(rot);

	rapidxml::xml_node<>* xmlroot = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Root"));
	rapidxml::xml_node<>* uiroot = doc.allocate_node(rapidxml::node_element, doc.allocate_string("UI"));
	xmlroot->append_node(uiroot);
	saveTree(doc, uiroot, tree);
	doc.append_node(xmlroot);

	std::string text;
	rapidxml::print(std::back_inserter(text), doc, 0);
	//qDebug("%s", text.c_str());
	std::ofstream out(fileName);
	out << doc;
}

#endif


#define streq(s1, s2) (!strcmp(s1, s2))


// static void setDefaultValue(AccessorGroup* gp, Node* node)
// {
// 	AccessorGroup::AAInfoList::iterator iter;
// 
// 	for (iter = gp->infolist.begin(); iter != gp->infolist.end(); iter++)
// 	{
// 		AAInfo* aainfo = *iter;
// 		if (!aainfo->defaultValue.isNull())
// 			aainfo->accessor->set(node, aainfo->defaultValue);
// 	}
// 
// 	if (gp->parent != nullptr)
// 		setDefaultValue(gp->parent, node);
// }

static Node* loadNode(rapidxml::xml_node<>* element
#ifdef QC_EDITOR
	, NodeInfo* tree
#endif
	)
{
	std::string typeName = element->name();
	AccessorGroup* gp = AAManager::getInstance().getGroup(typeName);

	if (!gp) return nullptr;// 

	Node* node = gp->ctor->operator()();
	//setDefaultValue(gp, node);//I guess, this is not necessary.

#ifdef QC_EDITOR
	NodeInfo curtree;
	curtree.self = node;
	curtree.typeName = typeName;
	Global::sceneCtrl->registerNode(&curtree);
#endif

	rapidxml::xml_node<>* prop = element->first_node();
	while (prop)
	{
		if (streq(prop->name(), "Property"))
		{
			AttributeAccessor* aa = gp->get(prop->first_attribute("name")->value());
			const char* vstr = prop->first_attribute("value")->value();
			aa->fromString(node, vstr);
		}
		else 
		{
			node->addChild(loadNode(prop
#ifdef QC_EDITOR
				, &curtree
#endif
				));
		}
		prop = prop->next_sibling();
	}
#ifdef QC_EDITOR
	tree->children.push_back(curtree);
#endif

	return node;
}

static Node* read_(const std::string& fileName
#ifdef QC_EDITOR
	, NodeInfo* tree
#endif
	)
{
#ifdef QC_EDITOR
	rapidxml::file<> fdoc(fileName.c_str());
	char* data = fdoc.data();
#else
	Data d = FileUtils::getInstance()->getDataFromFile(fileName);
	char* data = (char*)d.getBytes();
#endif
	rapidxml::xml_document<> doc;
	try
	{
		doc.parse<0>(data);
	}
	catch (std::exception* e)
	{
		//qDebug("%s", e->what());
	}

	rapidxml::xml_node<>* root = doc.first_node();
	if (!streq(root->name(), "Root"))//some log ?
		return nullptr;

	rapidxml::xml_node<>* uiroot = root->first_node();
	if (!streq(uiroot->name(), "UI"))
		return nullptr;

	Node* node = loadNode(uiroot->first_node()
#ifdef QC_EDITOR
		, tree
#endif
		);

#ifdef QC_EDITOR
	tree->self = nullptr;
	Global::sceneCtrl->registerNode(tree);
#endif

	return node;
}

#ifdef QC_EDITOR
Node* Serializer::read(const std::string& fileName, NodeInfo* tree)
{
	return read_(fileName, tree);
}
#else
Node* Serializer::read(const std::string& fileName)
{
	return read_(fileName);
}
#endif
