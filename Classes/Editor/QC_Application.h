#pragma once

#include "cocos2d.h"
#include <QApplication>

USING_NS_CC;

class QCApplication : public QApplication, public cocos2d::Application
{
	Q_OBJECT

public:
	QCApplication(int argc, char *argv[]) : QApplication(argc, argv) {}
	virtual ~QCApplication() {}

	virtual void setAnimationInterval(float interval) override {} 
	virtual bool applicationDidFinishLaunching() override { return true; }
	virtual void applicationDidEnterBackground() override {}
	virtual void applicationWillEnterForeground() override {}
};


