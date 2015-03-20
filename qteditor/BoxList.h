#ifndef BOXLIST_H
#define BOXLIST_H

#include <QWidget>
#include "ui_BoxList.h"
#include "2d/CCNode.h"

USING_NS_CC;

class BoxList : public QWidget
{
	Q_OBJECT

public:
	BoxList(QWidget *parent = 0);
	~BoxList();

	QTreeWidgetItem* add(const QString& name, QTreeWidgetItem* parent = nullptr);

private slots:
	void showMenu(const QPoint& pos);
	void doAddWidget();

public slots:
	void updateList(Node* root);
signals:
	void onAddNewItem(Node* parent);

private:
	Ui::Form ui;
	QTreeWidgetItem* _currentWidget;
	int _index;
};

#endif // BOXLIST_H
