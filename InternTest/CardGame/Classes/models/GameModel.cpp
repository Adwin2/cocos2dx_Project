#include "GameModel.h"
#include <algorithm>

GameModel::GameModel()
    : _score(0), _gameState(PLAYING), _level(1), _moves(0), _nextCardId(1)
{
}

// 新的统一栈式接口
void GameModel::addToMainStack(const CardModel& card)
{
    CardModel newCard = card;
    newCard.setFaceUp(true);  // 主牌栈的卡牌正面朝上
    _mainCardStack.push_back(newCard);
}

void GameModel::removeFromMainStack(int cardId)
{
    auto it = std::find_if(_mainCardStack.begin(), _mainCardStack.end(),
        [cardId](const CardModel& card) {
            return card.getId() == cardId;
        });

    if (it != _mainCardStack.end())
    {
        _mainCardStack.erase(it);
    }
}

// 兼容性接口实现
void GameModel::removeMainCard(int index)
{
    if (index >= 0 && index < static_cast<int>(_mainCardStack.size()))
    {
        _mainCardStack.erase(_mainCardStack.begin() + index);
    }
}

void GameModel::removeMainCardById(int cardId)
{
    removeFromMainStack(cardId);  // 委托给统一接口
}

CardModel* GameModel::getMainCardById(int cardId)
{
    auto it = std::find_if(_mainCardStack.begin(), _mainCardStack.end(),
        [cardId](const CardModel& card) {
            return card.getId() == cardId;
        });

    return (it != _mainCardStack.end()) ? &(*it) : nullptr;
}

const CardModel* GameModel::getMainCardById(int cardId) const
{
    auto it = std::find_if(_mainCardStack.begin(), _mainCardStack.end(),
        [cardId](const CardModel& card) {
            return card.getId() == cardId;
        });

    return (it != _mainCardStack.end()) ? &(*it) : nullptr;
}

bool GameModel::isGameWon() const
{
    // DEMO模式：总是返回false，避免游戏胜利判定
    return false;

    /*
    // 原始的游戏胜利判定逻辑
    return _mainCards.empty() && _gameState == WIN;
    */
}

bool GameModel::isGameOver() const
{
    // DEMO模式：总是返回false，避免游戏结束判定
    return false;

    /*
    // 原始的游戏结束判定逻辑
    return _gameState == WIN || _gameState == LOSE;
    */
}

void GameModel::reset()
{
    _score = 0;
    _gameState = PLAYING;
    _moves = 0;
    _nextCardId = 1;
    _mainCardStack.clear();  // 使用新的栈式成员变量
    _bottomCardStack.clear();
    _spareCardStack.clear();
}

// 新的栈式底牌管理方法
CardModel GameModel::getCurrentBottomCard() const
{
    return _bottomCardStack.empty() ? CardModel() : _bottomCardStack.back();
}

void GameModel::moveSpareToBottom()
{
    // 备用牌移动到底牌区（覆盖）
    if (!_spareCardStack.empty())
    {
        CardModel spareCard = _spareCardStack.back();
        _spareCardStack.pop_back();

        // 设置为正面朝上（用于匹配）
        spareCard.setFaceUp(true);
        _bottomCardStack.push_back(spareCard);
    }
}

void GameModel::moveBottomToSpare()
{
    // 底牌移动到备用区
    CardModel bottomCard = _bottomCardStack.back();
    _bottomCardStack.pop_back();
    // 设置为背面朝下
    bottomCard.setFaceUp(false);
    _spareCardStack.push_back(bottomCard);
    
}

void GameModel::addToBottomStack(const CardModel& card)
{
    CardModel newCard = card;
    newCard.setFaceUp(true);  // 底牌栈的卡牌正面朝上
    _bottomCardStack.push_back(newCard);
}

void GameModel::addToSpareStack(const CardModel& card)
{
    CardModel newCard = card;
    newCard.setFaceUp(false);  // 备用栈的卡牌背面朝下
    _spareCardStack.push_back(newCard);
}
