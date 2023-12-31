#include "CardSprite.h"

USING_NS_CC;
CardSprite* CardSprite::create()
{
    CardSprite* cardsprite = new (std::nothrow) CardSprite();
    if (cardsprite && cardsprite->init())
    {
        cardsprite->autorelease();//设置为自动回收
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
        // 设置初始值为 Val 的卡片属性
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

    // 初始化默认属性
    tileType = TileType::Empty;
    value = 0;
    row = 0;
    col = 0;

    // 设置默认显示
    setContentSize(TileSize);
    updateTexture(); // 初始化时更新纹理

    return true;
}

void CardSprite::setTileTypeAndValue(TileType type,int val)
{
    tileType = type;
    value = val;
    updateTexture(); // 设置类型时更新纹理
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
        // 根据不同的值设置不同的纹理
        std::string textureName = "number" + std::to_string(value) + ".png";
        setTexture(textureName);
    }
}