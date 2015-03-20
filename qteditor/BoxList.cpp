#include "BoxList.h"
#include "qmenu.h"
#include "qtreewidget.h"

Q_DECLARE_METATYPE(Node*);

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

	QVariant data = _currentWidget->data(0, Qt::UserRole);
	Node* parent = (Node*)data.value<void*>();
	
	emit onAddNewItem(parent);

	_currentWidget = NULL;
}

void BoxList::updateList(Node* root)
{
	QTreeWidgetItem *item = add("root");
	item->setData(0, Qt::UserRole, QVariant::fromValue(root));
	
	
	//for each child
}
