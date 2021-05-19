
#include "EditorMain.h"
#include "cocos2d.h"
#include "qttreepropertybrowser.h"
#include "qtvariantproperty.h"
#include "qtpropertybrowser.h"
#include "Editor/PropertyDef.h"
#include "Editor/QC_GLWidget.h"
#include "Editor/SceneCtrl.h"
#include "BoxList.h"
#include "qfiledialog.h"
#include "Serializer.h"

EditorMain::EditorMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	_boxlist = new BoxList(NULL);
	ui.widgetDock->setWidget(_boxlist);

	_variantManager = new QtVariantPropertyManager();
	QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();

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
	PropertyDef::setProperty(_sceneCtrl->getBox()->getNode(), prop, value);
}

void EditorMain::boxPositionChanged(const Vec2& pos)
{
	PropertyDef::setPosition(_sceneCtrl->getBox()->getNode(), QPoint(pos.x, pos.y));
}

void EditorMain::viewBoxAttr(Node* node)
{
	//avoid while(1)
	disconnect(_variantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)), this, SLOT(valueChanged(QtProperty *, const QVariant &)));

	PropertyDef::setupProperties(_sceneCtrl->getNodeType(node), node, _variantEditor, _variantManager);

	connect(_variantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)), this, SLOT(valueChanged(QtProperty *, const QVariant &)));
}

void EditorMain::onStart()
{
	_glwindow = new QCGLWidget(this);

	_glwindow->setWindowFlags(_glwindow->windowFlags() & ~Qt::WindowMaximizeButtonHint);
	_glwindow->setMinimumSize(500, 500);

	setCentralWidget(_glwindow);
    
	FileUtils::getInstance()->addSearchPath("Resources/");
    
    
    createActions();
    
    _glwindow->afterInit = [this]() {
        _sceneCtrl = _glwindow->getScene();

        connect(_boxlist, SIGNAL(onSelectNode(Node*)), this, SLOT(viewBoxAttr(Node*)));
        connect(_boxlist, SIGNAL(onSelectNode(Node*)), _sceneCtrl, SLOT(setCurrentNode(Node*)));
        connect(_boxlist, SIGNAL(newNode(NodeInfo*)), _sceneCtrl, SLOT(registerNode(NodeInfo*)));
        connect(_sceneCtrl->getBox(), SIGNAL(onPositionChanged(const Vec2&)), this, SLOT(boxPositionChanged(const Vec2&)));

        NodeInfo info;
        info.typeName = "Node";
        info.self = _sceneCtrl->getUiRoot();
        _boxlist->updateWithTree(&info);
    };
}

void EditorMain::createActions()
{
	_saveAction = new QAction("&Save", this);
	_saveAction->setShortcut(QKeySequence::Save);
	ui.menuFile->insertAction(ui.action_Quit, _saveAction);
	connect(_saveAction, SIGNAL(triggered()), this, SLOT(save()));

	_loadAction = new QAction("&Load", this);
	_loadAction->setShortcut(QKeySequence::Open);
	ui.menuFile->insertAction(ui.action_Quit, _loadAction);
	connect(_loadAction, SIGNAL(triggered()), this, SLOT(load()));
}

void EditorMain::save()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save"), "", tr("Save File (*.qc)"));
	NodeInfo tree;
	_boxlist->buildTree(&tree);
	Serializer::save(&tree, std::string(fileName.toUtf8()));
}

void EditorMain::load()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select File"), "", "Editor Files (*.qc *.xml)");

	if (fileName.length() != 0)
	{
		NodeInfo rootinfo;
		Serializer::read(std::string(fileName.toUtf8()), &rootinfo);
		_sceneCtrl->setUiRoot((*rootinfo.children.begin()).self);
		_boxlist->updateWithTreeRoot(&rootinfo);
	}
}
