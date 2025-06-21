#include "GameController.h"
#include "../services/GameLogicService.h"
#include "../services/CardGeneratorService.h"
#include "../configs/GameConfig.h"

GameController::GameController()
    : _gameModel(nullptr), _cardViewManager(nullptr)
{
    _gameModel = new GameModel();
}

GameController::~GameController()
{
    CC_SAFE_DELETE(_gameModel);
    CC_SAFE_DELETE(_cardViewManager);
}

void GameController::init(Node* parentNode)
{
    // 创建卡牌视图管理器
    _cardViewManager = new CardViewManager(parentNode);
    
    // 设置卡牌点击回调
    _cardViewManager->setCardClickCallback([this](int cardId) {
        onCardClicked(cardId);
    });
}

void GameController::startNewGame()
{
    // 重置游戏模型
    _gameModel->reset();
    
    // 初始化游戏数据
    initGameData();
    
    // 更新视图
    updateViews();
}

void GameController::restartGame()
{
    startNewGame();
}

void GameController::pauseGame()
{
    if (_gameModel->getGameState() == GameModel::PLAYING)
    {
        _gameModel->setGameState(GameModel::PAUSED);
    }
}

void GameController::resumeGame()
{
    if (_gameModel->getGameState() == GameModel::PAUSED)
    {
        _gameModel->setGameState(GameModel::PLAYING);
    }
}

void GameController::onCardClicked(int cardId)
{
    // 只有在游戏进行中才处理点击
    if (_gameModel->getGameState() != GameModel::PLAYING)
    {
        return;
    }

    // 检查是否点击的是当前底牌
    if (cardId == _gameModel->getBottomCard().getId())
    {
        handleBottomCardClick(cardId);
        return;
    }

    // 检查是否点击的是备用底牌
    if (cardId == _gameModel->getSpareCard().getId())
    {
        handleSpareCardClick(cardId);
        return;
    }

    // 点击的是主牌区的卡牌，进行匹配逻辑
    handleCardMatch(cardId);
}

void GameController::initGameData()
{
    // 使用卡牌生成服务初始化卡牌
    CardGeneratorService::generateInitialCards(*_gameModel, GameConfig::GameSettings::MAIN_CARDS_COUNT,
                                               GameConfig::GameSettings::BOTTOM_CARDS_COUNT,
                                               GameConfig::GameSettings::SPARE_CARDS_COUNT);
}

void GameController::updateViews()
{
    // 更新卡牌视图
    _cardViewManager->updateFromGameModel(*_gameModel);

    // 更新卡牌可点击状态
    _cardViewManager->updateCardClickableStates(*_gameModel);
}

void GameController::handleCardMatch(int cardId)
{
    // 使用游戏逻辑服务处理匹配
    bool matchSuccess = GameLogicService::processCardMatch(*_gameModel, cardId);
    
    if (matchSuccess)
    {
        handleMatchSuccess(cardId);
    }
    else
    {
        handleMatchFailure(cardId);
    }
}

void GameController::handleMatchSuccess(int cardId)
{
    // 新逻辑：匹配成功的主牌移动到底牌区（类似备用牌点击行为）
    CardView* matchedCardView = _cardViewManager->getCardView(cardId);
    if (!matchedCardView)
    {
        return;
    }

    // 预设值
    matchedCardView->setLocalZOrder(20); 

    // 执行数据操作：将匹配的主牌添加到底牌栈
    // 获取主牌数据
    const CardModel* mainCard = _gameModel->getMainCardById(cardId);
    if (mainCard)
    {
        // 创建新的底牌并添加到底牌栈
        CardModel newBottomCard = *mainCard;
        newBottomCard.setFaceUp(true); // 底牌应该正面朝上
        _gameModel->addToBottomStack(newBottomCard);

        // 从主牌栈移除（统一栈式接口）
        _gameModel->removeFromMainStack(cardId);
    }

    // 计算目标位置（底牌区）
    Vec2 targetPosition = _cardViewManager->calculateBottomCardPosition();

    // 创建移动动画
    float duration = GameConfig::AnimationSettings::FLIP_ANIMATION_DURATION;
    auto moveAction = MoveTo::create(duration, targetPosition);

    // 翻牌动作（保持正面朝上，因为是底牌）
    auto flipAction = CallFunc::create([matchedCardView]() {
        matchedCardView->setFaceUp(true);
        matchedCardView->updateDisplay();
        matchedCardView->setLocalZOrder(10); 
    });

    // 动画完成后的回调
    auto completeAction = CallFunc::create([this]() {
        // 同步状态，不重建视图
        _cardViewManager->syncCardStatesOnly(*_gameModel);

        // 检查游戏是否结束 - DEMO模式：注释掉游戏结束判定
        // checkGameEnd();
    });

    // 组合动画序列
    auto sequence = Sequence::create(
        Spawn::create(moveAction, flipAction, nullptr),
        completeAction,
        nullptr
    );

    matchedCardView->runAction(sequence);
}

