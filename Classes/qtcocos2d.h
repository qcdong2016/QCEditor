#ifndef __QTCOCOS2D_H__
#define __QTCOCOS2D_H__

#if _WIN32

#include "cocos2d.h"
#include "platform/CCFileUtils.h"
using namespace cocos2d;
using namespace cocos2d::ui;

#include "GUI/CCControlExtension/CCControlButton.h"
using namespace cocos2d::extension;
#else

#include <QOpenGLFunctions>

#include "cocos2d.h"
#include "platform/CCFileUtils.h"
using namespace cocos2d;
using namespace cocos2d::ui;

#include "GUI/CCControlExtension/CCControlButton.h"
using namespace cocos2d::extension;

#endif

#endif // __QTCOCOS2D_H__
