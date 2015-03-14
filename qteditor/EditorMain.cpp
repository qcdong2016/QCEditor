#include "EditorMain.h"
#include "cocos2d/cocos/base/CCDirector.h"
#include "BoxList.h"
#include "qttreepropertybrowser.h"
#include "qtvariantproperty.h"
#include "Editor/PropertyDef.h"
#include "qtpropertybrowser.h"
#include "Editor/CCQGLView.h"

EditorMain::EditorMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	_boxlist = new BoxList(NULL);
	ui.widgetDock->setWidget(_boxlist);

	QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();
	QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();

	connect(variantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
		this, SLOT(valueChanged(QtProperty *, const QVariant &)));

	QtProperty *topItem = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
		QLatin1String("Node Property"));
	
	PropertyDef::cocos2d_Node_properties(_attrMap);

	for (auto& iter = _attrMap.infoMap.begin(); iter != _attrMap.infoMap.end(); iter++)
	{
		AttributeInfo* info = iter->second;
		QtVariantProperty *item = variantManager->addProperty(info->_defaultValue.type(), QLatin1String(info->_name.c_str()));
		item->setValue(info->_defaultValue);
		if (info->_setMinimum)
		    item->setAttribute(QLatin1String("minimum"), info->_minimum);
		if (info->_setMaximum)
		    item->setAttribute(QLatin1String("maximum"), info->_maximum);
		if (info->_setStep)
			item->setAttribute(QLatin1String("singleStep"), info->_singleStep);

		topItem->addSubProperty(item);
	}

	QtTreePropertyBrowser *variantEditor = new QtTreePropertyBrowser();
	variantEditor->setFactoryForManager(variantManager, variantFactory);
	variantEditor->addProperty(topItem);
	variantEditor->setPropertiesWithoutValueMarked(true);
	variantEditor->setRootIsDecorated(false);


	ui.propDock->setWidget(variantEditor);
}

EditorMain::~EditorMain()
{
}

void EditorMain::closeEvent(QCloseEvent *)
{
	cocos2d::Director::getInstance()->end();
	qApp->quit();
}

void EditorMain::valueChanged(QtProperty* prop, const QVariant& value)
{
	QString name = prop->propertyName();
	auto& iter = _attrMap.infoMap.find(std::string(name.toLocal8Bit()));
	if (iter != _attrMap.infoMap.end())
	{
		AttributeInfo* info = iter->second;
		CCQGLView* view = (CCQGLView*)Director::getInstance()->getOpenGLView();
		if (view && view->getBox())
			info->_accessor->set(view->getBox()->GetWindow(), value);
	}
}
