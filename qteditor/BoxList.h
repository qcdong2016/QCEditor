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

	QTreeWidgetItem* add(const QString& name, QTreeWidgetItem* parent = nullptr);
	private slots:
	void showMenu(const QPoint& pos);
	void doAddWidget();

private:
	Ui::Form ui;
	QTreeWidgetItem* _currentWidget;
};

#endif // BOXLIST_H
