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
    std::vector<CardModel> _bottomCardStack;  // 底牌区栈（最上面的用于匹配）
    std::vector<CardModel> _spareCardStack;   // 备用区栈
    std::vector<CardModel> _mainCardStack;    // 主牌区栈（统一栈式管理）
    int _score;                               // 当前分数
    GameState _gameState;                     // 游戏状态
    int _level;                               // 当前关卡
    int _moves;                               // 移动次数
    int _nextCardId;                          // 下一个卡牌ID
    // 最小版本：注释掉层级相关的成员变量
    // int _currentAvailableLayer;               // 当前可点击的层级
    // std::vector<bool> _layerCleared;          // 各层是否已清空

public:
    GameModel();
    
    // 底牌相关（兼容性接口）
    CardModel getBottomCard() const { return getCurrentBottomCard(); }
    void setBottomCard(const CardModel& card) {
        _bottomCardStack.clear();
        _bottomCardStack.push_back(card);
    }

    // 备用底牌相关（兼容性接口）
    CardModel getSpareCard() const {
        return _spareCardStack.empty() ? CardModel() : _spareCardStack.back();
    }
    void setSpareCard(const CardModel& card) {
        _spareCardStack.clear();
        _spareCardStack.push_back(card);
    }

    // 兼容性方法
    void swapBottomAndSpareCards() {
        // 这个方法现在用于简单的栈顶交换
        if (!_bottomCardStack.empty() && !_spareCardStack.empty()) {
            CardModel temp = _bottomCardStack.back();
            _bottomCardStack.back() = _spareCardStack.back();
            _spareCardStack.back() = temp;
        }
    }

    // 底牌区管理
    const std::vector<CardModel>& getBottomCardStack() const { return _bottomCardStack; }
    const std::vector<CardModel>& getSpareCardStack() const { return _spareCardStack; }

    // 获取当前活跃的底牌（用于匹配）
    CardModel getCurrentBottomCard() const;
    bool hasBottomCard() const { return !_bottomCardStack.empty(); }

    // 底牌区操作
    void moveSpareToBottom();    // 备用牌移动到底牌区（覆盖）
    void moveBottomToSpare();    // 底牌移动到备用区（露出下层）

    // 初始化底牌区
    void addToBottomStack(const CardModel& card);
    void addToSpareStack(const CardModel& card);
    
    // 主牌区栈管理
    const std::vector<CardModel>& getMainCardStack() const { return _mainCardStack; }
    void addToMainStack(const CardModel& card);
    void removeFromMainStack(int cardId);
    void clearMainStack() { _mainCardStack.clear(); }

    // 主牌区相关（兼容性接口）
    const std::vector<CardModel>& getMainCards() const { return _mainCardStack; }
    void setMainCards(const std::vector<CardModel>& cards) { _mainCardStack = cards; }
    void addMainCard(const CardModel& card) { _mainCardStack.push_back(card); }
    void removeMainCard(int index);
    void removeMainCardById(int cardId);
    CardModel* getMainCardById(int cardId);
    const CardModel* getMainCardById(int cardId) const;
    int getMainCardCount() const { return _mainCardStack.size(); }
    
    // 游戏状态相关
    GameState getGameState() const { return _gameState; }
    void setGameState(GameState state) { _gameState = state; }
    
    // 卡牌ID管理
    int getNextCardId() { return _nextCardId++; }

    // 最小版本：返回true
    bool isCardClickable(const CardModel& card) const { return true; }  // 最小版本：总是可点击

    // 游戏状态检查
    bool isGameWon() const;
    bool isGameOver() const;

    // 重置游戏
    void reset();

    // 清空主牌区（兼容性接口）
    void clearMainCards() { _mainCardStack.clear(); }

    // 清空备用底牌
    void clearSpareCard() { _spareCardStack.clear(); }
};

#endif // __GAME_MODEL_H__
