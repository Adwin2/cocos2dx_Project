#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../managers/CardViewManager.h"

USING_NS_CC;

/**
 * 游戏控制器
 * 职责：协调模型和视图，处理游戏逻辑流程
 */
class GameController
{
public:
    // 游戏事件回调
    typedef std::function<void(int score)> ScoreUpdateCallback;
    typedef std::function<void()> GameWinCallback;
    typedef std::function<void()> GameOverCallback;

private:
    GameModel* _gameModel;                      // 游戏数据模型
    CardViewManager* _cardViewManager;          // 卡牌视图管理器
    
    // 回调函数
    ScoreUpdateCallback _scoreUpdateCallback;
    GameWinCallback _gameWinCallback;
    GameOverCallback _gameOverCallback;

public:
    GameController();
    ~GameController();
    
    // 初始化
    void init(Node* parentNode);
    
    // 设置回调
    void setScoreUpdateCallback(const ScoreUpdateCallback& callback) { _scoreUpdateCallback = callback; }
    void setGameWinCallback(const GameWinCallback& callback) { _gameWinCallback = callback; }
    void setGameOverCallback(const GameOverCallback& callback) { _gameOverCallback = callback; }
    
    // 游戏控制
    void startNewGame();
    void restartGame();
    void pauseGame();
    void resumeGame();
    
    // 获取游戏状态
    const GameModel& getGameModel() const { return *_gameModel; }
    GameModel::GameState getGameState() const { return _gameModel->getGameState(); }
    
    // 游戏操作
    void onCardClicked(int cardId);

private:
    // 初始化游戏数据
    void initGameData();
    
    // 更新视图
    void updateViews();
    
    // 处理卡牌匹配
    void handleCardMatch(int cardId);
    
    // 处理匹配成功
    void handleMatchSuccess(int cardId);
    
    // 处理匹配失败
    void handleMatchFailure(int cardId);

    // 处理当前底牌点击（切换到备用底牌）
    void handleBottomCardClick(int cardId);

    // 处理备用底牌点击（切换到当前底牌）
    void handleSpareCardClick(int cardId);
};

#endif // __GAME_CONTROLLER_H__
