#ifndef EDITORMAIN_H
#define EDITORMAIN_H

#include <QtWidgets/QMainWindow>
#include "ui_EditorMain.h"
#include "BoxList.h"
#include "Editor/PropertyDef.h"
#include "math/Vec2.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

USING_NS_CC;
class QtProperty;
class QCGLWidget;
class SceneCtrl;

class EditorMain : public QMainWindow
{
	Q_OBJECT

public:
	EditorMain(QWidget *parent = 0);
	~EditorMain();

private slots:
	void valueChanged(QtProperty *, const QVariant &);
	void boxPositionChanged(const Vec2& pos);
	void viewBoxAttr(Node*);
	void save();
	void load();

public:
	void onStart();

protected:
	virtual void closeEvent(QCloseEvent *) override;


private:
	void createActions();

	Ui::EditorMainClass ui;
	BoxList* _boxlist;
	QCGLWidget* _glwindow;
	SceneCtrl* _sceneCtrl;

	QtVariantPropertyManager* _variantManager;
	QtTreePropertyBrowser* _variantEditor;


	QAction* _saveAction;
	QAction* _loadAction;
};

#endif // EDITORMAIN_H
