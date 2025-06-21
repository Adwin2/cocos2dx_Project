#include "GameLogicService.h"
#include "../configs/GameConfig.h"

bool GameLogicService::canMatch(const CardModel& card1, const CardModel& card2)
{
    // 基本匹配规则：无关花色，数值相差一即可消除
    return abs(card1.getValue() - card2.getValue()) == 1;
}

bool GameLogicService::processCardMatch(GameModel& gameModel, int mainCardId)
{
    CardModel* mainCard = gameModel.getMainCardById(mainCardId);
    if (!mainCard)
    {
        return false;
    }

    // 检查卡牌是否可以点击
    if (!isCardClickable(gameModel, mainCardId))
    {
        return false;
    }

    const CardModel& bottomCard = gameModel.getBottomCard();

    if (canMatch(*mainCard, bottomCard))
    {
        return true;
    }

    return false;
}

bool GameLogicService::isCardClickable(const GameModel& gameModel, int cardId)
{
    const CardModel* card = gameModel.getMainCardById(cardId);
    if (!card)
    {
        return false;
    }

    return gameModel.isCardClickable(*card);
}
