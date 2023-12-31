#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"  
class MenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);

    // �л�����Ϸ�����Ļص�����
    void StartGameCallback(cocos2d::Ref* sender);
    // ������Ϸ�Ļص�����
    void OverGameCallback(cocos2d::Ref* sender);

    // �鿴���а�Ļص�����
    void ViewLeaderboardCallback(cocos2d::Ref* sender);

    // �����û����Ļص�����
    void InputNameCallback(cocos2d::Ref* sender);
    void UpdateUserName(cocos2d::String newUserName);
private:
    cocos2d::Label* userNameLabel;  // ������ʾ�û���
    cocos2d::String UserName;
};

#endif // __MENU_SCENE_H__
