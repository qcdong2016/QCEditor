#include "BoxList.h"
#include "qmenu.h"
#include "qtreewidget.h"
#include "Editor/SceneCtrl.h"

BoxList::BoxList(QWidget *parent)
	: QWidget(parent)
	, _index(0)
{
	ui.setupUi(this);
	connect(ui.treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMenu(const QPoint&)));
}

BoxList::~BoxList()
{

}

QTreeWidgetItem* BoxList::add(const QString& name, QTreeWidgetItem* parent)
{
	QTreeWidgetItem *item = new QTreeWidgetItem();
	item->setCheckState(0, Qt::Checked);
	item->setText(0, name);

	if (parent == nullptr)
		ui.treeWidget->insertTopLevelItem(0, item);
	else
		parent->addChild(item);

	return item;
}

void BoxList::showMenu(const QPoint& pos)
{
	QMenu* menu = new QMenu();

	QAction* act = new QAction(tr("&Add"), menu);
	connect(act, SIGNAL(triggered()), this, SLOT(doAddWidget()));
	menu->addAction(act);

	_currentWidget = ui.treeWidget->itemAt(pos);

	menu->exec(QCursor::pos());
}

void BoxList::doAddWidget()
{
	QTreeWidgetItem* newItem = add("new" + QString::number(_index), _currentWidget);

	newItem->setSelected(true);

	if (!_currentWidget)
		return;

	if (_currentWidget->childCount() == 1)
		_currentWidget->setExpanded(true);

	_currentWidget->setSelected(false);
	_currentWidget = NULL;
}

void BoxList::updateList()
{
	Node* root = _sceneCtrl->getUiRoot();

	add("root");
}
