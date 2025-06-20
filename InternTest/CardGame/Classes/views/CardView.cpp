#include "CardView.h"
#include "../configs/GameConfig.h"

CardView* CardView::create(const CardModel& cardModel)
{
    CardView* card = new (std::nothrow) CardView();
    if (card && card->init(cardModel))
    {
        card->autorelease();
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

bool CardView::init(const CardModel& cardModel)
{
    // 使用卡牌背景图片初始化
    auto resources = ResourceService::getCardResources(cardModel);
    if (!Sprite::initWithFile(resources.backgroundPath))
    {
        return false;
    }
    
    _cardModel = cardModel;
    _cardId = cardModel.getId();
    _backgroundSprite = nullptr;
    _valueSprite = nullptr;
    _suitSprite = nullptr;
    _smallValueSprite = nullptr;
    _smallSuitSprite = nullptr;
    
    // 创建精灵组件
    createSprites();
    
    // 初始化触摸事件
    initTouchEvents();
    
    // 更新显示
    updateDisplay();
    
    return true;
}

void CardView::setCardModel(const CardModel& cardModel)
{
    _cardModel = cardModel;
    _cardId = cardModel.getId();
    updateDisplay();
}

void CardView::updateDisplay()
{
    updateSprites();
}

void CardView::createSprites()
{
    Size cardSize = this->getContentSize();
    
    // 创建中央大数值精灵
    _valueSprite = Sprite::create();
    _valueSprite->setPosition(cardSize.width * 0.5f, cardSize.height * 0.5f);
    this->addChild(_valueSprite, 2);
    
    // 创建中央花色精灵
    _suitSprite = Sprite::create();
    _suitSprite->setPosition(cardSize.width * 0.5f, cardSize.height * 0.3f);
    this->addChild(_suitSprite, 2);
    
    // 创建左上角小数值精灵
    _smallValueSprite = Sprite::create();
    _smallValueSprite->setPosition(cardSize.width * 0.15f, cardSize.height * 0.85f);
    this->addChild(_smallValueSprite, 2);
    
    // 创建左上角小花色精灵
    _smallSuitSprite = Sprite::create();
    _smallSuitSprite->setPosition(cardSize.width * 0.15f, cardSize.height * 0.7f);
    this->addChild(_smallSuitSprite, 2);
}

void CardView::updateSprites()
{
    auto resources = ResourceService::getCardResources(_cardModel);
    
    if (_cardModel.isFaceUp())
    {
        // 正面显示
        this->setTexture(resources.backgroundPath);
        
        // 更新大数值精灵
        if (!resources.valuePath.empty() && _valueSprite)
        {
            _valueSprite->setTexture(resources.valuePath);
            _valueSprite->setVisible(true);
        }
        
        // 更新花色精灵
        if (!resources.suitPath.empty() && _suitSprite)
        {
            _suitSprite->setTexture(resources.suitPath);
            _suitSprite->setVisible(true);
        }
        
        // 更新小数值精灵
        if (_smallValueSprite)
        {
            std::string smallValuePath = ResourceService::getCardValueImagePath(_cardModel.getValue(), _cardModel.isRed(), false);
            _smallValueSprite->setTexture(smallValuePath);
            _smallValueSprite->setVisible(true);
        }
        
        // 更新小花色精灵
        if (_smallSuitSprite)
        {
            _smallSuitSprite->setTexture(resources.suitPath);
            _smallSuitSprite->setVisible(true);
        }
    }
    else
    {
        // 背面显示
        this->setTexture(ResourceService::Paths::CARD_BACK);
        
        // 隐藏所有内容精灵
        if (_valueSprite) _valueSprite->setVisible(false);
        if (_suitSprite) _suitSprite->setVisible(false);
        if (_smallValueSprite) _smallValueSprite->setVisible(false);
        if (_smallSuitSprite) _smallSuitSprite->setVisible(false);
    }
}

void CardView::flipCard()
{
    // 翻牌动画
    auto scaleDown = ScaleTo::create(0.1f, 0.0f, 1.0f);
    auto flipAction = CallFunc::create([this]() {
        _cardModel.flip();
        updateDisplay();
    });
    auto scaleUp = ScaleTo::create(0.1f, 1.0f, 1.0f);
    
    auto sequence = Sequence::create(scaleDown, flipAction, scaleUp, nullptr);
    this->runAction(sequence);
}

void CardView::setFaceUp(bool faceUp)
{
    _cardModel.setFaceUp(faceUp);
    updateDisplay();
}

void CardView::playMatchAnimation(const std::function<void()>& callback)
{
    // 匹配成功动画：放大然后淡出
    auto scaleUp = ScaleTo::create(0.2f, 1.2f);
    auto fadeOut = FadeOut::create(0.3f);
    auto callbackAction = callback ? CallFunc::create(callback) : nullptr;
    
    Action* sequence;
    if (callbackAction)
    {
        sequence = Sequence::create(scaleUp, fadeOut, callbackAction, nullptr);
    }
    else
    {
        sequence = Sequence::create(scaleUp, fadeOut, nullptr);
    }
    
    this->runAction(sequence);
}

void CardView::playMismatchAnimation()
{
    // 匹配失败动画：左右摇摆
    auto moveRight = MoveBy::create(0.05f, Vec2(5, 0));
    auto moveLeft = MoveBy::create(0.1f, Vec2(-10, 0));
    auto moveBack = MoveBy::create(0.05f, Vec2(5, 0));
    
    auto sequence = Sequence::create(moveRight, moveLeft, moveBack, nullptr);
    this->runAction(sequence);
}

void CardView::setHighlighted(bool highlighted)
{
    if (highlighted)
    {
        // 高亮显示：添加黄色边框效果
        this->setColor(Color3B::YELLOW);
        auto blink = Blink::create(1.0f, 3);
        this->runAction(RepeatForever::create(blink));
    }
    else
    {
        this->stopAllActions();
        this->setColor(Color3B::WHITE);
    }
}

void CardView::setInteractable(bool interactable)
{
    this->setOpacity(interactable ? 255 : 128);
}

void CardView::initTouchEvents()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool CardView::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
    Size s = this->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode))
    {
        if (_clickCallback)
        {
            _clickCallback(_cardId);
        }
        return true;
    }
    return false;
}
