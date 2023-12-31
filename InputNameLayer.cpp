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
    // �����ı������
    textField = cocos2d::ui::TextField::create("Click Here", "fonts/arial.ttf", 30);
    textField->setColor(Color3B::WHITE);
    textField->setTextColor(Color4B::WHITE);
    textField->setMaxLength(10);
    textField->setName("textField");
    textField->setPosition(Vec2(100, 90));
    textField->addEventListener(CC_CALLBACK_2(InputNameLayer::onTextFieldEvent, this));
    this->addChild(textField);

    // ��ӹرհ�ť
    auto closeButton = cocos2d::ui::Button::create("CloseButton.png");
    closeButton->setPosition(Vec2(180, 200));
    closeButton->addClickEventListener(CC_CALLBACK_1(InputNameLayer::onCloseButtonClicked, this));
    this->addChild(closeButton);

    // ��ȡInputNameLayer�Ĵ�С
    Size layerSize = this->getContentSize();

    // ��ӵײ�ͼƬ
    auto bottomImage = Sprite::create("bottomImage.png");
    bottomImage->setAnchorPoint(Vec2(0.5, 0));
    bottomImage->setPosition(Vec2(layerSize.width / 2, 0));
    bottomImage->setContentSize(layerSize);  // ���õײ�ͼƬ�Ĵ�СΪInputNameLayer�Ĵ�С
    this->addChild(bottomImage, -1);

    return true;
}

void InputNameLayer::onCloseButtonClicked(cocos2d::Ref* sender)
{
    // �ر�Layer
    this->removeFromParent();
}
void InputNameLayer::UpdateUserName(const cocos2d::String& newUserName)
{
    // �����û���
    UserName = newUserName;
    log("new name: %s", UserName.getCString());
   
}
void InputNameLayer::onTextFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType event)
{
    // �����ı�������¼�
    auto parent = dynamic_cast<MenuScene*>(this->getParent());
    switch (event)
    {
    case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
        // ���ı��򱻼���ʱ����
        CCLOG("TextField Attach With IME");
        break;
    case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
        // ���ı���ʧȥ����ʱ������������Ϊ�������
        CCLOG("TextField Detach With IME");

        // ��������Ӵ�������������߼�����������û���
        UpdateUserName(static_cast<cocos2d::ui::TextField*>(sender)->getString());
        // ��ȡ���ڵ㲢�ж��Ƿ�Ϊ MenuScene ����

        if (parent) {
            // ֱ�ӵ��ø��ڵ�ĺ���
            log("parent!");
            parent->UpdateUserName(static_cast<cocos2d::ui::TextField*>(sender)->getString());
        }
        // ʹ�� scheduleOnce �ӳ�һִ֡�в���
        this->scheduleOnce([this](float dt) {
            // ���Layer�Ƿ񻹴����ڳ����У��Լ��ı����Ƿ������Layer��
            if (this->getParent() && this->getChildByName("textField")) {
                // �ӳ������Ƴ��ı���
                this->removeChildByName("textField");
                this->removeFromParent();
            }
            }, 0.0f, "delayedRemove");
        /*�����������㰴ť��ֱ�ӵ�InputName�ͱ�������,��Ϊ�� onTextFieldEvent �����У�
        �ı����� DETACH_WITH_IME �¼��б��Ƴ��������¼���������ֳ��Է����Ѿ����Ƴ�����
        �����µġ�����Ҫ�� DETACH_WITH_IME �¼�������Ϻ��ӳ�һ֡��ִ����ز�����*/
        break;
    case cocos2d::ui::TextField::EventType::INSERT_TEXT:
        // ���ı�������ı�ʱ����
        CCLOG("TextField Insert Text");
        break;
    case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
        // ���ı���ɾ������ʱ����
        CCLOG("TextField Delete Backward");
        break;
    default:
        break;
    }
}
