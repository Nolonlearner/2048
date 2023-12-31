#ifndef __LEADERBOARD_H__
#define __LEADERBOARD_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
class ScoreEntry : public cocos2d::Ref
{
public:
    ScoreEntry(const cocos2d::String& playerName, int score);

    // ��ȡ�������
    //const std::string& getPlayerName() const;
    const cocos2d::String& getPlayerName() const { return playerName; }

    // ��ȡ��ҷ���
    int getScore() const { return score; };

    // ����ScoreEntryʵ��
    static ScoreEntry* create(const cocos2d::String& playerName, int score);

private:
    //std::string playerName;
    cocos2d::String playerName;
    int score;
};

class Leaderboard : public cocos2d::Node
{
public:
    // ����Leaderboardʵ��
    static Leaderboard* create();
    // ��ʾ���а�
    void showLeaderboard();
    void backButtonCallback(Ref* sender);
    // ���캯��
    Leaderboard();
    cocos2d::ui::Button* backButton;
    // ��������
    ~Leaderboard();
    int getTopScore() const { return TopScore; }
    void insertScore(const cocos2d::String& playerName, int score);//�����а��в�����Ԫ��

private:
    // ���ļ��������а���Ϣ
    void loadScoresFromFile();

    // �������а���Ϣ���ļ�
    void saveScoresToFile();
    // ���а�����
    cocos2d::Vector<ScoreEntry*> scores;
    int TopScore;
};

#endif // __LEADERBOARD_H__