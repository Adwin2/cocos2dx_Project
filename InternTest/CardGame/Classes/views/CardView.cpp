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
    
    // // 创建中央花色精灵
    // _suitSprite = Sprite::create();
    // _suitSprite->setPosition(cardSize.width * 0.5f, cardSize.height * 0.3f);
    // this->addChild(_suitSprite, 2);
    
    // 创建左上角小数值精灵
    _smallValueSprite = Sprite::create();
    _smallValueSprite->setPosition(cardSize.width * 0.15f, cardSize.height * 0.85f);
    this->addChild(_smallValueSprite, 2);
    
    // 创建右上角小花色精灵
    _smallSuitSprite = Sprite::create();
    _smallSuitSprite->setPosition(cardSize.width * 0.85f, cardSize.height * 0.85f);
    this->addChild(_smallSuitSprite, 2);
}

void CardView::updateSprites()
{
    auto resources = ResourceService::getCardResources(_cardModel);

    if (_cardModel.isFaceUp())
    {
        // 正面显示 - 使用卡牌背景和内容
        this->setTexture(resources.backgroundPath);
        this->setColor(Color3B::WHITE);  // 正面显示原色

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
        // 背面显示 - 使用专门的背面图片
        this->setTexture(ResourceService::Paths::CARD_BACK);
        this->setColor(Color3B::WHITE);  // 背面也使用原色，让背面图片自然显示

        // 隐藏所有内容精灵
        if (_valueSprite) _valueSprite->setVisible(false);
        if (_suitSprite) _suitSprite->setVisible(false);
        if (_smallValueSprite) _smallValueSprite->setVisible(false);
        if (_smallSuitSprite) _smallSuitSprite->setVisible(false);
    }
}

void CardView::flipCard()
{
    // 简单翻牌，无动画
    _cardModel.flip();
    updateDisplay();
}

void CardView::flipCardWithAnimation(const std::function<void()>& callback)
{
    // 优化的翻牌动画 - 3D翻转效果
    float duration = GameConfig::AnimationSettings::FLIP_ANIMATION_DURATION;
    float halfDuration = duration * 0.5f;

    // 第一阶段：缩放到0（模拟翻转到侧面）
    auto scaleDown = ScaleTo::create(halfDuration, 0.0f, 1.0f);
    auto easeIn = EaseInOut::create(scaleDown, 2.0f);

    // 翻牌动作
    auto flipAction = CallFunc::create([this]() {
        _cardModel.flip();
        updateDisplay();
    });

    // 第二阶段：从0缩放回来（模拟翻转完成）
    auto scaleUp = ScaleTo::create(halfDuration, 1.0f, 1.0f);
    auto easeOut = EaseInOut::create(scaleUp, 2.0f);

    // 添加轻微的弹跳效果
    float bounceDuration = GameConfig::AnimationSettings::BOUNCE_DURATION;
    float bounceScale = GameConfig::AnimationSettings::BOUNCE_SCALE_FACTOR;
    auto bounce = Sequence::create(
        ScaleTo::create(bounceDuration, bounceScale, bounceScale),
        ScaleTo::create(bounceDuration, 1.0f, 1.0f),
        nullptr
    );

    // 回调动作
    auto callbackAction = callback ? CallFunc::create(callback) : nullptr;

    // 组合完整动画序列
    Action* sequence;
    if (callbackAction)
    {
        sequence = Sequence::create(easeIn, flipAction, easeOut, bounce, callbackAction, nullptr);
    }
    else
    {
        sequence = Sequence::create(easeIn, flipAction, easeOut, bounce, nullptr);
    }

    // 添加标签以便可以停止动画
    this->stopActionByTag(100);  // 停止之前的翻牌动画
    sequence->setTag(100);
    this->runAction(sequence);
}

void CardView::flipCardWithAnimationAndSound(const std::function<void()>& callback)
{
    // 带音效的翻牌动画（预留接口，可以在这里添加音效播放）
    // TODO: 添加翻牌音效
    // AudioEngine::play2d("sounds/card_flip.mp3", false, 0.5f);

    flipCardWithAnimation(callback);
}

void CardView::setFaceUp(bool faceUp)
{
    _cardModel.setFaceUp(faceUp);
    updateDisplay();
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

void CardView::setInteractable(bool interactable)
{
    // 移除不透明度设置，避免影响观感
    // 可交互状态通过其他方式表示（如高亮、动画等）
    // this->setOpacity(interactable ? 255 : 128);
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
