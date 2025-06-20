#include "GameModel.h"
#include <algorithm>

GameModel::GameModel()
    : _score(0), _gameState(PLAYING), _level(1), _moves(0), _nextCardId(1)
{
}

void GameModel::removeMainCard(int index)
{
    if (index >= 0 && index < _mainCards.size())
    {
        _mainCards.erase(_mainCards.begin() + index);
    }
}

void GameModel::removeMainCardById(int cardId)
{
    auto it = std::find_if(_mainCards.begin(), _mainCards.end(),
        [cardId](const CardModel& card) {
            return card.getId() == cardId;
        });
    
    if (it != _mainCards.end())
    {
        _mainCards.erase(it);
    }
}

CardModel* GameModel::getMainCardById(int cardId)
{
    auto it = std::find_if(_mainCards.begin(), _mainCards.end(),
        [cardId](const CardModel& card) {
            return card.getId() == cardId;
        });
    
    return (it != _mainCards.end()) ? &(*it) : nullptr;
}

bool GameModel::isGameWon() const
{
    return _mainCards.empty() && _gameState == WIN;
}

bool GameModel::isGameOver() const
{
    return _gameState == WIN || _gameState == LOSE;
}

void GameModel::reset()
{
    _score = 0;
    _gameState = PLAYING;
    _moves = 0;
    _nextCardId = 1;
    _mainCards.clear();
    _bottomCard = CardModel();
}