void GameController::handleMatchFailure(int cardId)
{
    // 播放匹配失败动画
    _cardViewManager->playMismatchAnimation(cardId);
}

void GameController::handleBottomCardClick(int cardId)
{
    // 底牌点击：当前底牌回到备用区，露出下层底牌
    CardView* bottomCardView = _cardViewManager->getCardView(cardId);
    if (!bottomCardView)
    {
        return;
    }

    // 预设Z-order，确保卡牌的动画在栈顶显示
    bottomCardView->setLocalZOrder(20); // 设置高Z-order

    // 执行数据操作：底牌移动到备用栈
    _gameModel->moveBottomToSpare();

    // 计算目标位置（备用区栈顶）
    // 简化：使用固定的栈顶位置，后续可以根据实际栈大小调整
    Vec2 targetPosition = _cardViewManager->calculateSpareCardPosition();

    // 创建移动动画
    float duration = GameConfig::AnimationSettings::FLIP_ANIMATION_DURATION;
    auto moveAction = MoveTo::create(duration, targetPosition);

    // 翻牌动作（正面变背面）
    auto flipAction = CallFunc::create([bottomCardView]() {
        bottomCardView->flipCardWithAnimation([bottomCardView]() {
                bottomCardView->setLocalZOrder(10); // 动画完成后恢复正常Z-order
            }
        );
    });

    // 动画完成后的回调
    auto completeAction = CallFunc::create([this]() {
       _cardViewManager->syncCardStatesOnly(*_gameModel);
    });

    // 组合动画序列
    auto sequence = Sequence::create(
        Spawn::create(moveAction, flipAction, nullptr),
        completeAction,
        nullptr
    );

    bottomCardView->runAction(sequence);
}

void GameController::handleSpareCardClick(int cardId)
{
    // 备用牌点击：备用牌移动到底牌区，覆盖当前底牌
    CardView* spareCardView = _cardViewManager->getCardView(cardId);
    if (!spareCardView)
    {
        return;
    }

    // 预设Z-order，确保卡牌的动画在栈顶显示
    spareCardView->setLocalZOrder(20);

    // 执行数据操作：备用牌移动到底牌栈
    _gameModel->moveSpareToBottom();

    // 计算目标位置（底牌区栈顶）
    // 简化：直接使用底牌基础位置，避免插入到栈中间的视觉效果
    Vec2 targetPosition = _cardViewManager->calculateBottomCardPosition();

    // 创建移动动画
    float duration = GameConfig::AnimationSettings::FLIP_ANIMATION_DURATION;
    auto moveAction = MoveTo::create(duration, targetPosition);

    // 翻牌动作（背面变正面）
    auto flipAction = CallFunc::create([spareCardView]() {
        spareCardView->flipCardWithAnimation([spareCardView]() {
            spareCardView->setLocalZOrder(10); // 动画完成后恢复正常Z-order
        });
    });

    // 动画完成后的回调
    auto completeAction = CallFunc::create([this]() {
        // 同步必要的数据状态，避免中断正在进行的翻牌动画
        _cardViewManager->syncCardStatesOnly(*_gameModel);
    });

    // 组合动画序列
    auto sequence = Sequence::create(
        Spawn::create(moveAction, flipAction, nullptr),
        completeAction,
        nullptr
    );

    spareCardView->runAction(sequence);
}

// DEMO模式：暂无游戏结束判定，让游戏可以无限进行

