#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "Leaderboard.h"
#include "InputNameLayer.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define MENUFONTSIZE 60
#define MENUHEIGH 860
#define MENUWIDTH 860
static cocos2d::Size MenuSize = cocos2d::Size(MENUHEIGH, MENUWIDTH);

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}
// Print useful error message instead of segfaulting when files are not there.
//这段代码是用于在加载资源文件时出现问题时输出错误信息，
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool MenuScene::init()
{
    //////////////////////////////
  // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    // 更新窗口大小
    Director::getInstance()->getOpenGLView()->setFrameSize(MenuSize.width, MenuSize.height);
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

  
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("2048", "fonts/Marker Felt.ttf", MENUFONTSIZE*2);
    label->setColor(Color3B::BLACK);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }


    // 创建开始游戏菜单项
    auto startGameItem = MenuItemLabel::create(Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", MENUFONTSIZE),
        CC_CALLBACK_1(MenuScene::StartGameCallback, this));
    startGameItem->setColor(Color3B::BLACK);

    // 创建查看排行榜菜单项
    auto viewLeaderboardItem = MenuItemLabel::create(Label::createWithTTF("View Leaderboard", "fonts/Marker Felt.ttf", MENUFONTSIZE),
        CC_CALLBACK_1(MenuScene::ViewLeaderboardCallback, this));
    viewLeaderboardItem->setColor(Color3B::BLACK);

    // 输入用户名菜单项
    auto inputNameItem = MenuItemLabel::create(Label::createWithTTF("inputName", "fonts/Marker Felt.ttf", MENUFONTSIZE),
        CC_CALLBACK_1(MenuScene::InputNameCallback, this));
    inputNameItem->setColor(Color3B::BLACK);
    inputNameItem->setName("inputNameItem");
    // 创建结束游戏菜单项
    auto overGameItem = MenuItemLabel::create(Label::createWithTTF("Over Game", "fonts/Marker Felt.ttf", MENUFONTSIZE),
        CC_CALLBACK_1(MenuScene::OverGameCallback, this));
    overGameItem->setColor(Color3B::BLACK);

    // 创建菜单
    // 创建一个向量，并将所有菜单项添加到向量中
    try {
        // 创建一个向量，并将所有菜单项添加到向量中
        Vector<MenuItem*> menuItems;
        menuItems.pushBack(startGameItem);
        menuItems.pushBack(viewLeaderboardItem);
        menuItems.pushBack(inputNameItem);
        menuItems.pushBack(overGameItem);

        // 使用createWithArray创建菜单
        auto menu = Menu::createWithArray(menuItems);

        // 对菜单进行异常处理
        if (menu) {
            menu->alignItemsVerticallyWithPadding(MENUFONTSIZE);  // 设置菜单项之间的间隔为 20
            menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - MENUFONTSIZE ));
            this->addChild(menu, 2);
        }
        else {
            // 抛出异常，可根据需要定义异常类型
            throw std::runtime_error("Failed to create menu.");
        }
    }
    catch (const std::exception& e) {
        // 捕获异常并处理
        log("Exception caught: %s", e.what());
    }

    //username

    UserName = "xxx";
    log("myname is %s", UserName.getCString()); // 使用 getCString() 获取字符串的 C 风格表示
    userNameLabel = Label::createWithTTF(StringUtils::format("User: %s", UserName.getCString()), "fonts/Marker Felt.ttf", MENUFONTSIZE);
    userNameLabel->setColor(Color3B::RED);
    userNameLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + MENUFONTSIZE));
    this->addChild(userNameLabel, 2, "userNameLabel");

    // 添加背景图片
    auto background = Sprite::create("background.png");
    background->setPosition(visibleSize / 2);
    this->addChild(background, -1);

    return true;
}


// 结束游戏
//a selector callback
void MenuScene::OverGameCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    String txt = "Over";
    log("%s", txt);
    Director::getInstance()->end();//结束

}
void MenuScene::StartGameCallback(Ref* sender)
{
    // 开始游戏的逻辑，可以切换到游戏场景
    Director::getInstance()->replaceScene(GameScene::createScene(UserName));//传入用户名
}
void MenuScene::ViewLeaderboardCallback(Ref* sender)
{
   
    log("%s", "viewLeaderboard");
    // 查看排行榜，切换到排行榜场景
    Leaderboard* leaderboard = Leaderboard::create();
    this->addChild(leaderboard, 2); // 使用适当的层级
}
void MenuScene::InputNameCallback(Ref* sender)
{
    auto inputNameLayer = InputNameLayer::create();
    this->addChild(inputNameLayer, 3); // 使用适当的层级
    
}
void MenuScene::UpdateUserName(cocos2d::String newUserName)
{
    UserName = newUserName;
    log("MenuSceneUserName= %s", newUserName.getCString());
    // 移除旧的用户名标签
    Vec2 temp = userNameLabel->getPosition();
    this->removeChildByName("userNameLabel");

    // 创建新的用户名标签
    userNameLabel = Label::createWithTTF(StringUtils::format("User: %s", UserName.getCString()), "fonts/Marker Felt.ttf", MENUFONTSIZE);
    userNameLabel->setColor(Color3B::RED);
    userNameLabel->setPosition(temp);
    this->addChild(userNameLabel, 2, "userNameLabel");
   
}
