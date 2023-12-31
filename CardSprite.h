#ifndef __CARD_SPRITE_H__
#define __CARD_SPRITE_H__

#include "cocos2d.h"

class CardSprite : public cocos2d::Sprite
{
public:
    enum class TileType
    {
        Empty,//空
        Number//代表有数字
    };

    static CardSprite* create();
    static CardSprite* createInitialCard(int Val);  // 新增方法
    bool init() override;

    // 设置和获取 Tile 类型
    void setTileTypeAndValue(TileType type, int value);
    TileType getTileType() const;
    int getValue() const;
    // 设置和获取坐标
    void setCoordinates(int row, int col);
    int getRow() const;
    int getCol() const;

private:
    TileType tileType;
    cocos2d::Size TileSize;  // 棋盘格子大小
    int value;
    int row;
    int col;

    // 用于设置纹理的私有方法
    void updateTexture();
};

#endif // __TILE_H__

