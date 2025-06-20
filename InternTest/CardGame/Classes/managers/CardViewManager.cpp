#include "CardViewManager.h"
#include "../configs/GameConfig.h"

CardViewManager::CardViewManager(Node* parentNode)
    : _parentNode(parentNode)
{
    auto director = Director::getInstance();
    _visibleSize = director->getVisibleSize();
    _origin = director->getVisibleOrigin();
}

CardViewManager::~CardViewManager()
{
    clearAllCardViews();
}

void CardViewManager::updateFromGameModel(const GameModel& gameModel)
{
    // 清除现有视图
    clearAllCardViews();
    
    // 创建底牌视图
    createBottomCardView(gameModel.getBottomCard());
    
    // 创建主牌区视图
    createMainCardsView(gameModel.getMainCards());
    
    // 布局卡牌
    layoutCards(gameModel);
}

void CardViewManager::createBottomCardView(const CardModel& cardModel)
{
    CardView* cardView = createCardView(cardModel);
    if (cardView)
    {
        _cardViews[cardModel.getId()] = cardView;
        _parentNode->addChild(cardView, 1);
    }
}

void CardViewManager::createMainCardsView(const std::vector<CardModel>& cardModels)
{
    for (const auto& cardModel : cardModels)
    {
        CardView* cardView = createCardView(cardModel);
        if (cardView)
        {
            _cardViews[cardModel.getId()] = cardView;
            _parentNode->addChild(cardView, 1);
        }
    }
}

CardView* CardViewManager::createCardView(const CardModel& cardModel)
{
    CardView* cardView = CardView::create(cardModel);
    if (cardView)
    {
        // 设置点击回调
        cardView->setClickCallback([this](int cardId) {
            onCardClicked(cardId);
        });
    }
    return cardView;
}

void CardViewManager::removeCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        it->second->removeFromParent();
        _cardViews.erase(it);
    }
}

CardView* CardViewManager::getCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    return (it != _cardViews.end()) ? it->second : nullptr;
}

void CardViewManager::clearAllCardViews()
{
    for (auto& pair : _cardViews)
    {
        pair.second->removeFromParent();
    }
    _cardViews.clear();
}

void CardViewManager::layoutCards(const GameModel& gameModel)
{
    // 布局底牌
    CardView* bottomCardView = getCardView(gameModel.getBottomCard().getId());
    if (bottomCardView)
    {
        bottomCardView->setPosition(calculateBottomCardPosition());
    }
    
    // 布局主牌区
    const auto& mainCards = gameModel.getMainCards();
    for (int i = 0; i < mainCards.size(); i++)
    {
        CardView* cardView = getCardView(mainCards[i].getId());
        if (cardView)
        {
            cardView->setPosition(calculateMainCardPosition(i));
        }
    }
}

Vec2 CardViewManager::calculateBottomCardPosition()
{
    return Vec2(_origin.x + GameConfig::PositionSettings::BOTTOM_CARD_OFFSET.x,
                _origin.y + _visibleSize.height/2 + GameConfig::PositionSettings::BOTTOM_CARD_OFFSET.y);
}

Vec2 CardViewManager::calculateMainCardPosition(int index)
{
    int row = index / GameConfig::GameSettings::GRID_COLS;
    int col = index % GameConfig::GameSettings::GRID_COLS;
    
    float startX = _origin.x + _visibleSize.width/2 + GameConfig::PositionSettings::MAIN_CARDS_START_OFFSET.x;
    float startY = _origin.y + _visibleSize.height/2 + GameConfig::PositionSettings::MAIN_CARDS_START_OFFSET.y;
    
    float x = startX + (col - 1) * GameConfig::CardSettings::CARD_SPACING;
    float y = startY - row * GameConfig::CardSettings::CARD_SPACING;
    
    return Vec2(x, y);
}

void CardViewManager::playMatchAnimation(int cardId, const std::function<void()>& callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView)
    {
        cardView->playMatchAnimation([this, cardId, callback]() {
            removeCardView(cardId);
            if (callback)
            {
                callback();
            }
        });
    }
}

void CardViewManager::playMismatchAnimation(int cardId)
{
    CardView* cardView = getCardView(cardId);
    if (cardView)
    {
        cardView->playMismatchAnimation();
    }
}

void CardViewManager::highlightMatchableCards(const std::vector<int>& cardIds)
{
    clearAllHighlights();
    
    for (int cardId : cardIds)
    {
        CardView* cardView = getCardView(cardId);
        if (cardView)
        {
            cardView->setHighlighted(true);
        }
    }
}

void CardViewManager::clearAllHighlights()
{
    for (auto& pair : _cardViews)
    {
        pair.second->setHighlighted(false);
    }
}

void CardViewManager::updateBottomCard(const CardModel& cardModel)
{
    // 移除旧的底牌视图
    auto it = _cardViews.begin();
    while (it != _cardViews.end())
    {
        // 假设底牌是第一个创建的，或者可以通过其他方式识别
        // 这里简化处理，实际项目中可能需要更精确的识别方法
        if (it->second->getPosition().equals(calculateBottomCardPosition()))
        {
            it->second->removeFromParent();
            it = _cardViews.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }
    
    // 创建新的底牌视图
    createBottomCardView(cardModel);
    
    // 重新布局
    CardView* newBottomCardView = getCardView(cardModel.getId());
    if (newBottomCardView)
    {
        newBottomCardView->setPosition(calculateBottomCardPosition());
    }
}

void CardViewManager::onCardClicked(int cardId)
{
    if (_cardClickCallback)
    {
        _cardClickCallback(cardId);
    }
}
