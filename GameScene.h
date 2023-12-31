//游戏的核心逻辑
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "CardSprite.h"
#include "ui/CocosGUI.h"
class GameScene : public cocos2d::Scene
{
public:
    // 方向枚举类型
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

    // 游戏初始化
    void initGame();
    void initMusic();
    void initItem();
    void updateUI();
    void spawnTile(int NewSum = 1); // 生成新的方块
    void mergeTiles(CardSprite*& CurrentCard, CardSprite*& TargetCard); // 合并两个方块
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event); // 处理用户输入，即按动键盘上下左右
    void moveTiles(Direction direction);
    void ExchangeTile(int RowA, int RowB, int ColA, int ColB);
    bool MoveUp();
    bool MoveDown();
    bool MoveLeft();
    bool MoveRight();
    void createChessBoard();//创造棋盘
    void calScore(int newScore);
    void printAll();
    bool checkGameOver();
    bool checkGameWin();
    // 切换到主菜单的回调函数
    void TurnMenuCallback(cocos2d::Ref* sender);
    // 渲染游戏信息窗口
    void renderGameInfo();
    //游戏界面是分上下部分，上面的左边显示当前分数和目前记录最高分，右边显示游戏画面的菜单项目，
    // 点击后在中间区域生成，可以进行保存游戏进度，退出游戏、继续游戏等帮助
    // 游戏菜单
    void resumeGame(Ref* pSender); // 重新开始游戏
    void updateTimeLabel();
    void clearGrid();

    void showGameOver();
    void showGameWin();


private:
    // 存储游戏中的方块
    cocos2d::Sprite* gameInfoSprite;  // 游戏信息窗口
    cocos2d::Sprite* chessBoardSprite;  // 棋盘层
    const int boardSize = 4; // 棋盘大小，4x4

    CardSprite* gridShow[4][4]; //专门弄一个动画层卡片实现定位、显现、移动、隐藏系列动画//未完成
    CardSprite* grid[4][4];

    int cardPadding;  // 棋盘格子间隔
    int indentation;  // 棋盘左右缩进
    int currentScore;  // 当前分数
    int bestScore;  // 最高分
    time_t gameTimeSeconds;  // 当前时间（秒）
    cocos2d::Label* currentScoreLabel;  // 用于显示当前分数的 Label
    cocos2d::Label* timeLabel;  // 用于显示游戏时间的 Label
    cocos2d::Label* bestScoreLabel;//用于表示最高分的的label
    cocos2d::Label* titleLabel;//用于表示游戏名字的
    cocos2d::Label* UserNameLabel;//用于表示玩家名字的Label
    int fullTileSum;
    bool gameEndShown;
};

#endif // __GAME_SCENE_H__
