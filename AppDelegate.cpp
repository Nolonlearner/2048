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
 AppDelegate.h�����ʼ����Ϸ����ʹ���Ӧ�ó�������������¼���
  ****************************************************************************/


#include "AppDelegate.h"
#include "MenuScene.h"//����˵�����ͷ�ļ�

  // #define USE_AUDIO_ENGINE 1
  // #define USE_SIMPLE_AUDIO_ENGINE 1
  //����ʹ�õ���Ƶ���棺ͨ���궨��ѡ��ʹ�� Cocos2d-x �ṩ�� AudioEngine �� SimpleAudioEngine���������� Cocos2d-x �ṩ����Ƶ�⣬��ѡ��ʹ������һ�����ɡ�
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
//������Ʒֱ��ʺͲ�ͬ�ֱ����µ���ʾ���ԣ�designResolutionSize ��������Ʒֱ��ʣ����ڲ�ͬ�ֱ�����ѡ����ʵ���ʾ���ԡ�
static cocos2d::Size designResolutionSize = cocos2d::Size(880, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

//AppDelegate ���캯�������������������ʼ�����ͷ���Ƶ���档
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
//initGLContextAttrs ���������� OpenGL ���������ԡ�
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
//register_all_packages ������Ϊ��������ע�����еİ���
static int register_all_packages()
{
    return 0; //flag for packages manager
}

/*applicationDidFinishLaunching ����������Ӧ�ó������������ڵ㡣����������¹�����
��ʼ�� Director �� GLView��
��ʾ֡����Ϣ��
����֡�ʡ�
������Ʒֱ��ʺ���Ӧ���ԡ�
���� MenuScene���������С�*/
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();//��ȡ Director ʵ����
    auto glview = director->getOpenGLView();//��ȡ OpenGL ��ͼ������Ϸ���ڡ�
    if (!glview) {//���û�� OpenGL ��ͼ��ͨ���ڳ�ʼ��ʱû�У����򴴽�һ����
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("2048Game", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("2048Game");
#endif
        director->setOpenGLView(glview);
    }
    /*�� Windows��Mac �� Linux ƽ̨�ϣ�ʹ�� createWithRect ��������һ�����о��α߽�� OpenGL ��ͼ��
    �Լ�ָ������Ʒֱ��ʴ�С��������ƽ̨�ϣ�ʹ�� create ��������һ��û��ָ����С�� OpenGL ��ͼ��
    Ȼ�������ͼ���ø� Director��*/
    // turn on display FPS
    //��ʾ FPS ������֡�ʣ�
    //director->setDisplayStats(true);//����Ϸ���ڵ����Ͻ���ʾ֡��
    director->setDisplayStats(false);
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);//������Ϸ��֡��Ϊ 60 ֡ÿ�롣

    // Set the design resolution
    //������Ʒֱ��ʺ���Ӧ���ԣ�
    //������Ʒֱ��ʴ�С����Ӧ����Ϊ ResolutionPolicy::NO_BORDER������ NO_BORDER ��ʾ����Ļ���������Ʒֱ��ʵĳ���Ȳ�ͬʱ���������ݲ����β�����Ļ�Ͼ�����ʾ�����ܻ���һЩ���ݱ��ü���
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();//����ȡ��ǰ���ڵĴ�С�� ���ݴ��ڴ�С�Ͳ�ͬ�ķֱ��ʣ�������Ϸ���ݵ��������ӣ�����Ӧ��ͬ�ߴ����Ļ��

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

    register_all_packages();//����ע�����еİ�����ͨ�������ڴ���һЩ��Ϸ�е���չ��������⡣

    // create a scene. it's an autorelease object
    //�滻��MenuScene
    //auto scene = HelloWorld::createScene();
    auto scene = MenuScene::createScene();
    // run
    director->runWithScene(scene);

    return true;
}
//applicationDidEnterBackground �� applicationWillEnterForeground ����������Ӧ�ó�������̨��ǰ̨ʱ����ͣ�ͻָ��¼���
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