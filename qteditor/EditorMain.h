#ifndef EDITORMAIN_H
#define EDITORMAIN_H

#include <QtWidgets/QMainWindow>
#include "ui_EditorMain.h"
#include "BoxList.h"

class EditorMain : public QMainWindow
{
	Q_OBJECT

public:
	EditorMain(QWidget *parent = 0);
	~EditorMain();
protected:
	virtual void closeEvent(QCloseEvent *) override;
private:
	Ui::EditorMainClass ui;
	BoxList* _boxlist;
};

#endif // EDITORMAIN_H
