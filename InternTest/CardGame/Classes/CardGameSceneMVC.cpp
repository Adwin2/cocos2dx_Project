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

// 游戏label部分：如关卡、成绩 
void CardGameSceneMVC::createLabels()
{
}

void CardGameSceneMVC::createMenu()
{
    // 重新开始按钮
    _restartItem = MenuItemLabel::create(
        Label::createWithTTF("Restart", GameConfig::ResourcePaths::FONT_PATH, GameConfig::UISettings::MENU_FONT_SIZE),
        CC_CALLBACK_1(CardGameSceneMVC::onRestartClicked, this));
    _restartItem->setPosition(Vec2(_origin.x + _visibleSize.width - 100, _origin.y + _visibleSize.height - 100));

    // 关闭按钮
    _closeItem = MenuItemImage::create(
        GameConfig::ResourcePaths::CLOSE_NORMAL_IMAGE,
        GameConfig::ResourcePaths::CLOSE_SELECTED_IMAGE,
        CC_CALLBACK_1(CardGameSceneMVC::onCloseClicked, this));
    _closeItem->setPosition(Vec2(_origin.x + _visibleSize.width - _closeItem->getContentSize().width/2,
                                 _origin.y + _closeItem->getContentSize().height/2));
    
    _gameMenu = Menu::create(_restartItem, _closeItem, nullptr); // MVP版本：移除_hintItem
    _gameMenu->setPosition(Vec2::ZERO);
    this->addChild(_gameMenu, 10);
}

void CardGameSceneMVC::initGameController()
{
    _gameController = new GameController();
    _gameController->init(this);

    setupControllerCallbacks();
}

// DEMO版本暂未使用，便于测试游戏逻辑
// 设置游戏结束回调 
void CardGameSceneMVC::setupControllerCallbacks()
{
    // _gameController->setGameOverCallback([this]() {
    //     onGameOver();
    // });
}

void CardGameSceneMVC::onRestartClicked(Ref* sender)
{
    _gameController->restartGame();
}

void CardGameSceneMVC::onCloseClicked(Ref* sender)
{
    Director::getInstance()->end();
}
