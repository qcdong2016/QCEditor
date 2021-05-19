#pragma once

#include "qtcocos2d.h"
#include "WindowBox.h"

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <functional>

class QTimer;
class SceneCtrl;

USING_NS_CC;

class QCGLWidget : public QOpenGLWidget, public cocos2d::GLViewImpl
{
    Q_OBJECT

public:
	QCGLWidget(QWidget* parent = nullptr);
    ~QCGLWidget();
    
    SceneCtrl* getScene() { return _sceneCtrl; }
    
    std::function<void()> afterInit;
    
public:
    virtual void end() override {}
    virtual bool isOpenGLReady() override
    {
        return true;
    }
    virtual void swapBuffers() override
    {
        /* Swap buffers */
        //this->makeCurrent();
        //this->swapBuffers();
    }
    virtual void setIMEKeyboardState(bool open) override {}
    virtual void setFrameSize(float width, float height) override
    {
        GLView::setFrameSize(width, height);
    }
    virtual void pollEvents() override {}
    void         setRetinaFactor(const float fac)
    {
        _retinaFactor = fac;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    HWND getWin32Window() { return nullptr; }
#endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) */
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    
    virtual void mouseMoveEvent(QMouseEvent *event)override;
    virtual void mousePressEvent(QMouseEvent *event)override;
    virtual void mouseReleaseEvent(QMouseEvent *event)override;

	virtual void wheelEvent(QWheelEvent *event)override;
	virtual void keyPressEvent(QKeyEvent *event)override;
	virtual void keyReleaseEvent(QKeyEvent *event)override;

	QTimer* _timer;
	SceneCtrl* _sceneCtrl;
};

