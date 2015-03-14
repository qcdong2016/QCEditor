#include "BoxList.h"
#include "qmenu.h"
#include "qtreewidget.h"

BoxList::BoxList(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);


	connect(ui.treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMenu(const QPoint&)));
	connect(ui.treeWidget, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(showMenuItem(QTreeWidgetItem*, int)));

	QTreeWidgetItem *root = new QTreeWidgetItem(ui.treeWidget);
	root->setText(0, "root");
	ui.treeWidget->insertTopLevelItem(0, root);
}

BoxList::~BoxList()
{

}

void BoxList::add()
{
}

void BoxList::showMenu(const QPoint& pos)
{
	qDebug("hello");
}

void BoxList::doAddWidget()
{
	qDebug("hello");
}

void BoxList::showMenuItem(QTreeWidgetItem * item, int column)
{
	QMenu* menu = new QMenu();

	QAction* act = new QAction(tr("&Add"), menu);
	connect(act, SIGNAL(triggered()), this, SLOT(doAddWidget()));
	menu->addAction(act);

	menu->exec(QCursor::pos());
}
