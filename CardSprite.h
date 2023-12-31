#ifndef __CARD_SPRITE_H__
#define __CARD_SPRITE_H__

#include "cocos2d.h"

class CardSprite : public cocos2d::Sprite
{
public:
    enum class TileType
    {
        Empty,//��
        Number//����������
    };

    static CardSprite* create();
    static CardSprite* createInitialCard(int Val);  // ��������
    bool init() override;

    // ���úͻ�ȡ Tile ����
    void setTileTypeAndValue(TileType type, int value);
    TileType getTileType() const;
    int getValue() const;
    // ���úͻ�ȡ����
    void setCoordinates(int row, int col);
    int getRow() const;
    int getCol() const;

private:
    TileType tileType;
    cocos2d::Size TileSize;  // ���̸��Ӵ�С
    int value;
    int row;
    int col;

    // �������������˽�з���
    void updateTexture();
};

#endif // __TILE_H__

