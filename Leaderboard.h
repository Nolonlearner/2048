#ifndef __LEADERBOARD_H__
#define __LEADERBOARD_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
class ScoreEntry : public cocos2d::Ref
{
public:
    ScoreEntry(const cocos2d::String& playerName, int score);

    // 获取玩家名字
    //const std::string& getPlayerName() const;
    const cocos2d::String& getPlayerName() const { return playerName; }

    // 获取玩家分数
    int getScore() const { return score; };

    // 创建ScoreEntry实例
    static ScoreEntry* create(const cocos2d::String& playerName, int score);

private:
    //std::string playerName;
    cocos2d::String playerName;
    int score;
};

class Leaderboard : public cocos2d::Node
{
public:
    // 创建Leaderboard实例
    static Leaderboard* create();
    // 显示排行榜
    void showLeaderboard();
    void backButtonCallback(Ref* sender);
    // 构造函数
    Leaderboard();
    cocos2d::ui::Button* backButton;
    // 析构函数
    ~Leaderboard();
    int getTopScore() const { return TopScore; }
    void insertScore(const cocos2d::String& playerName, int score);//在排行榜中插入新元素

private:
    // 从文件加载排行榜信息
    void loadScoresFromFile();

    // 保存排行榜信息到文件
    void saveScoresToFile();
    // 排行榜数据
    cocos2d::Vector<ScoreEntry*> scores;
    int TopScore;
};

#endif // __LEADERBOARD_H__