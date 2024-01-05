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
    // 在构造函数中加载排行榜信息
    log("initLeaderboard");

    // 初始化返回按钮
    backButton = cocos2d::ui::Button::create("LeaderBoardCloseNormal.png", "LeaderBoardCloseSelected.png");
    backButton->setPosition(Vec2(400, 480));  // 设置返回按钮的位置
    backButton->addClickEventListener(CC_CALLBACK_1(Leaderboard::backButtonCallback, this));
    backButton->setName("backButton");
    addChild(backButton, 4);
    TopScore = 0;
    // 加载排行榜信息
    loadScoresFromFile();

    // 显示排行榜
    showLeaderboard();

}

Leaderboard::~Leaderboard()
{
    // 在析构函数中保存排行榜信息到文件
    saveScoresToFile();

    // 释放排行榜信息的内存
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
    // 返回按钮的逻辑，可以切换到其他场景或执行其他操作
    // 这里你可以切换回主菜单场景或执行其他逻辑
    log("Back Button Clicked");
    this->removeChildByName("leaderboardLayer");
    this->removeChildByName("backButton");
}
void Leaderboard::loadScoresFromFile()
{
    // 从文件加载排行榜信息
    std::string filePath = FileUtils::getInstance()->fullPathForFilename("leaderboard.txt");
    log("load File : %s", filePath.c_str());
    // 以文本方式读取文件
    std::string fileContents = FileUtils::getInstance()->getStringFromFile(filePath);

    // 将读取到的文本内容按行分割
    std::vector<std::string> lines;
    std::istringstream stream(fileContents);
    std::string line;
    while (std::getline(stream, line))
    {
        lines.push_back(line);
    }

    // 解析每一行数据
    for (const auto& line : lines)
    {
        std::istringstream lineStream(line);
        std::string playerName;
        int score;
    
        // 文件中每一行的格式是 "PlayerName Score"
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
    
    // 获取工程文件夹中leaderboard.txt的完整路径
    std::string filePath = cocos2d::FileUtils::getInstance()->fullPathForFilename("leaderboard.txt");
    log("saved File : %s", filePath.c_str());
    // 将排行榜数据转换为文本格式
    std::string fileContents;
    for (const auto& entry : scores)
    {
        fileContents += cocos2d::StringUtils::format("%s %d\n", entry->getPlayerName().getCString(), entry->getScore());
    }

    // 使用 FileUtils 写入文件
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
    // 获取前五位玩家的信息
    Vector<ScoreEntry*> topScores;
    int numScores = std::min(5, static_cast<int>(scores.size()));
    for (int i = 0; i < numScores; ++i) {
        topScores.pushBack(scores.at(i));
    }
    // 创建一个Layer用于显示排行榜
    auto leaderboardLayer = LayerColor::create(Color4B::BLACK, 320, 400);
    leaderboardLayer->setAnchorPoint(Vec2(0.0f, 1.0f));
    leaderboardLayer->setPosition(Vec2(100, 100));  // 设置Layer的位置
    this->addChild(leaderboardLayer, 3); // 使用适当的层级
    leaderboardLayer->setName("leaderboardLayer");

    log("Show");
    // 在Layer中添加前五名玩家的信息Label
    std::string labelText = "LeaderBoard";
    auto TileLabel = Label::createWithTTF(labelText, "fonts/Marker Felt.ttf", 30);
    TileLabel->setPosition(Vec2(leaderboardLayer->getPositionX() + 30, leaderboardLayer->getPositionY() + 250)); // 设置Label的位置
    leaderboardLayer->addChild(TileLabel);
    for (int i = 0; i < topScores.size(); ++i) {
        ScoreEntry* entry = topScores.at(i);

        // 创建文本标签，显示玩家名字和分数

        std::string labelText = StringUtils::format("Rank %d: %s - Score: %d", i + 1, entry->getPlayerName().getCString(), entry->getScore());
        auto label = Label::createWithTTF(labelText, "fonts/arial.ttf", 18);
        label->setPosition(Vec2(leaderboardLayer->getPositionX() + 50, leaderboardLayer->getPositionY() +200- i * 50)); // 设置Label的位置
        leaderboardLayer->addChild(label);
    }
    // 在控制台输出前五名玩家的信息
    /*for (int i = 0; i < topScores.size(); ++i) {
        ScoreEntry* entry = topScores.at(i);
        log("Rank %d: %s - Score: %d", i + 1, entry->getPlayerName().c_str(), entry->getScore());
    }*/
}

void Leaderboard::insertScore(const cocos2d::String& playerName, int score)
{//在排行榜中插入新元素
      // 创建新的 ScoreEntry
    log("insert!!!");
    // 创建新的 ScoreEntry
    auto newEntry = ScoreEntry::create(playerName, score);

    // 使用二分查找找到插入位置
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
            // 处理相等的情况，可以根据需要的规则进行调整
            break;
        }
    }

    // 插入新的数据
    log("%s,--%d", playerName.getCString(), score);
    // 插入新的数据
    scores.insert(low, newEntry);

    log("All Scores:");
    for (const auto& entry : scores) {
        log("%s - Score: %d", entry->getPlayerName().getCString(), entry->getScore());
    }

    // 更新排行榜文件
    saveScoresToFile();
}
