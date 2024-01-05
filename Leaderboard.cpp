#include "Leaderboard.h"

USING_NS_CC;

ScoreEntry::ScoreEntry(const cocos2d::String& playerName, int score)
    : playerName(playerName), score(score) {}


ScoreEntry* ScoreEntry::create(const cocos2d::String& playerName, int score)
{
    ScoreEntry* entry = new (std::nothrow) ScoreEntry(playerName, score);
    if (entry)
    {
        entry->autorelease();
        return entry;
    }
    CC_SAFE_DELETE(entry);
    return nullptr;
}

Leaderboard::Leaderboard()
{
    // �ڹ��캯���м������а���Ϣ
    log("initLeaderboard");

    // ��ʼ�����ذ�ť
    backButton = cocos2d::ui::Button::create("LeaderBoardCloseNormal.png", "LeaderBoardCloseSelected.png");
    backButton->setPosition(Vec2(400, 480));  // ���÷��ذ�ť��λ��
    backButton->addClickEventListener(CC_CALLBACK_1(Leaderboard::backButtonCallback, this));
    backButton->setName("backButton");
    addChild(backButton, 4);
    TopScore = 0;
    // �������а���Ϣ
    loadScoresFromFile();

    // ��ʾ���а�
    showLeaderboard();

}

Leaderboard::~Leaderboard()
{
    // �����������б������а���Ϣ���ļ�
    saveScoresToFile();

    // �ͷ����а���Ϣ���ڴ�
    scores.clear();
}

Leaderboard* Leaderboard::create()
{
    Leaderboard* leaderboard = new (std::nothrow) Leaderboard();
    if (leaderboard)
    {
        leaderboard->autorelease();
        return leaderboard;
    }
    CC_SAFE_DELETE(leaderboard);
    return nullptr;
}

void Leaderboard::backButtonCallback(Ref* sender)
{
    // ���ذ�ť���߼��������л�������������ִ����������
    // ����������л������˵�������ִ�������߼�
    log("Back Button Clicked");
    this->removeChildByName("leaderboardLayer");
    this->removeChildByName("backButton");
}
void Leaderboard::loadScoresFromFile()
{
    // ���ļ��������а���Ϣ
    std::string filePath = FileUtils::getInstance()->fullPathForFilename("leaderboard.txt");
    log("load File : %s", filePath.c_str());
    // ���ı���ʽ��ȡ�ļ�
    std::string fileContents = FileUtils::getInstance()->getStringFromFile(filePath);

    // ����ȡ�����ı����ݰ��зָ�
    std::vector<std::string> lines;
    std::istringstream stream(fileContents);
    std::string line;
    while (std::getline(stream, line))
    {
        lines.push_back(line);
    }

    // ����ÿһ������
    for (const auto& line : lines)
    {
        std::istringstream lineStream(line);
        std::string playerName;
        int score;
    
        // �ļ���ÿһ�еĸ�ʽ�� "PlayerName Score"
        if (lineStream >> playerName >> score)
        {
            scores.pushBack(ScoreEntry::create(playerName, score));
            if (TopScore < score)
                TopScore = score;
        }
    }
}

void Leaderboard::saveScoresToFile()
{
    
    // ��ȡ�����ļ�����leaderboard.txt������·��
    std::string filePath = cocos2d::FileUtils::getInstance()->fullPathForFilename("leaderboard.txt");
    log("saved File : %s", filePath.c_str());
    // �����а�����ת��Ϊ�ı���ʽ
    std::string fileContents;
    for (const auto& entry : scores)
    {
        fileContents += cocos2d::StringUtils::format("%s %d\n", entry->getPlayerName().getCString(), entry->getScore());
    }

    // ʹ�� FileUtils д���ļ�
    if (cocos2d::FileUtils::getInstance()->writeStringToFile(fileContents, filePath))
    {
        cocos2d::log("File saved successfully: %s", filePath.c_str());
    }
    else
    {
        cocos2d::log("Failed to save file: %s", filePath.c_str());
    }
}


void Leaderboard::showLeaderboard()
{
    // ��ȡǰ��λ��ҵ���Ϣ
    Vector<ScoreEntry*> topScores;
    int numScores = std::min(5, static_cast<int>(scores.size()));
    for (int i = 0; i < numScores; ++i) {
        topScores.pushBack(scores.at(i));
    }
    // ����һ��Layer������ʾ���а�
    auto leaderboardLayer = LayerColor::create(Color4B::BLACK, 320, 400);
    leaderboardLayer->setAnchorPoint(Vec2(0.0f, 1.0f));
    leaderboardLayer->setPosition(Vec2(100, 100));  // ����Layer��λ��
    this->addChild(leaderboardLayer, 3); // ʹ���ʵ��Ĳ㼶
    leaderboardLayer->setName("leaderboardLayer");

    log("Show");
    // ��Layer�����ǰ������ҵ���ϢLabel
    std::string labelText = "LeaderBoard";
    auto TileLabel = Label::createWithTTF(labelText, "fonts/Marker Felt.ttf", 30);
    TileLabel->setPosition(Vec2(leaderboardLayer->getPositionX() + 30, leaderboardLayer->getPositionY() + 250)); // ����Label��λ��
    leaderboardLayer->addChild(TileLabel);
    for (int i = 0; i < topScores.size(); ++i) {
        ScoreEntry* entry = topScores.at(i);

        // �����ı���ǩ����ʾ������ֺͷ���

        std::string labelText = StringUtils::format("Rank %d: %s - Score: %d", i + 1, entry->getPlayerName().getCString(), entry->getScore());
        auto label = Label::createWithTTF(labelText, "fonts/arial.ttf", 18);
        label->setPosition(Vec2(leaderboardLayer->getPositionX() + 50, leaderboardLayer->getPositionY() +200- i * 50)); // ����Label��λ��
        leaderboardLayer->addChild(label);
    }
    // �ڿ���̨���ǰ������ҵ���Ϣ
    /*for (int i = 0; i < topScores.size(); ++i) {
        ScoreEntry* entry = topScores.at(i);
        log("Rank %d: %s - Score: %d", i + 1, entry->getPlayerName().c_str(), entry->getScore());
    }*/
}

void Leaderboard::insertScore(const cocos2d::String& playerName, int score)
{//�����а��в�����Ԫ��
      // �����µ� ScoreEntry
    log("insert!!!");
    // �����µ� ScoreEntry
    auto newEntry = ScoreEntry::create(playerName, score);

    // ʹ�ö��ֲ����ҵ�����λ��
    ssize_t low = 0;
    ssize_t high = scores.size() - 1;
    ssize_t mid;

    while (low <= high)
    {
        mid = (low + high) / 2;
        int midScore = scores.at(mid)->getScore();

        if (midScore > score)
        {
            low = mid + 1;
        }
        else if (midScore < score)
        {
            high = mid - 1;
        }
        else
        {
            // ������ȵ���������Ը�����Ҫ�Ĺ�����е���
            break;
        }
    }

    // �����µ�����
    log("%s,--%d", playerName.getCString(), score);
    // �����µ�����
    scores.insert(low, newEntry);

    log("All Scores:");
    for (const auto& entry : scores) {
        log("%s - Score: %d", entry->getPlayerName().getCString(), entry->getScore());
    }

    // �������а��ļ�
    saveScoresToFile();
}
