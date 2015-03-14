#ifndef EDITORMAIN_H
#define EDITORMAIN_H

#include <QtWidgets/QMainWindow>
#include "ui_EditorMain.h"
#include "BoxList.h"
#include "Editor/PropertyDef.h"

class QtProperty;

class EditorMain : public QMainWindow
{
	Q_OBJECT

public:
	EditorMain(QWidget *parent = 0);
	~EditorMain();

private slots:
	void valueChanged(QtProperty *, const QVariant &);
protected:
	virtual void closeEvent(QCloseEvent *) override;
private:
	Ui::EditorMainClass ui;
	BoxList* _boxlist;
	AttributeInfoMap _attrMap;
};

#endif // EDITORMAIN_H
