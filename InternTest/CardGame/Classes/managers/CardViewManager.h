#ifndef __CARD_VIEW_MANAGER_H__
#define __CARD_VIEW_MANAGER_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../views/CardView.h"
#include <map>

USING_NS_CC;

/**
 * 卡牌视图管理器
 * 职责：管理所有卡牌视图的创建、更新和销毁
 */
class CardViewManager
{
public:
    // 卡牌点击回调函数类型
    typedef std::function<void(int cardId)> CardClickCallback;

private:
    Node* _parentNode;                          // 父节点
    std::map<int, CardView*> _cardViews;        // 卡牌视图映射表
    CardClickCallback _cardClickCallback;       // 卡牌点击回调
    Size _visibleSize;                          // 可见区域大小
    Vec2 _origin;                               // 原点位置

public:
    CardViewManager(Node* parentNode);
    ~CardViewManager();
    
    // 设置卡牌点击回调
    void setCardClickCallback(const CardClickCallback& callback) { _cardClickCallback = callback; }
    
    // 根据游戏模型更新所有卡牌视图
    void updateFromGameModel(const GameModel& gameModel);
    
    // 创建底牌视图
    void createBottomCardView(const CardModel& cardModel);
    
    // 创建主牌区视图
    void createMainCardsView(const std::vector<CardModel>& cardModels);
    
    // 移除卡牌视图
    void removeCardView(int cardId);
    
    // 获取卡牌视图
    CardView* getCardView(int cardId);
    
    // 清除所有卡牌视图
    void clearAllCardViews();
    
    // 布局卡牌
    void layoutCards(const GameModel& gameModel);
    
    // 播放匹配动画
    void playMatchAnimation(int cardId, const std::function<void()>& callback = nullptr);
    
    // 播放不匹配动画
    void playMismatchAnimation(int cardId);
    
    // 高亮可匹配的卡牌
    void highlightMatchableCards(const std::vector<int>& cardIds);
    
    // 清除所有高亮
    void clearAllHighlights();
    
    // 更新底牌显示
    void updateBottomCard(const CardModel& cardModel);

private:
    // 创建单个卡牌视图
    CardView* createCardView(const CardModel& cardModel);
    
    // 计算底牌位置
    Vec2 calculateBottomCardPosition();
    
    // 计算主牌区位置
    Vec2 calculateMainCardPosition(int index);
    
    // 卡牌点击处理
    void onCardClicked(int cardId);
};

#endif // __CARD_VIEW_MANAGER_H__
