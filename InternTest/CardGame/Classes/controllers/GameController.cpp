#include "GameController.h"
#include "../services/GameLogicService.h"
#include "../services/CardGeneratorService.h"
#include "../configs/GameConfig.h"

GameController::GameController()
    : _gameModel(nullptr), _cardViewManager(nullptr)
{
    _gameModel = new GameModel();
}

GameController::~GameController()
{
    CC_SAFE_DELETE(_gameModel);
    CC_SAFE_DELETE(_cardViewManager);
}

void GameController::init(Node* parentNode)
{
    // 创建卡牌视图管理器
    _cardViewManager = new CardViewManager(parentNode);
    
    // 设置卡牌点击回调
    _cardViewManager->setCardClickCallback([this](int cardId) {
        onCardClicked(cardId);
    });
}

void GameController::startNewGame()
{
    // 重置游戏模型
    _gameModel->reset();
    
    // 初始化游戏数据
    initGameData();
    
    // 更新视图
    updateViews();
}

void GameController::restartGame()
{
    startNewGame();
}

void GameController::pauseGame()
{
    if (_gameModel->getGameState() == GameModel::PLAYING)
    {
        _gameModel->setGameState(GameModel::PAUSED);
    }
}

void GameController::resumeGame()
{
    if (_gameModel->getGameState() == GameModel::PAUSED)
    {
        _gameModel->setGameState(GameModel::PLAYING);
    }
}

void GameController::onCardClicked(int cardId)
{
    // 只有在游戏进行中才处理点击
    if (_gameModel->getGameState() != GameModel::PLAYING)
    {
        return;
    }
    
    handleCardMatch(cardId);
}

void GameController::showHint()
{
    if (_gameModel->getGameState() != GameModel::PLAYING)
    {
        return;
    }
    
    // 获取可匹配的卡牌
    auto matchableCards = GameLogicService::getAvailableMatches(*_gameModel);
    
    // 高亮显示
    _cardViewManager->highlightMatchableCards(matchableCards);
}

void GameController::clearHints()
{
    _cardViewManager->clearAllHighlights();
}

void GameController::nextLevel()
{
    int currentLevel = _gameModel->getLevel();
    setLevel(currentLevel + 1);
}

void GameController::setLevel(int level)
{
    _gameModel->setLevel(level);
    startNewGame();
}

void GameController::initGameData()
{
    // 使用卡牌生成服务初始化卡牌
    CardGeneratorService::generateInitialCards(*_gameModel, GameConfig::GameSettings::MAIN_CARDS_COUNT);
}

void GameController::updateViews()
{
    // 更新卡牌视图
    _cardViewManager->updateFromGameModel(*_gameModel);
    
    // 通知分数更新
    notifyScoreUpdate();
}

void GameController::handleCardMatch(int cardId)
{
    // 使用游戏逻辑服务处理匹配
    bool matchSuccess = GameLogicService::processCardMatch(*_gameModel, cardId);
    
    if (matchSuccess)
    {
        handleMatchSuccess(cardId);
    }
    else
    {
        handleMatchFailure(cardId);
    }
}

void GameController::handleMatchSuccess(int cardId)
{
    // 播放匹配成功动画
    _cardViewManager->playMatchAnimation(cardId, [this]() {
        // 动画完成后生成新的底牌
        generateNewBottomCard();
        
        // 检查游戏是否结束
        checkGameEnd();
        
        // 通知分数更新
        notifyScoreUpdate();
    });
}

void GameController::handleMatchFailure(int cardId)
{
    // 播放匹配失败动画
    _cardViewManager->playMismatchAnimation(cardId);
}

void GameController::generateNewBottomCard()
{
    // 使用卡牌生成服务生成新的底牌
    CardGeneratorService::generateNewBottomCard(*_gameModel);
    
    // 更新底牌视图
    _cardViewManager->updateBottomCard(_gameModel->getBottomCard());
}

void GameController::checkGameEnd()
{
    if (_gameModel->getMainCardCount() == 0)
    {
        // 游戏胜利
        _gameModel->setGameState(GameModel::WIN);
        notifyGameWin();
    }
    else if (!GameLogicService::hasAvailableMatches(*_gameModel))
    {
        // 没有可用匹配，游戏失败
        _gameModel->setGameState(GameModel::LOSE);
        notifyGameOver();
    }
}

void GameController::notifyScoreUpdate()
{
    if (_scoreUpdateCallback)
    {
        _scoreUpdateCallback(_gameModel->getScore());
    }
}

void GameController::notifyGameWin()
{
    if (_gameWinCallback)
    {
        _gameWinCallback();
    }
}

void GameController::notifyGameOver()
{
    if (_gameOverCallback)
    {
        _gameOverCallback();
    }
}
