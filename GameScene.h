//��Ϸ�ĺ����߼�
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "CardSprite.h"
#include "ui/CocosGUI.h"
class GameScene : public cocos2d::Scene
{
public:
    // ����ö������
    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createScene(cocos2d::String UserName);
    virtual bool init();

    // implement the "static create()" method manually

    CREATE_FUNC(GameScene);

    // ��Ϸ��ʼ��
    void initGame();
    void initMusic();
    void initItem();
    void updateUI();
    void spawnTile(int NewSum = 1); // �����µķ���
    void mergeTiles(CardSprite*& CurrentCard, CardSprite*& TargetCard); // �ϲ���������
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event); // �����û����룬������������������
    void moveTiles(Direction direction);
    void ExchangeTile(int RowA, int RowB, int ColA, int ColB);
    bool MoveUp();
    bool MoveDown();
    bool MoveLeft();
    bool MoveRight();
    void createChessBoard();//��������
    void calScore(int newScore);
    void printAll();
    bool checkGameOver();
    bool checkGameWin();
    // �л������˵��Ļص�����
    void TurnMenuCallback(cocos2d::Ref* sender);
    // ��Ⱦ��Ϸ��Ϣ����
    void renderGameInfo();
    //��Ϸ�����Ƿ����²��֣�����������ʾ��ǰ������Ŀǰ��¼��߷֣��ұ���ʾ��Ϸ����Ĳ˵���Ŀ��
    // ��������м��������ɣ����Խ��б�����Ϸ���ȣ��˳���Ϸ��������Ϸ�Ȱ���
    // ��Ϸ�˵�
    void resumeGame(Ref* pSender); // ���¿�ʼ��Ϸ
    void updateTimeLabel();
    void clearGrid();

    void showGameOver();
    void showGameWin();


private:
    // �洢��Ϸ�еķ���
    cocos2d::Sprite* gameInfoSprite;  // ��Ϸ��Ϣ����
    cocos2d::Sprite* chessBoardSprite;  // ���̲�
    const int boardSize = 4; // ���̴�С��4x4

    CardSprite* gridShow[4][4]; //ר��Ūһ�������㿨Ƭʵ�ֶ�λ�����֡��ƶ�������ϵ�ж���//δ���
    CardSprite* grid[4][4];

    int cardPadding;  // ���̸��Ӽ��
    int indentation;  // ������������
    int currentScore;  // ��ǰ����
    int bestScore;  // ��߷�
    time_t gameTimeSeconds;  // ��ǰʱ�䣨�룩
    cocos2d::Label* currentScoreLabel;  // ������ʾ��ǰ������ Label
    cocos2d::Label* timeLabel;  // ������ʾ��Ϸʱ��� Label
    cocos2d::Label* bestScoreLabel;//���ڱ�ʾ��߷ֵĵ�label
    cocos2d::Label* titleLabel;//���ڱ�ʾ��Ϸ���ֵ�
    cocos2d::Label* UserNameLabel;//���ڱ�ʾ������ֵ�Label
    int fullTileSum;
    bool gameEndShown;
};

#endif // __GAME_SCENE_H__
