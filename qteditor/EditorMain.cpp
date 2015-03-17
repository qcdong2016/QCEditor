#include "EditorMain.h"
#include "cocos2d/cocos/base/CCDirector.h"
#include "BoxList.h"
#include "qttreepropertybrowser.h"
#include "qtvariantproperty.h"
#include "Editor/PropertyDef.h"
#include "qtpropertybrowser.h"
#include "Editor/QC_GLView.h"
#include "Editor/QC_GLWidget.h"

EditorMain::EditorMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	_boxlist = new BoxList(NULL);
	ui.widgetDock->setWidget(_boxlist);

	_variantManager = new QtVariantPropertyManager();
	QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();

	connect(_variantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
		this, SLOT(valueChanged(QtProperty *, const QVariant &)));

	_variantEditor = new QtTreePropertyBrowser();
	_variantEditor->setFactoryForManager(_variantManager, variantFactory);
	_variantEditor->setPropertiesWithoutValueMarked(true);
	_variantEditor->setRootIsDecorated(false);

	ui.propDock->setWidget(_variantEditor);
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
	//slowly
	auto& iter = _attrMap.infoMap.find(std::string(name.toLocal8Bit()));
	if (iter != _attrMap.infoMap.end())
	{
		AttributeInfo* info = iter->second;
		info->_accessor->set(_glwindow->getBox()->GetWindow(), value);
	}
}

void EditorMain::boxPositionChanged(const Vec2& pos)
{
	_variantManager->setValue(_attrMap.get("Position")->_prop, QPoint(pos.x, pos.y));
}

void EditorMain::viewBoxAttr()
{

	QtProperty *topItem = _variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
		QLatin1String("Node Property"));

	PropertyDef::cocos2d_Node_properties(_attrMap);
	//
	Node* node = _glwindow->getBox()->GetWindow();

	for (auto& iter = _attrMap.infoMap.begin(); iter != _attrMap.infoMap.end(); iter++)
	{
		AttributeInfo* info = iter->second;
		QtVariantProperty *item = _variantManager->addProperty(info->_defaultValue.type(), QLatin1String(info->_name.c_str()));

		QVariant value;
		info->_accessor->get(node, value);

		item->setValue(value);

		info->_prop = item;

		if (info->_setMinimum)
			item->setAttribute(QLatin1String("minimum"), info->_minimum);
		if (info->_setMaximum)
			item->setAttribute(QLatin1String("maximum"), info->_maximum);
		if (info->_setStep)
			item->setAttribute(QLatin1String("singleStep"), info->_singleStep);

		topItem->addSubProperty(item);
	}

	_variantEditor->clear();
	_variantEditor->addProperty(topItem);
}

void EditorMain::onStart()
{
	_glwindow = new QCGLWidget(this);

	_glwindow->setWindowFlags(_glwindow->windowFlags() & ~Qt::WindowMaximizeButtonHint);
	_glwindow->setMinimumSize(500, 500);

	setCentralWidget(_glwindow);

	QCGLView* view = QCGLView::getInstance();

	_glwindow->makeCurrent();
	view->init();

	auto director = Director::getInstance();

	director->setOpenGLView(view);
	view->setFrameSize(_glwindow->frameSize().width(), _glwindow->frameSize().height());

	_glwindow->startCocos2d();

	connect(_glwindow, SIGNAL(selectedBox()), this, SLOT(viewBoxAttr()));
	connect(_glwindow->getBox(), SIGNAL(onPositionChanged(const Vec2&)), this, SLOT(boxPositionChanged(const Vec2&)));
}
