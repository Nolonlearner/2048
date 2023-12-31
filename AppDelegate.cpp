/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

 /****************************************************************************
 AppDelegate.h负责初始化游戏引擎和处理应用程序的生命周期事件。
  ****************************************************************************/


#include "AppDelegate.h"
#include "MenuScene.h"//传入菜单场景头文件

  // #define USE_AUDIO_ENGINE 1
  // #define USE_SIMPLE_AUDIO_ENGINE 1
  //定义使用的音频引擎：通过宏定义选择使用 Cocos2d-x 提供的 AudioEngine 或 SimpleAudioEngine。这两者是 Cocos2d-x 提供的音频库，你选择使用其中一个即可。
#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;
//设置设计分辨率和不同分辨率下的显示策略：designResolutionSize 定义了设计分辨率，而在不同分辨率下选择合适的显示策略。
static cocos2d::Size designResolutionSize = cocos2d::Size(880, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

//AppDelegate 构造函数和析构函数：负责初始化和释放音频引擎。
AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
//initGLContextAttrs 方法：设置 OpenGL 上下文属性。
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
//register_all_packages 方法：为包管理器注册所有的包。
static int register_all_packages()
{
    return 0; //flag for packages manager
}

/*applicationDidFinishLaunching 方法：这是应用程序启动后的入口点。它完成了以下工作：
初始化 Director 和 GLView。
显示帧率信息。
设置帧率。
设置设计分辨率和适应策略。
创建 MenuScene场景并运行。*/
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();//获取 Director 实例。
    auto glview = director->getOpenGLView();//获取 OpenGL 视图，即游戏窗口。
    if (!glview) {//如果没有 OpenGL 视图（通常在初始化时没有），则创建一个。
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("2048Game", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("2048Game");
#endif
        director->setOpenGLView(glview);
    }
    /*在 Windows、Mac 和 Linux 平台上，使用 createWithRect 方法创建一个带有矩形边界的 OpenGL 视图，
    以及指定的设计分辨率大小。在其他平台上，使用 create 方法创建一个没有指定大小的 OpenGL 视图。
    然后将这个视图设置给 Director。*/
    // turn on display FPS
    //显示 FPS 和设置帧率：
    //director->setDisplayStats(true);//在游戏窗口的左上角显示帧率
    director->setDisplayStats(false);
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);//设置游戏的帧率为 60 帧每秒。

    // Set the design resolution
    //设置设计分辨率和适应策略：
    //设置设计分辨率大小和适应策略为 ResolutionPolicy::NO_BORDER。这里 NO_BORDER 表示当屏幕长宽比与设计分辨率的长宽比不同时，保持内容不变形并在屏幕上居中显示，可能会有一些内容被裁剪。
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();//：获取当前窗口的大小。 根据窗口大小和不同的分辨率，调整游戏内容的缩放因子，以适应不同尺寸的屏幕。

    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {
        director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {
        director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {
        director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
    }

    register_all_packages();//用于注册所有的包，这通常是用于处理一些游戏中的扩展或第三方库。

    // create a scene. it's an autorelease object
    //替换成MenuScene
    //auto scene = HelloWorld::createScene();
    auto scene = MenuScene::createScene();
    // run
    director->runWithScene(scene);

    return true;
}
//applicationDidEnterBackground 和 applicationWillEnterForeground 方法：处理应用程序进入后台和前台时的暂停和恢复事件。
// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}