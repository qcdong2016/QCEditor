#pragma once

#include "cocos2d.h"
#include <QApplication>
#include "EditorMain.h"

USING_NS_CC;

class QCGLWidget;
class TaskManager;
class QTimer;

class ACApplication : public QApplication, public cocos2d::Application
{
	Q_OBJECT
public:
	ACApplication(int argc, char *argv[]);
	virtual ~ACApplication();

	int run();

	static ACApplication* getInstance();

	virtual void setAnimationInterval(double interval) override;
	virtual bool applicationDidFinishLaunching() override;
	virtual void applicationDidEnterBackground() override {}
	virtual void applicationWillEnterForeground() override {}

protected:
	float _qAnimationInterval;
    static ACApplication * sm_pSharedApplication;

private:

	QCGLWidget* _glwindow;
	TaskManager* _loadingTasks;
	QTimer* _taskTimer;

	EditorMain _mainwindow;
};


