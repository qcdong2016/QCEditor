#include "BoxList.h"
#include "qmenu.h"
#include "qtreewidget.h"

BoxList::BoxList(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);


	connect(ui.treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMenu(const QPoint&)));

	add("root");
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
	QTreeWidgetItem* newItem = add("new", _currentWidget);

	newItem->setSelected(true);

	if (!_currentWidget)
		return;

	if (_currentWidget->childCount() == 1)
		_currentWidget->setExpanded(true);

	_currentWidget->setSelected(false);
	_currentWidget = NULL;
}
