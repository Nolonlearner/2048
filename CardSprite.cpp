#include "CardSprite.h"

USING_NS_CC;
CardSprite* CardSprite::create()
{
    CardSprite* cardsprite = new (std::nothrow) CardSprite();
    if (cardsprite && cardsprite->init())
    {
        cardsprite->autorelease();//����Ϊ�Զ�����
        return cardsprite;
    }
    CC_SAFE_DELETE(cardsprite);
    return nullptr;
}

CardSprite* CardSprite::createInitialCard(int Val)
{
    CardSprite* initialCard = create();
    if (initialCard)
    {
        // ���ó�ʼֵΪ Val �Ŀ�Ƭ����
        initialCard->setTileTypeAndValue(TileType::Number, Val);
    }
    return initialCard;
}
bool CardSprite::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // ��ʼ��Ĭ������
    tileType = TileType::Empty;
    value = 0;
    row = 0;
    col = 0;

    // ����Ĭ����ʾ
    setContentSize(TileSize);
    updateTexture(); // ��ʼ��ʱ��������

    return true;
}

void CardSprite::setTileTypeAndValue(TileType type,int val)
{
    tileType = type;
    value = val;
    updateTexture(); // ��������ʱ��������
}

CardSprite::TileType CardSprite::getTileType() const
{
    return tileType;
}


int CardSprite::getValue() const
{
    return value;
}

void CardSprite::setCoordinates(int row, int col)
{
    this->row = row;
    this->col = col;
}

int CardSprite::getRow() const
{
    return row;
}

int CardSprite::getCol() const
{
    return col;
}

void CardSprite::updateTexture()
{
    if (tileType == TileType::Empty)
    {
        value = 0;
        setTexture("Empty.png");
    }
    else if (tileType == TileType::Number)
    {
        // ���ݲ�ͬ��ֵ���ò�ͬ������
        std::string textureName = "number" + std::to_string(value) + ".png";
        setTexture(textureName);
    }
}