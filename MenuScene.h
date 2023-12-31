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

    // 切换到游戏场景的回调函数
    void StartGameCallback(cocos2d::Ref* sender);
    // 结束游戏的回调函数
    void OverGameCallback(cocos2d::Ref* sender);

    // 查看排行榜的回调函数
    void ViewLeaderboardCallback(cocos2d::Ref* sender);

    // 输入用户名的回调函数
    void InputNameCallback(cocos2d::Ref* sender);
    void UpdateUserName(cocos2d::String newUserName);
private:
    cocos2d::Label* userNameLabel;  // 用于显示用户名
    cocos2d::String UserName;
};

#endif // __MENU_SCENE_H__
