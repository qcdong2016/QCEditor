#include "Serializer.h"
#include "Common.h"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"

#include "AttributeAccessor.h"
#include <iostream>
#include <fstream>
#include "SceneCtrl.h"
#include "CCFileUtils.h"

static void saveTree(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent, NodeTree* node)
{
	const std::string& type = Global::sceneCtrl->getNodeType(node->self);
	rapidxml::xml_node<>* xmlnode = doc.allocate_node(rapidxml::node_element, doc.allocate_string(type.c_str()));
	parent->append_node(xmlnode);

	AccessorGroup* gp = AAManager::getInstance().getGroup(type);

	for (AccessorGroup::AAInfoList::iterator iter = gp->infolist.begin(); iter != gp->infolist.end(); iter++)
	{
		AAInfo* info = *iter;
		AttributeAccessor* aa = info->accessor;
		Variant v;
		aa->get(node->self, v);
		if (v == info->defaultValue)
			continue;

		std::string s;
		aa->save(v, s);

		rapidxml::xml_node<>* attrnode = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Property"));
		rapidxml::xml_attribute<>* attrname = doc.allocate_attribute("name", doc.allocate_string(aa->getName().c_str()));
		rapidxml::xml_attribute<>* attrvalue = doc.allocate_attribute("value", doc.allocate_string(s.c_str()));
		attrnode->append_attribute(attrname);
		attrnode->append_attribute(attrvalue);
		xmlnode->append_node(attrnode);
	}

	for (auto iter : node->children)
	{
		saveTree(doc, xmlnode, &iter);
	}
}

void Serializer::save(NodeTree* tree, const std::string& fileName)
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

#define streq(s1, s2) (!strcmp(s1, s2))


static void setDefaultValue(AccessorGroup* gp, Node* node)
{
	AccessorGroup::AAInfoList::iterator iter;

	for (iter = gp->infolist.begin(); iter != gp->infolist.end(); iter++)
	{
		AAInfo* aainfo = *iter;
		if (!aainfo->defaultValue.isNull())
			aainfo->accessor->set(node, aainfo->defaultValue);
	}

	if (gp->parent != nullptr)
		setDefaultValue(gp->parent, node);
}

static Node* loadNode(rapidxml::xml_node<>* element)
{
	AccessorGroup* gp = AAManager::getInstance().getGroup(element->name());

	if (!gp) return nullptr;// 

	Node* node = gp->ctor->operator()();
	setDefaultValue(gp, node);//slowly

	rapidxml::xml_node<>* prop = element->first_node();
	while (prop)
	{
		if (streq(prop->name(), "Property"))
		{
			AttributeAccessor* aa = gp->get(prop->first_attribute("name")->value());
			const char* vstr = prop->first_attribute("value")->value();
			Variant v;
			aa->read(vstr, v);
			aa->set(node, v);
		}
		else 
		{
			node->addChild(loadNode(prop));
		}
		prop = prop->next_sibling();
	}

	return node;
}

Node* Serializer::read(const std::string& fileName)
{
// 	Data d = FileUtils::getInstance()->getDataFromFile(fileName);
// 
// 	char* buf = new char[d.getSize()];
// 	memcpy(buf, d.getBytes(), d.getSize());
	rapidxml::file<> fdoc(fileName.c_str());
	rapidxml::xml_document<> doc;
	try
	{
		doc.parse<0>(fdoc.data());
	}
	catch (std::exception* e)
	{
		qDebug("%s", e->what());
	}

	rapidxml::xml_node<>* root = doc.first_node();
	if (!streq(root->name(), "Root"))//some log ?
		return nullptr;

	rapidxml::xml_node<>* uiroot = root->first_node();
	if (!streq(uiroot->name(), "UI"))
		return nullptr;

	Node* node =  loadNode(uiroot->first_node());

	return node;
}