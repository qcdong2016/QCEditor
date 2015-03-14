#ifndef BOXLIST_H
#define BOXLIST_H

#include <QWidget>
#include "ui_BoxList.h"

class BoxList : public QWidget
{
	Q_OBJECT

public:
	BoxList(QWidget *parent = 0);
	~BoxList();

	void add();
private slots:
	void showMenuItem(QTreeWidgetItem * item, int column);
	void showMenu(const QPoint& pos);
	void doAddWidget();

private:
	Ui::Form ui;
};

#endif // BOXLIST_H
