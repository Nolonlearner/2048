#include "InputNameLayer.h"
#include "MenuScene.h"
USING_NS_CC;

bool InputNameLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    this->setContentSize(Size(200, 200));
    this->setPosition(Vec2(100, 100));
    // 创建文本输入框
    textField = cocos2d::ui::TextField::create("Click Here", "fonts/arial.ttf", 30);
    textField->setColor(Color3B::WHITE);
    textField->setTextColor(Color4B::WHITE);
    textField->setMaxLength(10);
    textField->setName("textField");
    textField->setPosition(Vec2(100, 90));
    textField->addEventListener(CC_CALLBACK_2(InputNameLayer::onTextFieldEvent, this));
    this->addChild(textField);

    // 添加关闭按钮
    auto closeButton = cocos2d::ui::Button::create("CloseButton.png");
    closeButton->setPosition(Vec2(180, 200));
    closeButton->addClickEventListener(CC_CALLBACK_1(InputNameLayer::onCloseButtonClicked, this));
    this->addChild(closeButton);

    // 获取InputNameLayer的大小
    Size layerSize = this->getContentSize();

    // 添加底部图片
    auto bottomImage = Sprite::create("bottomImage.png");
    bottomImage->setAnchorPoint(Vec2(0.5, 0));
    bottomImage->setPosition(Vec2(layerSize.width / 2, 0));
    bottomImage->setContentSize(layerSize);  // 设置底部图片的大小为InputNameLayer的大小
    this->addChild(bottomImage, -1);

    return true;
}

void InputNameLayer::onCloseButtonClicked(cocos2d::Ref* sender)
{
    // 关闭Layer
    this->removeFromParent();
}
void InputNameLayer::UpdateUserName(const cocos2d::String& newUserName)
{
    // 更新用户名
    UserName = newUserName;
    log("new name: %s", UserName.getCString());
   
}
void InputNameLayer::onTextFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType event)
{
    // 处理文本输入框事件
    auto parent = dynamic_cast<MenuScene*>(this->getParent());
    switch (event)
    {
    case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
        // 当文本框被激活时触发
        CCLOG("TextField Attach With IME");
        break;
    case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
        // 当文本框失去焦点时触发，可以视为输入结束
        CCLOG("TextField Detach With IME");

        // 在这里添加处理输入结束的逻辑，例如更新用户名
        UpdateUserName(static_cast<cocos2d::ui::TextField*>(sender)->getString());
        // 获取父节点并判断是否为 MenuScene 类型

        if (parent) {
            // 直接调用父节点的函数
            log("parent!");
            parent->UpdateUserName(static_cast<cocos2d::ui::TextField*>(sender)->getString());
        }
        // 使用 scheduleOnce 延迟一帧执行操作
        this->scheduleOnce([this](float dt) {
            // 检查Layer是否还存在于场景中，以及文本框是否存在于Layer中
            if (this->getParent() && this->getChildByName("textField")) {
                // 从场景中移除文本框
                this->removeChildByName("textField");
                this->removeFromParent();
            }
            }, 0.0f, "delayedRemove");
        /*输入后如果不点按钮，直接点InputName就报错弹出了,因为在 onTextFieldEvent 函数中，
        文本框在 DETACH_WITH_IME 事件中被移除，而在事件处理完后又尝试访问已经被移除的文
        本框导致的。所以要在 DETACH_WITH_IME 事件处理完毕后延迟一帧再执行相关操作。*/
        break;
    case cocos2d::ui::TextField::EventType::INSERT_TEXT:
        // 当文本框插入文本时触发
        CCLOG("TextField Insert Text");
        break;
    case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
        // 当文本框删除文字时触发
        CCLOG("TextField Delete Backward");
        break;
    default:
        break;
    }
}
