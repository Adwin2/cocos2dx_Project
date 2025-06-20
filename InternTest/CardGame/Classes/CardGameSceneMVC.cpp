#include "CardGameSceneMVC.h"
#include "configs/GameConfig.h"

Scene* CardGameSceneMVC::createScene()
{
    return CardGameSceneMVC::create();
}

bool CardGameSceneMVC::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    _gameController = nullptr;
    
    // 创建UI
    createUI();
    
    // 初始化游戏控制器
    initGameController();
    
    // 开始新游戏
    _gameController->startNewGame();
    
    return true;
}

void CardGameSceneMVC::createUI()
{
    createBackground();
    createLabels();
    createMenu();
}

void CardGameSceneMVC::createBackground()
{
    auto background = LayerColor::create(Color4B(GameConfig::ColorSettings::BACKGROUND_COLOR.r,
                                                 GameConfig::ColorSettings::BACKGROUND_COLOR.g,
                                                 GameConfig::ColorSettings::BACKGROUND_COLOR.b,
                                                 255));
    this->addChild(background, -1);
}

void CardGameSceneMVC::createLabels()
{
    // 分数标签
    _scoreLabel = Label::createWithTTF("分数: 0", 
                                       GameConfig::ResourcePaths::FONT_PATH, 
                                       GameConfig::UISettings::SCORE_LABEL_FONT_SIZE);
    _scoreLabel->setPosition(Vec2(_origin.x + _visibleSize.width + GameConfig::PositionSettings::SCORE_LABEL_OFFSET.x,
                                  _origin.y + _visibleSize.height + GameConfig::PositionSettings::SCORE_LABEL_OFFSET.y));
    _scoreLabel->setColor(GameConfig::ColorSettings::SCORE_TEXT_COLOR);
    this->addChild(_scoreLabel, 10);
    
    // 关卡标签
    _levelLabel = Label::createWithTTF("关卡: 1", 
                                       GameConfig::ResourcePaths::FONT_PATH, 
                                       GameConfig::UISettings::SCORE_LABEL_FONT_SIZE);
    _levelLabel->setPosition(Vec2(_origin.x + _visibleSize.width + GameConfig::PositionSettings::SCORE_LABEL_OFFSET.x,
                                  _origin.y + _visibleSize.height + GameConfig::PositionSettings::SCORE_LABEL_OFFSET.y - 40));
    _levelLabel->setColor(GameConfig::ColorSettings::SCORE_TEXT_COLOR);
    this->addChild(_levelLabel, 10);
    
    // 说明标签
    _instructionLabel = Label::createWithTTF("点击主牌区的牌与底牌匹配消除", 
                                             GameConfig::ResourcePaths::FONT_PATH, 
                                             GameConfig::UISettings::INSTRUCTION_FONT_SIZE);
    _instructionLabel->setPosition(Vec2(_origin.x + _visibleSize.width/2,
                                        _origin.y + GameConfig::PositionSettings::INSTRUCTION_OFFSET.y));
    this->addChild(_instructionLabel, 10);
    
    // 游戏状态标签
    _gameStatusLabel = Label::createWithTTF("", 
                                            GameConfig::ResourcePaths::FONT_PATH, 
                                            32);
    _gameStatusLabel->setPosition(Vec2(_origin.x + _visibleSize.width/2,
                                       _origin.y + _visibleSize.height/2));
    _gameStatusLabel->setVisible(false);
    this->addChild(_gameStatusLabel, 20);
}

void CardGameSceneMVC::createMenu()
{
    // 重新开始按钮
    _restartItem = MenuItemLabel::create(
        Label::createWithTTF("重新开始", GameConfig::ResourcePaths::FONT_PATH, GameConfig::UISettings::MENU_FONT_SIZE),
        CC_CALLBACK_1(CardGameSceneMVC::onRestartClicked, this));
    _restartItem->setPosition(Vec2(_origin.x + _visibleSize.width - 100, _origin.y + _visibleSize.height - 100));
    
    // 提示按钮
    _hintItem = MenuItemLabel::create(
        Label::createWithTTF("提示", GameConfig::ResourcePaths::FONT_PATH, GameConfig::UISettings::MENU_FONT_SIZE),
        CC_CALLBACK_1(CardGameSceneMVC::onHintClicked, this));
    _hintItem->setPosition(Vec2(_origin.x + _visibleSize.width - 100, _origin.y + _visibleSize.height - 140));
    
    // 关闭按钮
    _closeItem = MenuItemImage::create(
        GameConfig::ResourcePaths::CLOSE_NORMAL_IMAGE,
        GameConfig::ResourcePaths::CLOSE_SELECTED_IMAGE,
        CC_CALLBACK_1(CardGameSceneMVC::onCloseClicked, this));
    _closeItem->setPosition(Vec2(_origin.x + _visibleSize.width - _closeItem->getContentSize().width/2,
                                 _origin.y + _closeItem->getContentSize().height/2));
    
    _gameMenu = Menu::create(_restartItem, _hintItem, _closeItem, nullptr);
    _gameMenu->setPosition(Vec2::ZERO);
    this->addChild(_gameMenu, 10);
}

void CardGameSceneMVC::initGameController()
{
    _gameController = new GameController();
    _gameController->init(this);

    setupControllerCallbacks();
}

void CardGameSceneMVC::setupControllerCallbacks()
{
    // 设置分数更新回调
    _gameController->setScoreUpdateCallback([this](int score) {
        onScoreUpdate(score);
    });

    // 设置游戏胜利回调
    _gameController->setGameWinCallback([this]() {
        onGameWin();
    });

    // 设置游戏结束回调
    _gameController->setGameOverCallback([this]() {
        onGameOver();
    });
}

void CardGameSceneMVC::updateScoreDisplay(int score)
{
    _scoreLabel->setString("分数: " + std::to_string(score));
}

void CardGameSceneMVC::updateLevelDisplay(int level)
{
    _levelLabel->setString("关卡: " + std::to_string(level));
}

void CardGameSceneMVC::updateGameStatusDisplay(const std::string& status)
{
    _gameStatusLabel->setString(status);
    _gameStatusLabel->setVisible(!status.empty());
}

void CardGameSceneMVC::onGameWin()
{
    showMessage("恭喜！游戏完成！", GameConfig::ColorSettings::WIN_TEXT_COLOR);
}

void CardGameSceneMVC::onGameOver()
{
    showMessage("游戏结束！没有可用的匹配了", Color3B::RED);
}

void CardGameSceneMVC::onScoreUpdate(int score)
{
    updateScoreDisplay(score);
    updateLevelDisplay(_gameController->getCurrentLevel());
}

void CardGameSceneMVC::onRestartClicked(Ref* sender)
{
    hideMessage();
    _gameController->restartGame();
}

void CardGameSceneMVC::onHintClicked(Ref* sender)
{
    _gameController->showHint();

    // 3秒后自动清除提示
    this->scheduleOnce([this](float dt) {
        _gameController->clearHints();
    }, 3.0f, "clear_hints");
}

void CardGameSceneMVC::onCloseClicked(Ref* sender)
{
    Director::getInstance()->end();
}

void CardGameSceneMVC::showMessage(const std::string& message, const Color3B& color)
{
    _gameStatusLabel->setString(message);
    _gameStatusLabel->setColor(color);
    _gameStatusLabel->setVisible(true);
}

void CardGameSceneMVC::hideMessage()
{
    _gameStatusLabel->setVisible(false);
}
