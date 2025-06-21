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

    // 创建底牌栈视图
    createBottomCardStackView(gameModel.getBottomCardStack());

    // 创建备用栈视图
    createSpareCardStackView(gameModel.getSpareCardStack());

    // 创建主牌栈视图
    createMainCardStackView(gameModel.getMainCardStack());

    // 布局卡牌
    layoutCards(gameModel);
}

// 底牌栈视图
void CardViewManager::createBottomCardStackView(const std::vector<CardModel>& bottomStack)
{
    for (size_t i = 0; i < bottomStack.size(); i++)
    {
        const auto& cardModel = bottomStack[i];
        CardView* cardView = createCardView(cardModel);
        if (cardView)
        {
            _cardViews[cardModel.getId()] = cardView;
            // Z-order: 底层卡牌Z值较低，顶层卡牌Z值较高
            int zOrder = 5 + static_cast<int>(i);
            _parentNode->addChild(cardView, zOrder);
        }
    }
}

// 备用栈视图
void CardViewManager::createSpareCardStackView(const std::vector<CardModel>& spareStack)
{
    for (size_t i = 0; i < spareStack.size(); i++)
    {
        const auto& cardModel = spareStack[i];
        CardView* cardView = createCardView(cardModel);
        if (cardView)
        {
            // 备用栈的卡牌都是背面朝下
            cardView->setFaceUp(false);

            _cardViews[cardModel.getId()] = cardView;
            // Z-order: 备用栈的Z值较低
            int zOrder = 2 + static_cast<int>(i);
            _parentNode->addChild(cardView, zOrder);
        }
    }
}

// 主牌栈视图（统一栈式接口）
void CardViewManager::createMainCardStackView(const std::vector<CardModel>& mainStack)
{
    for (size_t i = 0; i < mainStack.size(); i++)
    {
        const auto& cardModel = mainStack[i];
        CardView* cardView = createCardView(cardModel);
        if (cardView)
        {
            _cardViews[cardModel.getId()] = cardView;

            // 主牌栈使用网格布局，Z-order按索引递增
            int zOrder = 10 + static_cast<int>(i);
            _parentNode->addChild(cardView, zOrder);
        }
    }
}

// 主牌区（兼容性接口）
void CardViewManager::createMainCardsView(const std::vector<CardModel>& cardModels)
{
    createMainCardStackView(cardModels);  // 委托给统一接口
}

// 创建单个卡牌视图
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

// 移除卡牌视图
void CardViewManager::removeCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        it->second->removeFromParent();
        _cardViews.erase(it);
    }
}

// 获取卡牌视图
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
    // 布局底牌栈（所有卡牌在同一位置，形成叠放效果）
    const auto& bottomStack = gameModel.getBottomCardStack();
    Vec2 bottomPosition = calculateBottomCardPosition();
    for (const auto& card : bottomStack)
    {
        CardView* cardView = getCardView(card.getId());
        if (cardView)
        {
            cardView->setPosition(bottomPosition);
        }
    }

    // 布局备用栈（所有卡牌在同一位置，形成叠放效果）
    const auto& spareStack = gameModel.getSpareCardStack();
    Vec2 sparePosition = calculateSpareCardPosition();
    for (const auto& card : spareStack)
    {
        CardView* cardView = getCardView(card.getId());
        if (cardView)
        {
            cardView->setPosition(sparePosition);
        }
    }

    // 布局主牌栈（统一接口）
    const auto& mainStack = gameModel.getMainCardStack();
    for (size_t i = 0; i < mainStack.size(); i++)
    {
        CardView* cardView = getCardView(mainStack[i].getId());
        if (cardView)
        {
            cardView->setPosition(calculateMainCardStackPosition(static_cast<int>(i)));
        }
    }
}

Vec2 CardViewManager::calculateBottomCardPosition()
{
    return Vec2(_origin.x + GameConfig::PositionSettings::BOTTOM_CARD_OFFSET.x,
                _origin.y + _visibleSize.height/2 + GameConfig::PositionSettings::BOTTOM_CARD_OFFSET.y);
}

Vec2 CardViewManager::calculateSpareCardPosition()
{
    return Vec2(_origin.x + GameConfig::PositionSettings::SPARE_CARD_OFFSET.x,
                _origin.y + _visibleSize.height/2 + GameConfig::PositionSettings::SPARE_CARD_OFFSET.y);
}

Vec2 CardViewManager::calculateMainCardPosition(int index)
{
    return calculateMainCardStackPosition(index);  // 委托给统一接口
}

// 主牌栈位置计算
Vec2 CardViewManager::calculateMainCardStackPosition(int stackIndex)
{
    int row = stackIndex / GameConfig::UISettings::MAIN_CARD_COLS;
    int col = stackIndex % GameConfig::UISettings::MAIN_CARD_COLS;

    // 计算网格起始位置
    float startX = _origin.x + _visibleSize.width/2 - (GameConfig::UISettings::MAIN_CARD_COLS - 1) * GameConfig::UISettings::MAIN_CARD_SPACING / 2 + GameConfig::PositionSettings::MAIN_CARDS_OFFSET.x;
    float startY = _origin.y + _visibleSize.height/2 + (GameConfig::UISettings::MAIN_CARD_ROWS - 1) * GameConfig::UISettings::MAIN_CARD_SPACING / 2 + GameConfig::PositionSettings::MAIN_CARDS_OFFSET.y;

    float x = startX + col * GameConfig::UISettings::MAIN_CARD_SPACING;
    float y = startY - row * GameConfig::UISettings::MAIN_CARD_SPACING;

    return Vec2(x, y);
}

// 播放匹配失败动画
void CardViewManager::playMismatchAnimation(int cardId)
{
    CardView* cardView = getCardView(cardId);
    if (cardView)
    {
        cardView->playMismatchAnimation();
    }
}

void CardViewManager::syncBottomCardsState(const GameModel& gameModel)
{
    // 只更新现有CardView的数据，不重建视图
    // 这个方法用于动画完成后同步数据状态，避免干扰正在进行的动画

    // 更新底牌CardView的数据模型
    CardView* bottomCardView = getCardView(gameModel.getBottomCard().getId());
    if (bottomCardView)
    {
        bottomCardView->setCardModel(gameModel.getBottomCard());
    }

    // 更新备用底牌CardView的数据模型
    CardView* spareCardView = getCardView(gameModel.getSpareCard().getId());
    if (spareCardView)
    {
        spareCardView->setCardModel(gameModel.getSpareCard());
    }
}

// 只更新卡牌的可点击状态，不触发updateViews
void CardViewManager::syncCardStatesOnly(const GameModel& gameModel)
{
    updateCardClickableStates(gameModel);
    // 不调用updateFromGameModel()，避免clearAllCardViews()
    // 让正在进行的动画自然完成
}

void CardViewManager::onCardClicked(int cardId)
{
    if (_cardClickCallback)
    {
        _cardClickCallback(cardId);
    }
}

void CardViewManager::setCardClickable(int cardId, bool clickable)
{
    CardView* cardView = getCardView(cardId);
    if (cardView)
    {
        cardView->setInteractable(clickable);
    }
}

void CardViewManager::updateCardClickableStates(const GameModel& gameModel)
{
    for (auto& pair : _cardViews)
    {
        CardView* cardView = pair.second;
        if (cardView)
        {
            // 检查卡牌是否可点击
            bool clickable = gameModel.isCardClickable(cardView->getCardModel());
            cardView->setInteractable(clickable);
        }
    }
}
