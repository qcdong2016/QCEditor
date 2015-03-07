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

	/* Run the message loop. */
	int run();

	/* Get current application instance. */
	static CCQApplication* getInstance();

	/* override functions */
	virtual void setAnimationInterval(double interval) override;
	virtual bool applicationDidFinishLaunching();
	virtual void applicationDidEnterBackground() {}
	virtual void applicationWillEnterForeground() {}

protected:
	float _qAnimationInterval;
    static CCQApplication * sm_pSharedApplication;

private:
};


