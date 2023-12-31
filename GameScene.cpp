#include "MenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Leaderboard.h"

#define GAMEHEIGH 860
#define GAMEWIDTH 920
cocos2d::String USER = "xxx";//全局
static cocos2d::Size GameSize = cocos2d::Size(GAMEHEIGH, GAMEWIDTH);
USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}
Scene* GameScene::createScene(cocos2d::String UserName)
{
    USER = UserName;
    return GameScene::create();
}
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    // 更新窗口大小
    auto GLview = Director::getInstance()->getOpenGLView();
    GLview->setFrameSize(GameSize.width, GameSize.height);
    // 获取屏幕大小
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 在GameScene的成员变量中定义图片文件名
    std::string gameInfoImageFile = "game_info_image.png";
    std::string chessBoardImageFile = "chess_board_image.png";

    // 在init方法中加载图片
    gameInfoSprite = Sprite::create(gameInfoImageFile);
    chessBoardSprite = Sprite::create(chessBoardImageFile);

    // 设置底色
    //gameInfoSprite->setColor(Color3B::WHITE);  // 或者使用setOpacity设置透明度
    //chessBoardSprite->setColor(Color3B::GRAY);  // 或者使用setOpacity设置透明度

    // 设置上下两个部分的大小
    gameInfoSprite->setContentSize(Size(visibleSize.width, visibleSize.height * 0.3));
    chessBoardSprite->setContentSize(Size(visibleSize.width, visibleSize.height * 0.7));

    // 设置锚点和位置
    gameInfoSprite->setAnchorPoint(Vec2(0.5, 1.0));
    gameInfoSprite->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height));//135
    chessBoardSprite->setAnchorPoint(Vec2(0.5, 0.0));
    chessBoardSprite->setPosition(origin.x + +visibleSize.width / 2, 0);

    // 添加到场景中
    this->addChild(gameInfoSprite);
    this->addChild(chessBoardSprite);
    initGame();
    initMusic();
    return true;
}
void GameScene::initItem()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto resumeItem = MenuItemImage::create(
        "ResumeNormal.png",
        "ResumeSelected.png",
        CC_CALLBACK_1(GameScene::resumeGame, this));

    if (resumeItem == nullptr ||
        resumeItem->getContentSize().width <= 0 ||
        resumeItem->getContentSize().height <= 0)
    {
        problemLoading("'ResumeNormal.png' and 'ResumeSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width * 0.8- resumeItem->getContentSize().width / 2;
        float y = origin.y + visibleSize.height * 0.7+50;
        resumeItem->setPosition(Vec2(x, y));
    }



    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(GameScene::TurnMenuCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width * 0.9 - closeItem->getContentSize().width / 2;
        float y = origin.y + visibleSize.height * 0.7+50;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, resumeItem, NULL);
    //auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

}
void GameScene::initGame()
{
    currentScore = 0;
    bestScore = 0;
    cardPadding = 35.0f; // 卡片之间的间隔
    indentation = 40.0f;
    fullTileSum = 0;
    gameEndShown = false; // 初始化为false
    gameTimeSeconds = time(nullptr);  // 获取当前时间（秒）
    createChessBoard();// 创建4x4棋盘
    initItem();


    Leaderboard* leaderboard = Leaderboard::create();
    bestScore = leaderboard->getTopScore();//获得得到最高分

    //游戏标题Label
    const int FontSize2048 = 90;
    titleLabel = Label::createWithTTF("2048", "fonts/arial.ttf", FontSize2048);
    titleLabel->setTextColor(cocos2d::Color4B::BLACK);
    titleLabel->setPosition(Vec2(gameInfoSprite->getContentSize().width / 5, gameInfoSprite->getContentSize().height - FontSize2048));
    gameInfoSprite->addChild(titleLabel);

    // 当前分数Label
    const int FontSizeScore = 40;
    currentScoreLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", FontSizeScore);
    currentScoreLabel->setPosition(Vec2(gameInfoSprite->getContentSize().width / 2, gameInfoSprite->getContentSize().height - FontSizeScore - 10));
    gameInfoSprite->addChild(currentScoreLabel);

   

    //最高分Label
    const int FontSizeBestScore = 40;
    bestScoreLabel = Label::createWithTTF("Best: " + std::to_string(bestScore), "fonts/Marker Felt.ttf", FontSizeBestScore);
    bestScoreLabel->setPosition(Vec2(gameInfoSprite->getContentSize().width / 2, gameInfoSprite->getContentSize().height - 2 * FontSizeScore - 10));;
    gameInfoSprite->addChild(bestScoreLabel);

    // 游戏时间Label
    const int FontSizeTime = 30;
    timeLabel = Label::createWithTTF("Time: 0", "fonts/Marker Felt.ttf", FontSizeTime);
    timeLabel->setPosition(Vec2(gameInfoSprite->getContentSize().width / 2, gameInfoSprite->getContentSize().height - 3 * FontSizeScore - 10));
    gameInfoSprite->addChild(timeLabel);

     
    //玩家姓名
    const int FontSizeUser = 40;
    UserNameLabel = Label::createWithTTF("User: ", "fonts/Marker Felt.ttf", FontSizeUser);
    UserNameLabel->setString(StringUtils::format("User: %s ", USER.getCString()));
    UserNameLabel->setPosition(Vec2(gameInfoSprite->getContentSize().width / 2, gameInfoSprite->getContentSize().height - 4 * FontSizeScore - 10));;
    gameInfoSprite->addChild(UserNameLabel);

    // 启动时间更新定时器
    schedule([this](float dt=1.0f) {
        updateTimeLabel();
        }, "updateTimeLabelKey");
    // 添加键盘事件监听器
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);




    // 更新UI显示
    updateUI();
  

   

}
void GameScene::initMusic()
{
    // 预加载背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("background_music.mp3");
    // 播放背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background_music.mp3", true);
    // 预加载音效
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("merge_sound.mp3");
}
void GameScene::TurnMenuCallback(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(MenuScene::createScene());

}
void GameScene::resumeGame(Ref* pSender)
{
    log("resumeGame");
    Leaderboard* leaderboard = Leaderboard::create();
    leaderboard->insertScore(USER, currentScore);
    // 移除之前的 UI 元素
    if (currentScoreLabel != nullptr) {
        currentScoreLabel->removeFromParent();
        currentScoreLabel = nullptr;
    }

    if (timeLabel != nullptr) {
        timeLabel->removeFromParent();
        timeLabel = nullptr;
    }
    if (titleLabel != nullptr) {
        titleLabel->removeFromParent();
        titleLabel = nullptr;
    }
    if (bestScoreLabel != nullptr)
    {
        bestScoreLabel->removeFromParent();
        bestScoreLabel = nullptr;
    }
    if (UserNameLabel != nullptr)
    {
        UserNameLabel->removeFromParent();
        UserNameLabel = nullptr;
    }
    // 移除键盘事件监听器
    this->getEventDispatcher()->removeEventListenersForTarget(this);

    log("remove");
    this->removeChildByName("OverMenu");
    this->removeChildByName("WinMenu");
    this->removeChildByName("gameOverLabel");
    this->removeChildByName("gameWinLable");
    // 重置游戏状态，重新开始
    clearGrid();
    initGame();

}
void GameScene::createChessBoard()
{
   
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    const auto theta = 0.8f;
    const auto cardSize = ((chessBoardSprite->getContentSize().width - (4 - 1) * cardPadding - 2 * indentation) / 4) * theta;

    const auto startX = origin.x + indentation ;//origin.x
    const auto startY = origin.y + cardSize * 4 + cardPadding * (4 - 1) / 2 + indentation - 80;


    if (fullTileSum == boardSize * boardSize)
    {
        log("IsFull!!!");
        return;
    }


    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
        {
            // 创建空卡片
            CardSprite* card = CardSprite::create();
            card->setContentSize(Size(cardSize, cardSize));
            card->setScale(cardSize / card->getContentSize().width);  // 设置 Sprite 的大小      
            card->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
            card->setAnchorPoint(Vec2(0.5, 0.5));
            card->setPosition(Vec2(startX + j * (cardPadding + cardSize) - j * 10, startY - i * (cardPadding + cardSize) + i * 30));          
            card->setCoordinates(i, j);
            grid[i][j] = card;
            chessBoardSprite->addChild(card, 1);



            // 创建动画卡片
            CardSprite* cardShow = CardSprite::create();
            cardShow->setContentSize(Size(cardSize, cardSize));
            cardShow->setScale(cardSize / cardShow->getContentSize().width);  // 设置 Sprite 的大小
            cardShow->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
            cardShow->setAnchorPoint(Vec2(0.5, 0.5));
            cardShow->setPosition(Vec2(startX + j * (cardPadding + cardSize) - j * 10, startY - i * (cardPadding + cardSize) + i * 30));
            cardShow->setCoordinates(i, j);
            gridShow[i][j] = cardShow;           
            chessBoardSprite->addChild(cardShow, 2);//添加到子结点中,级别比grid高，所以更晚渲染，可以覆盖在grid上
            auto hide = Hide::create();
            gridShow[i][j]->runAction(hide);//隐藏起来

        }
    }
    // 在随机位置生成两个初始值为 2 的卡片
    spawnTile(2);  
}
void GameScene::spawnTile(int NewSum )//随机生成2和4
{
    // 在随机位置生成1个初始值为 2 或者 4 的卡片
    int initialCardsCount = 0;

    if (checkGameOver()) {
        // 处理游戏结束逻辑，可以显示游戏结束界面等
        log("GameOver");
        showGameOver();
    }
    else if (checkGameWin()) {
        // 处理游戏胜利逻辑，可以显示胜利界面等
        log("GameWin");
        showGameWin();
    }
    if (fullTileSum == boardSize * boardSize)
    {
        log("spawnTile IsFull!!!");
        return;
    }
    while (initialCardsCount < NewSum)
    {
        int randomRow = rand() % 4;
        int randomCol = rand() % 4;
        if (grid[randomRow][randomCol]->getTileType() == CardSprite::TileType::Empty)
        {
            //将该空卡片变成初始值为 2或者4 的卡片并放置在随机位置
            grid[randomRow][randomCol]->setTileTypeAndValue(CardSprite::TileType::Number, CCRANDOM_0_1() * 10 < 1 ? 4 : 2);
             //有10%的几率生成4
           
            //用动画效果生成
    
            auto action = Sequence::createWithTwoActions(ScaleTo::create(0, 0), ScaleTo::create(0.3f, 1));  //在0.3秒内从小缩放到大
            grid[randomRow][randomCol]->runAction(action);
            //cardArr[row][col]->getCardLayer()->runAction(action);  //用卡片的层而不是卡片精灵本身做动作是为了使用局部坐标缩放
            
            
            log("<%d,%d>\n", randomRow + 1, randomCol + 1);

            initialCardsCount++;
            fullTileSum++;
        }
    }
}
void GameScene::calScore(int newScore)
{
    currentScore += newScore;
}
void GameScene::mergeTiles(CardSprite*& CurrentCard, CardSprite*& TargetCard) // 合并两个方块
{
    calScore(TargetCard->getValue());
    TargetCard->setTileTypeAndValue(CardSprite::TileType::Number, TargetCard->getValue() * 2);
    CurrentCard->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
    fullTileSum--;//当前减少一个fullTileSum
    log("fullTileSum= %d", fullTileSum);

    // 创建合并动画
    auto mergeAnimation = Sequence::create(
        ScaleTo::create(0.1f, 1.2f),
        ScaleTo::create(0.1f, 1.0f),
        nullptr
    );
    // 播放合并动画
    TargetCard->runAction(mergeAnimation);

    // 播放合成音效
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("merge_sound.mp3");

}
void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        // 处理向上移动的逻辑
        // 调用 moveTiles 方法，传入方向参数 Direction::UP
        log("UP");
        moveTiles(Direction::UP);
        break;

    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        // 处理向下移动的逻辑
        // 调用 moveTiles 方法，传入方向参数 Direction::DOWN
        log("DOWN");
        moveTiles(Direction::DOWN);
        break;

    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        // 处理向左移动的逻辑
        // 调用 moveTiles 方法，传入方向参数 Direction::LEFT
        log("left");
        moveTiles(Direction::LEFT);
        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        // 处理向右移动的逻辑
        // 调用 moveTiles 方法，传入方向参数 Direction::RIGHT
        log("RIGHT");
        moveTiles(Direction::RIGHT);
        break;
    case EventKeyboard::KeyCode::KEY_0:
        log("print");
        printAll();
        break;
    default:
        break;
    }
}
void GameScene::moveTiles(Direction direction)
{
    bool moved = false;
    // 根据指定的方向设置遍历参数
    switch (direction) {
    case Direction::UP://向上，则从顶部开始遍历，每一列都是独立的，只看行的变化
        moved |= MoveUp();

        break;

    case Direction::DOWN://向下，则从底部开始遍历，每一列都是独立的，只看行的变化
        moved |= MoveDown();
        break;

    case Direction::LEFT://向左，则从左开始向右遍历，每一行都是独立的，只看列的变化
        moved |= MoveLeft();
        break;

    case Direction::RIGHT://向右，则从右开始向左遍历，每一行都是独立的，只看列的变化
        moved |= MoveRight();
        break;

    default:
        break;
    }

    // 如果有方块移动，则生成新方块并更新显示
    if (moved) {
        updateUI();
    }
    spawnTile(1);
}
void GameScene::ExchangeTile(int RowA, int RowB, int ColA, int ColB)
{   
      //交换两个位置的值和方块类型，并且实现动画效果
    //用gridShow来实现动画
    //专门弄一个动画层卡片实现定位、显现、移动、隐藏系列动画
    auto currentPosition = grid[RowA][ColA]->getPosition();  // 获取当前位置的坐标
    auto targetPosition = grid[RowB][ColB]->getPosition();  // 获取目标位置的坐标
    auto place = Place::create(currentPosition);  // 使用目标位置的坐标创建 Place 动作
    auto show = Show::create();
    //auto move = EaseBackInOut::create(MoveTo::create(0.20f, targetPosition));
    auto move = EaseExponentialInOut::create(MoveTo::create(0.2f, targetPosition));
    auto easeMove = EaseInOut::create(move, 2.0f);  // 使用 EaseInOut 缓动效果
    auto hide = Hide::create();
    gridShow[RowA][ColA]->setTileTypeAndValue(grid[RowA][ColA]->getTileType(), grid[RowA][ColA]->getValue());
    gridShow[RowA][ColA]->runAction(Sequence::create(place, show, easeMove, hide, nullptr));

    auto TempValue = grid[RowA][ColA]->getValue();
    auto TempType = grid[RowA][ColA]->getTileType();
    grid[RowA][ColA]->setTileTypeAndValue(grid[RowB][ColB]->getTileType(), grid[RowB][ColB]->getValue());
    grid[RowB][ColB]->setTileTypeAndValue(TempType, TempValue);

  
}
bool GameScene::MoveUp()
{
    bool moved = false;
    bool isBlock = false;
    // 遍历所有列
    for (int col = 0; col < boardSize; col++)
    {
        // 遍历从第二行开始的所有行
        for (int row = 1; row < boardSize; row++)
        {
            // 如果当前方块为空，不用处理，直接进入下一行
            if (grid[row][col]->getTileType() == CardSprite::TileType::Empty) { continue; }

            int nowRow = row;
            //方块不为空
            // 移动逻辑
            while (nowRow > 0 && grid[nowRow - 1][col]->getTileType() == CardSprite::TileType::Empty)
            {
                ExchangeTile(nowRow, nowRow - 1, col, col);
                nowRow--;
                moved = true;
            }

            // 合并逻辑
            if (nowRow > 0 && grid[nowRow - 1][col]->getTileType() == CardSprite::TileType::Number &&
                grid[nowRow][col]->getTileType() == CardSprite::TileType::Number &&
                grid[nowRow - 1][col]->getValue() == grid[nowRow][col]->getValue())
            {
                mergeTiles(grid[nowRow][col], grid[nowRow - 1][col]);
                moved = true;
            }
        }
    }
    return moved;
}
bool GameScene::MoveDown()
{
    bool moved = false;

    // 遍历所有列
    for (int col = 0; col < boardSize; col++)
    {
        // 从倒数第二行开始向上遍历到第一行
        for (int row = boardSize - 2; row >= 0; row--)
        {
            // 如果当前方块为空，不处理，直接进入下一行
            if (grid[row][col]->getTileType() == CardSprite::TileType::Empty) { continue; }

            int nowRow = row;

            // 移动逻辑
            while (nowRow < boardSize - 1 && grid[nowRow + 1][col]->getTileType() == CardSprite::TileType::Empty)
            {
                ExchangeTile(nowRow, nowRow + 1, col, col);
                nowRow++;
                moved = true;
            }

            // 合并逻辑
            if (nowRow < boardSize - 1 && grid[nowRow + 1][col]->getTileType() == CardSprite::TileType::Number &&
                grid[nowRow][col]->getTileType() == CardSprite::TileType::Number &&
                grid[nowRow + 1][col]->getValue() == grid[nowRow][col]->getValue())
            {
                mergeTiles(grid[nowRow][col], grid[nowRow + 1][col]);
                moved = true;
            }
        }
    }
    return moved;
}
bool GameScene::MoveRight()
{
    bool moved = false;

    // 遍历所有行
    for (int row = 0; row < boardSize; row++)
    {
        // 从倒数第二列开始向左遍历到第一列
        for (int col = boardSize - 2; col >= 0; col--)
        {
            // 如果当前方块为空，不处理，直接进入下一列
            if (grid[row][col]->getTileType() == CardSprite::TileType::Empty) { continue; }

            int nowCol = col;

            // 移动逻辑
            while (nowCol < boardSize - 1 && grid[row][nowCol + 1]->getTileType() == CardSprite::TileType::Empty)
            {
                ExchangeTile(row, row, nowCol, nowCol + 1);
                nowCol++;
                moved = true;
            }

            // 合并逻辑
            if (nowCol < boardSize - 1 && grid[row][nowCol + 1]->getTileType() == CardSprite::TileType::Number &&
                grid[row][nowCol]->getTileType() == CardSprite::TileType::Number &&
                grid[row][nowCol + 1]->getValue() == grid[row][nowCol]->getValue())
            {
                mergeTiles(grid[row][nowCol], grid[row][nowCol + 1]);
                moved = true;
            }
        }
    }
    return moved;
}
bool GameScene::MoveLeft()
{
    bool moved = false;

    // 遍历所有行
    for (int row = 0; row < boardSize; row++)
    {
        // 从第二列开始向右遍历到最后一列
        for (int col = 1; col < boardSize; col++)
        {
            // 如果当前方块为空，不处理，直接进入下一列
            if (grid[row][col]->getTileType() == CardSprite::TileType::Empty) { continue; }

            int nowCol = col;

            // 移动逻辑
            while (nowCol > 0 && grid[row][nowCol - 1]->getTileType() == CardSprite::TileType::Empty)
            {
                ExchangeTile(row, row, nowCol, nowCol - 1);
                nowCol--;
                moved = true;
            }

            // 合并逻辑
            if (nowCol > 0 && grid[row][nowCol - 1]->getTileType() == CardSprite::TileType::Number &&
                grid[row][nowCol]->getTileType() == CardSprite::TileType::Number &&
                grid[row][nowCol - 1]->getValue() == grid[row][nowCol]->getValue())
            {
                mergeTiles(grid[row][nowCol], grid[row][nowCol - 1]);
                moved = true;
            }
        }
    }
    return moved;
    //是否有移动的逻辑变量，如果没有任何移动，则不需要随机生成卡片，也不检验赢输，这一点很关键，否则很容易出bug
    /*bool moved = false;
    //计算移动的步进间距
    //y表示行标号，x表示列标号
    for (int y = 0; y < 4; y++)  //最外层的行遍历可以先不管
    {
        for (int x = 0; x < 4; x++)   //内部的N^2复杂度的类似冒泡排序
        {
            for (int x1 = x + 1; x1 < 4; x1++)
            {
                if (grid[x1][y]->getValue() > 0)  //x右边的卡片有数字才动作
                {
                    if (grid[x][y]->getValue() == 0)
                    {
                      
                        grid[x][y]->setTileTypeAndValue(grid[x1][y]->getTileType(), grid[x1][y]->getValue());
                        grid[x1][y]->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
                        x--;  //再扫描一遍，确保所有结果正确
                        moved = true;
                    }
                    else if (grid[x][y]->getValue() == grid[x1][y]->getValue())
                    {
              
                        //如果x位置非空，且与x1处数字相同，则乘2
                        grid[x][y]->setTileTypeAndValue(grid[x][y]->getTileType(), grid[x][y]->getValue() * 2);   
                        grid[x1][y]->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
                        fullTileSum--;
                        moved = true;
                    }
                    break;   //此处break防止出现连续乘2的bug
                }
            }
        }
    }
    return moved;*/
}
bool GameScene::checkGameOver()
{
    // 检查是否有空位置
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (grid[i][j]->getTileType() == CardSprite::TileType::Empty) {
                return false; // 游戏未结束
            }
        }
    }

    // 检查是否有相邻的相同数字
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if ((j < boardSize - 1 && grid[i][j]->getValue() == grid[i][j + 1]->getValue()) ||
                (i < boardSize - 1 && grid[i][j]->getValue() == grid[i + 1][j]->getValue())) {
                return false; // 游戏未结束
            }
        }
    }

    // 游戏结束
    return true;
}
bool GameScene::checkGameWin()
{
    // 检查是否有2048的数字
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (grid[i][j]->getValue() == 2048) {
                return true; // 游戏胜利
            }
        }
    }

    // 游戏未胜利
    return false;
}
void GameScene::updateUI()
{
    // 更新分数显示
    // 你可以在 gameInfoLayer 中添加显示分数的 Label，并在这里更新显示
    if (currentScore > bestScore)
    {
        bestScore = currentScore;//更新最高分
    }
    renderGameInfo();
    // 检查游戏状态
    if (checkGameOver()) {
        // 处理游戏结束逻辑，可以显示游戏结束界面等
        log("GameOver");
        showGameOver();
    }
    else if (checkGameWin()) {
        // 处理游戏胜利逻辑，可以显示胜利界面等
        log("GameWin");
        showGameWin();
    }

}
void GameScene::renderGameInfo()
{
    // 渲染游戏信息窗口
    // 可以显示游戏名称、最高分记录、游戏时间等
    // 游戏标题
    titleLabel->setString("2048");
    // 当前分数
    currentScoreLabel->setString("Score: " + std::to_string(currentScore));
    // 最高分记录
    bestScoreLabel->setString("Best: " + std::to_string(bestScore));


}
void GameScene::updateTimeLabel()
{
    time_t currentTimeSeconds = time(nullptr);  // 获取当前时间（秒）
    long seconds = currentTimeSeconds - gameTimeSeconds;

    timeLabel->setString("Time: " + std::to_string(seconds));
}
void GameScene::printAll()
{
    log(" -1-2-3-4-");
    for (int i = 0; i < boardSize; i++)
    {
        int j = 0;
        log("%d %d %d %d %d", i+1, grid[i][j]->getValue(), grid[i][j+1]->getValue(), grid[i][j+2]->getValue(), grid[i][j+3]->getValue());
      
    }
}
void GameScene::clearGrid()
{
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            grid[i][j]->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
        }
    }
}
void GameScene::showGameOver()
{
    if (gameEndShown) {
        return;  // 如果已经显示过结束画面，则不再显示
    }

    //更新信息
    Leaderboard* leaderboard = Leaderboard::create();
    leaderboard->insertScore(USER, currentScore);
    

    bestScore = leaderboard->getTopScore();//获得得到最高分


    // 显示游戏结束信息和重新开始或退出游戏选项
    auto gameOverLabel = Label::createWithTTF("GameOver!!!", "fonts/arial.ttf", 80);
    gameOverLabel->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 + 50));
    gameOverLabel->setName("gameOverLabel");
    this->addChild(gameOverLabel, 10);
    
    gameOverLabel->setTextColor(Color4B::BLACK);
    // 添加重新开始按钮
    auto restartButton = MenuItemLabel::create(Label::createWithTTF("ReStart", "fonts/arial.ttf", 40),
        CC_CALLBACK_1(GameScene::resumeGame, this));
    restartButton->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 - 50));
    restartButton->setColor(Color3B::BLACK);
    // 添加退出游戏按钮
    auto quitButton = MenuItemLabel::create(Label::createWithTTF("Exit", "fonts/arial.ttf", 40),
        CC_CALLBACK_1(GameScene::TurnMenuCallback, this));
    quitButton->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 - 150));
    quitButton->setColor(Color3B::BLACK);
    // 创建菜单，包含重新开始和退出游戏按钮
    auto OverMenu = Menu::create(restartButton, quitButton, nullptr);
    OverMenu->setPosition(Vec2::ZERO);
    OverMenu->setName("OverMenu");
    this->addChild(OverMenu, 10);
    gameEndShown = true;
}
void GameScene::showGameWin()
{
    if (gameEndShown) {
        return;  // 如果已经显示过结束画面，则不再显示
    }
    Leaderboard* leaderboard = Leaderboard::create();
    leaderboard->insertScore(USER, currentScore);
    bestScore = leaderboard->getTopScore();//获得得到最高分


    // 显示游戏胜利信息和重新开始或退出游戏选项
    auto gameWinLabel = Label::createWithTTF("WIN!!!", "fonts/arial.ttf", 80);
    gameWinLabel->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 + 50));
    gameWinLabel->setTextColor(Color4B::BLACK);
    gameWinLabel->setName("gameWinLable");
    this->addChild(gameWinLabel, 10);
    
    // 添加重新开始按钮
    auto restartButton = MenuItemLabel::create(Label::createWithTTF("ReStart", "fonts/arial.ttf", 40),
        CC_CALLBACK_1(GameScene::resumeGame, this));
    restartButton->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 - 50));
    restartButton->setColor(Color3B::BLACK);
    // 添加退出游戏按钮
    auto quitButton = MenuItemLabel::create(Label::createWithTTF("Exit", "fonts/arial.ttf", 40),
        CC_CALLBACK_1(GameScene::TurnMenuCallback, this));
    quitButton->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 - 150));
    quitButton->setColor(Color3B::BLACK);
    // 创建菜单，包含重新开始和退出游戏按钮
    auto menu = Menu::create(restartButton, quitButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    menu->setName("WinMenu");
    this->addChild(menu, 10);
    gameEndShown = true;

}