#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "CardModel.h"
#include <vector>

/**
 * 游戏数据模型
 * 职责：存储游戏运行时的所有数据状态
 */
class GameModel
{
public:
    // 游戏状态枚举
    enum GameState {
        PLAYING,        // 游戏中
        WIN,           // 胜利
        LOSE,          // 失败
        PAUSED         // 暂停
    };

private:
    CardModel _bottomCard;                    // 底牌
    std::vector<CardModel> _mainCards;        // 主牌区的牌
    int _score;                               // 当前分数
    GameState _gameState;                     // 游戏状态
    int _level;                               // 当前关卡
    int _moves;                               // 移动次数
    int _nextCardId;                          // 下一个卡牌ID

public:
    GameModel();
    
    // 底牌相关
    const CardModel& getBottomCard() const { return _bottomCard; }
    void setBottomCard(const CardModel& card) { _bottomCard = card; }
    
    // 主牌区相关
    const std::vector<CardModel>& getMainCards() const { return _mainCards; }
    void setMainCards(const std::vector<CardModel>& cards) { _mainCards = cards; }
    void addMainCard(const CardModel& card) { _mainCards.push_back(card); }
    void removeMainCard(int index);
    void removeMainCardById(int cardId);
    CardModel* getMainCardById(int cardId);
    int getMainCardCount() const { return _mainCards.size(); }
    
    // 分数相关
    int getScore() const { return _score; }
    void setScore(int score) { _score = score; }
    void addScore(int points) { _score += points; }
    
    // 游戏状态相关
    GameState getGameState() const { return _gameState; }
    void setGameState(GameState state) { _gameState = state; }
    
    // 关卡相关
    int getLevel() const { return _level; }
    void setLevel(int level) { _level = level; }
    
    // 移动次数相关
    int getMoves() const { return _moves; }
    void setMoves(int moves) { _moves = moves; }
    void incrementMoves() { _moves++; }
    
    // 卡牌ID管理
    int getNextCardId() { return _nextCardId++; }
    
    // 游戏状态检查
    bool isGameWon() const;
    bool isGameOver() const;
    
    // 重置游戏
    void reset();
    
    // 清空主牌区
    void clearMainCards() { _mainCards.clear(); }
};

#endif // __GAME_MODEL_H__
