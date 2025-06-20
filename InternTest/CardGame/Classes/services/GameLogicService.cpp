#include "GameLogicService.h"
#include "../configs/GameConfig.h"

bool GameLogicService::canMatch(const CardModel& card1, const CardModel& card2)
{
    // 基本匹配规则：相同数值或相同花色
    return (card1.getValue() == card2.getValue()) || (card1.getSuit() == card2.getSuit());
}

bool GameLogicService::processCardMatch(GameModel& gameModel, int mainCardId)
{
    CardModel* mainCard = gameModel.getMainCardById(mainCardId);
    if (!mainCard)
    {
        return false;
    }
    
    const CardModel& bottomCard = gameModel.getBottomCard();
    
    if (canMatch(*mainCard, bottomCard))
    {
        // 匹配成功，移除主牌区的卡牌
        gameModel.removeMainCardById(mainCardId);
        
        // 增加分数
        int score = calculateMatchScore(*mainCard, bottomCard, gameModel.getLevel());
        gameModel.addScore(score);
        
        // 增加移动次数
        gameModel.incrementMoves();
        
        // 检查游戏是否结束
        if (gameModel.getMainCardCount() == 0)
        {
            gameModel.setGameState(GameModel::WIN);
        }
        
        return true;
    }
    
    return false;
}

bool GameLogicService::checkGameOver(const GameModel& gameModel)
{
    // 如果主牌区没有卡牌了，游戏胜利
    if (gameModel.getMainCardCount() == 0)
    {
        return true;
    }
    
    // 如果没有可用的匹配，游戏失败
    if (!hasAvailableMatches(gameModel))
    {
        return true;
    }
    
    return false;
}

int GameLogicService::calculateMatchScore(const CardModel& card1, const CardModel& card2, int level)
{
    int baseScore = GameConfig::GameSettings::MATCH_SCORE;
    
    // 相同数值的匹配得分更高
    if (card1.getValue() == card2.getValue())
    {
        baseScore *= 2;
    }
    
    // 关卡越高，得分越高
    baseScore *= level;
    
    return baseScore;
}

bool GameLogicService::hasAvailableMatches(const GameModel& gameModel)
{
    const CardModel& bottomCard = gameModel.getBottomCard();
    const std::vector<CardModel>& mainCards = gameModel.getMainCards();
    
    for (const auto& card : mainCards)
    {
        if (canMatch(card, bottomCard))
        {
            return true;
        }
    }
    
    return false;
}

std::vector<int> GameLogicService::getAvailableMatches(const GameModel& gameModel)
{
    std::vector<int> matches;
    const CardModel& bottomCard = gameModel.getBottomCard();
    const std::vector<CardModel>& mainCards = gameModel.getMainCards();
    
    for (const auto& card : mainCards)
    {
        if (canMatch(card, bottomCard))
        {
            matches.push_back(card.getId());
        }
    }
    
    return matches;
}
