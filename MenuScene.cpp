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
//��δ����������ڼ�����Դ�ļ�ʱ��������ʱ���������Ϣ��
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

    // ���´��ڴ�С
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


    // ������ʼ��Ϸ�˵���
    auto startGameItem = MenuItemLabel::create(Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", MENUFONTSIZE),
        CC_CALLBACK_1(MenuScene::StartGameCallback, this));
    startGameItem->setColor(Color3B::BLACK);

    // �����鿴���а�˵���
    auto viewLeaderboardItem = MenuItemLabel::create(Label::createWithTTF("View Leaderboard", "fonts/Marker Felt.ttf", MENUFONTSIZE),
        CC_CALLBACK_1(MenuScene::ViewLeaderboardCallback, this));
    viewLeaderboardItem->setColor(Color3B::BLACK);

    // �����û����˵���
    auto inputNameItem = MenuItemLabel::create(Label::createWithTTF("inputName", "fonts/Marker Felt.ttf", MENUFONTSIZE),
        CC_CALLBACK_1(MenuScene::InputNameCallback, this));
    inputNameItem->setColor(Color3B::BLACK);
    inputNameItem->setName("inputNameItem");
    // ����������Ϸ�˵���
    auto overGameItem = MenuItemLabel::create(Label::createWithTTF("Over Game", "fonts/Marker Felt.ttf", MENUFONTSIZE),
        CC_CALLBACK_1(MenuScene::OverGameCallback, this));
    overGameItem->setColor(Color3B::BLACK);

    // �����˵�
    // ����һ���������������в˵�����ӵ�������
    try {
        // ����һ���������������в˵�����ӵ�������
        Vector<MenuItem*> menuItems;
        menuItems.pushBack(startGameItem);
        menuItems.pushBack(viewLeaderboardItem);
        menuItems.pushBack(inputNameItem);
        menuItems.pushBack(overGameItem);

        // ʹ��createWithArray�����˵�
        auto menu = Menu::createWithArray(menuItems);

        // �Բ˵������쳣����
        if (menu) {
            menu->alignItemsVerticallyWithPadding(MENUFONTSIZE);  // ���ò˵���֮��ļ��Ϊ 20
            menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - MENUFONTSIZE ));
            this->addChild(menu, 2);
        }
        else {
            // �׳��쳣���ɸ�����Ҫ�����쳣����
            throw std::runtime_error("Failed to create menu.");
        }
    }
    catch (const std::exception& e) {
        // �����쳣������
        log("Exception caught: %s", e.what());
    }

    //username

    UserName = "xxx";
    log("myname is %s", UserName.getCString()); // ʹ�� getCString() ��ȡ�ַ����� C ����ʾ
    userNameLabel = Label::createWithTTF(StringUtils::format("User: %s", UserName.getCString()), "fonts/Marker Felt.ttf", MENUFONTSIZE);
    userNameLabel->setColor(Color3B::RED);
    userNameLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + MENUFONTSIZE));
    this->addChild(userNameLabel, 2, "userNameLabel");

    // ��ӱ���ͼƬ
    auto background = Sprite::create("background.png");
    background->setPosition(visibleSize / 2);
    this->addChild(background, -1);

    return true;
}


// ������Ϸ
//a selector callback
void MenuScene::OverGameCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    String txt = "Over";
    log("%s", txt);
    Director::getInstance()->end();//����

}
void MenuScene::StartGameCallback(Ref* sender)
{
    // ��ʼ��Ϸ���߼��������л�����Ϸ����
    Director::getInstance()->replaceScene(GameScene::createScene(UserName));//�����û���
}
void MenuScene::ViewLeaderboardCallback(Ref* sender)
{
   
    log("%s", "viewLeaderboard");
    // �鿴���а��л������а񳡾�
    Leaderboard* leaderboard = Leaderboard::create();
    this->addChild(leaderboard, 2); // ʹ���ʵ��Ĳ㼶
}
void MenuScene::InputNameCallback(Ref* sender)
{
    auto inputNameLayer = InputNameLayer::create();
    this->addChild(inputNameLayer, 3); // ʹ���ʵ��Ĳ㼶
    
}
void MenuScene::UpdateUserName(cocos2d::String newUserName)
{
    UserName = newUserName;
    log("MenuSceneUserName= %s", newUserName.getCString());
    // �Ƴ��ɵ��û�����ǩ
    Vec2 temp = userNameLabel->getPosition();
    this->removeChildByName("userNameLabel");

    // �����µ��û�����ǩ
    userNameLabel = Label::createWithTTF(StringUtils::format("User: %s", UserName.getCString()), "fonts/Marker Felt.ttf", MENUFONTSIZE);
    userNameLabel->setColor(Color3B::RED);
    userNameLabel->setPosition(temp);
    this->addChild(userNameLabel, 2, "userNameLabel");
   
}
