#ifndef BOXLIST_H
#define BOXLIST_H

#include <QWidget>
#include "ui_BoxList.h"
#include "2d/CCNode.h"

USING_NS_CC;

class MyTreeWidgetItem : public QTreeWidgetItem
{
public:
	Node* node;
};


class BoxList : public QWidget
{
	Q_OBJECT

public:
	BoxList(QWidget *parent = 0);
	~BoxList();

	MyTreeWidgetItem* add(const QString& name, QTreeWidgetItem* parent = nullptr);
private slots:
	void showMenu(const QPoint& pos);
	void doAddWidget(QAction* act);
	void selectedNode(QTreeWidgetItem* curr, QTreeWidgetItem* prev);
public slots:
	void updateList(Node* root);

signals:
	void onSelectNode(Node* node);

private:
	Ui::Form ui;
	MyTreeWidgetItem* _currentWidget;
	int _index;
};

#endif // BOXLIST_H
