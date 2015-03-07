#ifndef EDITORMAIN_H
#define EDITORMAIN_H

#include <QtWidgets/QMainWindow>
#include "ui_EditorMain.h"

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
};

#endif // EDITORMAIN_H
