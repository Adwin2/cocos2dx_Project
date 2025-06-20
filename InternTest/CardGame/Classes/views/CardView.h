#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "../models/CardModel.h"
#include "../services/ResourceService.h"

USING_NS_CC;

/**
 * 卡牌视图类
 * 职责：负责卡牌的显示和用户交互，不包含业务逻辑
 */
class CardView : public Sprite
{
public:
    // 卡牌点击回调函数类型
    typedef std::function<void(int cardId)> CardClickCallback;

private:
    int _cardId;                    // 卡牌ID
    CardModel _cardModel;           // 卡牌数据模型
    Sprite* _backgroundSprite;      // 背景精灵
    Sprite* _valueSprite;           // 数值精灵
    Sprite* _suitSprite;            // 花色精灵
    Sprite* _smallValueSprite;      // 小数值精灵（左上角）
    Sprite* _smallSuitSprite;       // 小花色精灵（左上角）
    CardClickCallback _clickCallback; // 点击回调

public:
    static CardView* create(const CardModel& cardModel);
    bool init(const CardModel& cardModel);
    
    // 设置卡牌数据并更新显示
    void setCardModel(const CardModel& cardModel);
    const CardModel& getCardModel() const { return _cardModel; }
    
    // 获取卡牌ID
    int getCardId() const { return _cardId; }
    
    // 设置点击回调
    void setClickCallback(const CardClickCallback& callback) { _clickCallback = callback; }
    
    // 更新显示
    void updateDisplay();
    
    // 翻牌动画
    void flipCard();
    void setFaceUp(bool faceUp);
    
    // 匹配成功动画
    void playMatchAnimation(const std::function<void()>& callback = nullptr);
    
    // 匹配失败动画
    void playMismatchAnimation();
    
    // 高亮显示（提示可匹配）
    void setHighlighted(bool highlighted);
    
    // 设置卡牌可交互性
    void setInteractable(bool interactable);

private:
    // 初始化触摸事件
    void initTouchEvents();

    // 创建精灵组件
    void createSprites();

    // 更新精灵显示
    void updateSprites();

    // 更新卡牌资源
    void updateCardResources();

    // 触摸事件处理
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif // __CARD_VIEW_H__
