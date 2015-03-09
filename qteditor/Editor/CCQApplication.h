#pragma once

#include "cocos2d.h"
#include <QApplication>

USING_NS_CC;

class CCQApplication : public QApplication, public cocos2d::Application
{
	Q_OBJECT
public:
	CCQApplication(int argc, char *argv[]);
	virtual ~CCQApplication();

	int run();

	static CCQApplication* getInstance();

	virtual void setAnimationInterval(double interval) override;
	virtual bool applicationDidFinishLaunching() override;
	virtual void applicationDidEnterBackground() override {}
	virtual void applicationWillEnterForeground() override {}

protected:
	float _qAnimationInterval;
    static CCQApplication * sm_pSharedApplication;

private:
};


