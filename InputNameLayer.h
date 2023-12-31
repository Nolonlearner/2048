#include "cocos2d.h"
#include "ui/CocosGUI.h"
class InputNameLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(InputNameLayer);
    const cocos2d::String& getUserName() const { return UserName; }
    void UpdateUserName(const cocos2d::String& newUserName);
    

private:
    void onCloseButtonClicked(cocos2d::Ref* sender);
    void onTextFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType event);
    cocos2d::String UserName = "xxx";
    cocos2d::ui::TextField* textField;
};

