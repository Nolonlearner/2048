#include "MenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Leaderboard.h"

#define GAMEHEIGH 860
#define GAMEWIDTH 920
cocos2d::String USER = "xxx";//ȫ��
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
    // ���´��ڴ�С
    auto GLview = Director::getInstance()->getOpenGLView();
    GLview->setFrameSize(GameSize.width, GameSize.height);
    // ��ȡ��Ļ��С
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ��GameScene�ĳ�Ա�����ж���ͼƬ�ļ���
    std::string gameInfoImageFile = "game_info_image.png";
    std::string chessBoardImageFile = "chess_board_image.png";

    // ��init�����м���ͼƬ
    gameInfoSprite = Sprite::create(gameInfoImageFile);
    chessBoardSprite = Sprite::create(chessBoardImageFile);

    // ���õ�ɫ
    //gameInfoSprite->setColor(Color3B::WHITE);  // ����ʹ��setOpacity����͸����
    //chessBoardSprite->setColor(Color3B::GRAY);  // ����ʹ��setOpacity����͸����

    // ���������������ֵĴ�С
    gameInfoSprite->setContentSize(Size(visibleSize.width, visibleSize.height * 0.3));
    chessBoardSprite->setContentSize(Size(visibleSize.width, visibleSize.height * 0.7));

    // ����ê���λ��
    gameInfoSprite->setAnchorPoint(Vec2(0.5, 1.0));
    gameInfoSprite->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height));//135
    chessBoardSprite->setAnchorPoint(Vec2(0.5, 0.0));
    chessBoardSprite->setPosition(origin.x + +visibleSize.width / 2, 0);

    // ��ӵ�������
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
    cardPadding = 35.0f; // ��Ƭ֮��ļ��
    indentation = 40.0f;
    fullTileSum = 0;
    gameEndShown = false; // ��ʼ��Ϊfalse
    gameTimeSeconds = time(nullptr);  // ��ȡ��ǰʱ�䣨�룩
    createChessBoard();// ����4x4����
    initItem();


    Leaderboard* leaderboard = Leaderboard::create();
    bestScore = leaderboard->getTopScore();//��õõ���߷�

    //��Ϸ����Label
    const int FontSize2048 = 90;
    titleLabel = Label::createWithTTF("2048", "fonts/arial.ttf", FontSize2048);
    titleLabel->setTextColor(cocos2d::Color4B::BLACK);
    titleLabel->setPosition(Vec2(gameInfoSprite->getContentSize().width / 5, gameInfoSprite->getContentSize().height - FontSize2048));
    gameInfoSprite->addChild(titleLabel);

    // ��ǰ����Label
    const int FontSizeScore = 40;
    currentScoreLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", FontSizeScore);
    currentScoreLabel->setPosition(Vec2(gameInfoSprite->getContentSize().width / 2, gameInfoSprite->getContentSize().height - FontSizeScore - 10));
    gameInfoSprite->addChild(currentScoreLabel);

   

    //��߷�Label
    const int FontSizeBestScore = 40;
    bestScoreLabel = Label::createWithTTF("Best: " + std::to_string(bestScore), "fonts/Marker Felt.ttf", FontSizeBestScore);
    bestScoreLabel->setPosition(Vec2(gameInfoSprite->getContentSize().width / 2, gameInfoSprite->getContentSize().height - 2 * FontSizeScore - 10));;
    gameInfoSprite->addChild(bestScoreLabel);

    // ��Ϸʱ��Label
    const int FontSizeTime = 30;
    timeLabel = Label::createWithTTF("Time: 0", "fonts/Marker Felt.ttf", FontSizeTime);
    timeLabel->setPosition(Vec2(gameInfoSprite->getContentSize().width / 2, gameInfoSprite->getContentSize().height - 3 * FontSizeScore - 10));
    gameInfoSprite->addChild(timeLabel);

     
    //�������
    const int FontSizeUser = 40;
    UserNameLabel = Label::createWithTTF("User: ", "fonts/Marker Felt.ttf", FontSizeUser);
    UserNameLabel->setString(StringUtils::format("User: %s ", USER.getCString()));
    UserNameLabel->setPosition(Vec2(gameInfoSprite->getContentSize().width / 2, gameInfoSprite->getContentSize().height - 4 * FontSizeScore - 10));;
    gameInfoSprite->addChild(UserNameLabel);

    // ����ʱ����¶�ʱ��
    schedule([this](float dt=1.0f) {
        updateTimeLabel();
        }, "updateTimeLabelKey");
    // ��Ӽ����¼�������
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);




    // ����UI��ʾ
    updateUI();
  

   

}
void GameScene::initMusic()
{
    // Ԥ���ر�������
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("background_music.mp3");
    // ���ű�������
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background_music.mp3", true);
    // Ԥ������Ч
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
    // �Ƴ�֮ǰ�� UI Ԫ��
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
    // �Ƴ������¼�������
    this->getEventDispatcher()->removeEventListenersForTarget(this);

    log("remove");
    this->removeChildByName("OverMenu");
    this->removeChildByName("WinMenu");
    this->removeChildByName("gameOverLabel");
    this->removeChildByName("gameWinLable");
    // ������Ϸ״̬�����¿�ʼ
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
            // �����տ�Ƭ
            CardSprite* card = CardSprite::create();
            card->setContentSize(Size(cardSize, cardSize));
            card->setScale(cardSize / card->getContentSize().width);  // ���� Sprite �Ĵ�С      
            card->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
            card->setAnchorPoint(Vec2(0.5, 0.5));
            card->setPosition(Vec2(startX + j * (cardPadding + cardSize) - j * 10, startY - i * (cardPadding + cardSize) + i * 30));          
            card->setCoordinates(i, j);
            grid[i][j] = card;
            chessBoardSprite->addChild(card, 1);



            // ����������Ƭ
            CardSprite* cardShow = CardSprite::create();
            cardShow->setContentSize(Size(cardSize, cardSize));
            cardShow->setScale(cardSize / cardShow->getContentSize().width);  // ���� Sprite �Ĵ�С
            cardShow->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
            cardShow->setAnchorPoint(Vec2(0.5, 0.5));
            cardShow->setPosition(Vec2(startX + j * (cardPadding + cardSize) - j * 10, startY - i * (cardPadding + cardSize) + i * 30));
            cardShow->setCoordinates(i, j);
            gridShow[i][j] = cardShow;           
            chessBoardSprite->addChild(cardShow, 2);//��ӵ��ӽ����,�����grid�ߣ����Ը�����Ⱦ�����Ը�����grid��
            auto hide = Hide::create();
            gridShow[i][j]->runAction(hide);//��������

        }
    }
    // �����λ������������ʼֵΪ 2 �Ŀ�Ƭ
    spawnTile(2);  
}
void GameScene::spawnTile(int NewSum )//�������2��4
{
    // �����λ������1����ʼֵΪ 2 ���� 4 �Ŀ�Ƭ
    int initialCardsCount = 0;

    if (checkGameOver()) {
        // ������Ϸ�����߼���������ʾ��Ϸ���������
        log("GameOver");
        showGameOver();
    }
    else if (checkGameWin()) {
        // ������Ϸʤ���߼���������ʾʤ�������
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
            //���ÿտ�Ƭ��ɳ�ʼֵΪ 2����4 �Ŀ�Ƭ�����������λ��
            grid[randomRow][randomCol]->setTileTypeAndValue(CardSprite::TileType::Number, CCRANDOM_0_1() * 10 < 1 ? 4 : 2);
             //��10%�ļ�������4
           
            //�ö���Ч������
    
            auto action = Sequence::createWithTwoActions(ScaleTo::create(0, 0), ScaleTo::create(0.3f, 1));  //��0.3���ڴ�С���ŵ���
            grid[randomRow][randomCol]->runAction(action);
            //cardArr[row][col]->getCardLayer()->runAction(action);  //�ÿ�Ƭ�Ĳ�����ǿ�Ƭ���鱾����������Ϊ��ʹ�þֲ���������
            
            
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
void GameScene::mergeTiles(CardSprite*& CurrentCard, CardSprite*& TargetCard) // �ϲ���������
{
    calScore(TargetCard->getValue());
    TargetCard->setTileTypeAndValue(CardSprite::TileType::Number, TargetCard->getValue() * 2);
    CurrentCard->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
    fullTileSum--;//��ǰ����һ��fullTileSum
    log("fullTileSum= %d", fullTileSum);

    // �����ϲ�����
    auto mergeAnimation = Sequence::create(
        ScaleTo::create(0.1f, 1.2f),
        ScaleTo::create(0.1f, 1.0f),
        nullptr
    );
    // ���źϲ�����
    TargetCard->runAction(mergeAnimation);

    // ���źϳ���Ч
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("merge_sound.mp3");

}
void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        // ���������ƶ����߼�
        // ���� moveTiles ���������뷽����� Direction::UP
        log("UP");
        moveTiles(Direction::UP);
        break;

    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        // ���������ƶ����߼�
        // ���� moveTiles ���������뷽����� Direction::DOWN
        log("DOWN");
        moveTiles(Direction::DOWN);
        break;

    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        // ���������ƶ����߼�
        // ���� moveTiles ���������뷽����� Direction::LEFT
        log("left");
        moveTiles(Direction::LEFT);
        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        // ���������ƶ����߼�
        // ���� moveTiles ���������뷽����� Direction::RIGHT
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
    // ����ָ���ķ������ñ�������
    switch (direction) {
    case Direction::UP://���ϣ���Ӷ�����ʼ������ÿһ�ж��Ƕ����ģ�ֻ���еı仯
        moved |= MoveUp();

        break;

    case Direction::DOWN://���£���ӵײ���ʼ������ÿһ�ж��Ƕ����ģ�ֻ���еı仯
        moved |= MoveDown();
        break;

    case Direction::LEFT://���������ʼ���ұ�����ÿһ�ж��Ƕ����ģ�ֻ���еı仯
        moved |= MoveLeft();
        break;

    case Direction::RIGHT://���ң�����ҿ�ʼ���������ÿһ�ж��Ƕ����ģ�ֻ���еı仯
        moved |= MoveRight();
        break;

    default:
        break;
    }

    // ����з����ƶ����������·��鲢������ʾ
    if (moved) {
        updateUI();
    }
    spawnTile(1);
}
void GameScene::ExchangeTile(int RowA, int RowB, int ColA, int ColB)
{   
      //��������λ�õ�ֵ�ͷ������ͣ�����ʵ�ֶ���Ч��
    //��gridShow��ʵ�ֶ���
    //ר��Ūһ�������㿨Ƭʵ�ֶ�λ�����֡��ƶ�������ϵ�ж���
    auto currentPosition = grid[RowA][ColA]->getPosition();  // ��ȡ��ǰλ�õ�����
    auto targetPosition = grid[RowB][ColB]->getPosition();  // ��ȡĿ��λ�õ�����
    auto place = Place::create(currentPosition);  // ʹ��Ŀ��λ�õ����괴�� Place ����
    auto show = Show::create();
    //auto move = EaseBackInOut::create(MoveTo::create(0.20f, targetPosition));
    auto move = EaseExponentialInOut::create(MoveTo::create(0.2f, targetPosition));
    auto easeMove = EaseInOut::create(move, 2.0f);  // ʹ�� EaseInOut ����Ч��
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
    // ����������
    for (int col = 0; col < boardSize; col++)
    {
        // �����ӵڶ��п�ʼ��������
        for (int row = 1; row < boardSize; row++)
        {
            // �����ǰ����Ϊ�գ����ô���ֱ�ӽ�����һ��
            if (grid[row][col]->getTileType() == CardSprite::TileType::Empty) { continue; }

            int nowRow = row;
            //���鲻Ϊ��
            // �ƶ��߼�
            while (nowRow > 0 && grid[nowRow - 1][col]->getTileType() == CardSprite::TileType::Empty)
            {
                ExchangeTile(nowRow, nowRow - 1, col, col);
                nowRow--;
                moved = true;
            }

            // �ϲ��߼�
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

    // ����������
    for (int col = 0; col < boardSize; col++)
    {
        // �ӵ����ڶ��п�ʼ���ϱ�������һ��
        for (int row = boardSize - 2; row >= 0; row--)
        {
            // �����ǰ����Ϊ�գ�������ֱ�ӽ�����һ��
            if (grid[row][col]->getTileType() == CardSprite::TileType::Empty) { continue; }

            int nowRow = row;

            // �ƶ��߼�
            while (nowRow < boardSize - 1 && grid[nowRow + 1][col]->getTileType() == CardSprite::TileType::Empty)
            {
                ExchangeTile(nowRow, nowRow + 1, col, col);
                nowRow++;
                moved = true;
            }

            // �ϲ��߼�
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

    // ����������
    for (int row = 0; row < boardSize; row++)
    {
        // �ӵ����ڶ��п�ʼ�����������һ��
        for (int col = boardSize - 2; col >= 0; col--)
        {
            // �����ǰ����Ϊ�գ�������ֱ�ӽ�����һ��
            if (grid[row][col]->getTileType() == CardSprite::TileType::Empty) { continue; }

            int nowCol = col;

            // �ƶ��߼�
            while (nowCol < boardSize - 1 && grid[row][nowCol + 1]->getTileType() == CardSprite::TileType::Empty)
            {
                ExchangeTile(row, row, nowCol, nowCol + 1);
                nowCol++;
                moved = true;
            }

            // �ϲ��߼�
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

    // ����������
    for (int row = 0; row < boardSize; row++)
    {
        // �ӵڶ��п�ʼ���ұ��������һ��
        for (int col = 1; col < boardSize; col++)
        {
            // �����ǰ����Ϊ�գ�������ֱ�ӽ�����һ��
            if (grid[row][col]->getTileType() == CardSprite::TileType::Empty) { continue; }

            int nowCol = col;

            // �ƶ��߼�
            while (nowCol > 0 && grid[row][nowCol - 1]->getTileType() == CardSprite::TileType::Empty)
            {
                ExchangeTile(row, row, nowCol, nowCol - 1);
                nowCol--;
                moved = true;
            }

            // �ϲ��߼�
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
    //�Ƿ����ƶ����߼����������û���κ��ƶ�������Ҫ������ɿ�Ƭ��Ҳ������Ӯ�䣬��һ��ܹؼ�����������׳�bug
    /*bool moved = false;
    //�����ƶ��Ĳ������
    //y��ʾ�б�ţ�x��ʾ�б��
    for (int y = 0; y < 4; y++)  //�������б��������Ȳ���
    {
        for (int x = 0; x < 4; x++)   //�ڲ���N^2���Ӷȵ�����ð������
        {
            for (int x1 = x + 1; x1 < 4; x1++)
            {
                if (grid[x1][y]->getValue() > 0)  //x�ұߵĿ�Ƭ�����ֲŶ���
                {
                    if (grid[x][y]->getValue() == 0)
                    {
                      
                        grid[x][y]->setTileTypeAndValue(grid[x1][y]->getTileType(), grid[x1][y]->getValue());
                        grid[x1][y]->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
                        x--;  //��ɨ��һ�飬ȷ�����н����ȷ
                        moved = true;
                    }
                    else if (grid[x][y]->getValue() == grid[x1][y]->getValue())
                    {
              
                        //���xλ�÷ǿգ�����x1��������ͬ�����2
                        grid[x][y]->setTileTypeAndValue(grid[x][y]->getTileType(), grid[x][y]->getValue() * 2);   
                        grid[x1][y]->setTileTypeAndValue(CardSprite::TileType::Empty, 0);
                        fullTileSum--;
                        moved = true;
                    }
                    break;   //�˴�break��ֹ����������2��bug
                }
            }
        }
    }
    return moved;*/
}
bool GameScene::checkGameOver()
{
    // ����Ƿ��п�λ��
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (grid[i][j]->getTileType() == CardSprite::TileType::Empty) {
                return false; // ��Ϸδ����
            }
        }
    }

    // ����Ƿ������ڵ���ͬ����
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if ((j < boardSize - 1 && grid[i][j]->getValue() == grid[i][j + 1]->getValue()) ||
                (i < boardSize - 1 && grid[i][j]->getValue() == grid[i + 1][j]->getValue())) {
                return false; // ��Ϸδ����
            }
        }
    }

    // ��Ϸ����
    return true;
}
bool GameScene::checkGameWin()
{
    // ����Ƿ���2048������
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (grid[i][j]->getValue() == 2048) {
                return true; // ��Ϸʤ��
            }
        }
    }

    // ��Ϸδʤ��
    return false;
}
void GameScene::updateUI()
{
    // ���·�����ʾ
    // ������� gameInfoLayer �������ʾ������ Label���������������ʾ
    if (currentScore > bestScore)
    {
        bestScore = currentScore;//������߷�
    }
    renderGameInfo();
    // �����Ϸ״̬
    if (checkGameOver()) {
        // ������Ϸ�����߼���������ʾ��Ϸ���������
        log("GameOver");
        showGameOver();
    }
    else if (checkGameWin()) {
        // ������Ϸʤ���߼���������ʾʤ�������
        log("GameWin");
        showGameWin();
    }

}
void GameScene::renderGameInfo()
{
    // ��Ⱦ��Ϸ��Ϣ����
    // ������ʾ��Ϸ���ơ���߷ּ�¼����Ϸʱ���
    // ��Ϸ����
    titleLabel->setString("2048");
    // ��ǰ����
    currentScoreLabel->setString("Score: " + std::to_string(currentScore));
    // ��߷ּ�¼
    bestScoreLabel->setString("Best: " + std::to_string(bestScore));


}
void GameScene::updateTimeLabel()
{
    time_t currentTimeSeconds = time(nullptr);  // ��ȡ��ǰʱ�䣨�룩
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
        return;  // ����Ѿ���ʾ���������棬������ʾ
    }

    //������Ϣ
    Leaderboard* leaderboard = Leaderboard::create();
    leaderboard->insertScore(USER, currentScore);
    

    bestScore = leaderboard->getTopScore();//��õõ���߷�


    // ��ʾ��Ϸ������Ϣ�����¿�ʼ���˳���Ϸѡ��
    auto gameOverLabel = Label::createWithTTF("GameOver!!!", "fonts/arial.ttf", 80);
    gameOverLabel->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 + 50));
    gameOverLabel->setName("gameOverLabel");
    this->addChild(gameOverLabel, 10);
    
    gameOverLabel->setTextColor(Color4B::BLACK);
    // ������¿�ʼ��ť
    auto restartButton = MenuItemLabel::create(Label::createWithTTF("ReStart", "fonts/arial.ttf", 40),
        CC_CALLBACK_1(GameScene::resumeGame, this));
    restartButton->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 - 50));
    restartButton->setColor(Color3B::BLACK);
    // ����˳���Ϸ��ť
    auto quitButton = MenuItemLabel::create(Label::createWithTTF("Exit", "fonts/arial.ttf", 40),
        CC_CALLBACK_1(GameScene::TurnMenuCallback, this));
    quitButton->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 - 150));
    quitButton->setColor(Color3B::BLACK);
    // �����˵����������¿�ʼ���˳���Ϸ��ť
    auto OverMenu = Menu::create(restartButton, quitButton, nullptr);
    OverMenu->setPosition(Vec2::ZERO);
    OverMenu->setName("OverMenu");
    this->addChild(OverMenu, 10);
    gameEndShown = true;
}
void GameScene::showGameWin()
{
    if (gameEndShown) {
        return;  // ����Ѿ���ʾ���������棬������ʾ
    }
    Leaderboard* leaderboard = Leaderboard::create();
    leaderboard->insertScore(USER, currentScore);
    bestScore = leaderboard->getTopScore();//��õõ���߷�


    // ��ʾ��Ϸʤ����Ϣ�����¿�ʼ���˳���Ϸѡ��
    auto gameWinLabel = Label::createWithTTF("WIN!!!", "fonts/arial.ttf", 80);
    gameWinLabel->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 + 50));
    gameWinLabel->setTextColor(Color4B::BLACK);
    gameWinLabel->setName("gameWinLable");
    this->addChild(gameWinLabel, 10);
    
    // ������¿�ʼ��ť
    auto restartButton = MenuItemLabel::create(Label::createWithTTF("ReStart", "fonts/arial.ttf", 40),
        CC_CALLBACK_1(GameScene::resumeGame, this));
    restartButton->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 - 50));
    restartButton->setColor(Color3B::BLACK);
    // ����˳���Ϸ��ť
    auto quitButton = MenuItemLabel::create(Label::createWithTTF("Exit", "fonts/arial.ttf", 40),
        CC_CALLBACK_1(GameScene::TurnMenuCallback, this));
    quitButton->setPosition(Vec2(GameSize.width / 2, GameSize.height / 2 - 150));
    quitButton->setColor(Color3B::BLACK);
    // �����˵����������¿�ʼ���˳���Ϸ��ť
    auto menu = Menu::create(restartButton, quitButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    menu->setName("WinMenu");
    this->addChild(menu, 10);
    gameEndShown = true;

}