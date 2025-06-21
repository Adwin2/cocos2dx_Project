#ifndef __GAME_LOGIC_SERVICE_H__
#define __GAME_LOGIC_SERVICE_H__

#include "../models/CardModel.h"
#include "../models/GameModel.h"

/**
 * 游戏逻辑服务
 * 职责：提供无状态的游戏逻辑处理服务，不持有数据
 */
class GameLogicService
{
public:
    /**
     * 检查两张卡牌是否可以匹配
     * @param card1 第一张卡牌
     * @param card2 第二张卡牌
     * @return 是否可以匹配
     */
    static bool canMatch(const CardModel& card1, const CardModel& card2);
    
    /**
     * 处理卡牌匹配逻辑
     * @param gameModel 游戏数据模型
     * @param mainCardId 主牌区卡牌ID
     * @return 是否匹配成功
     */
    static bool processCardMatch(GameModel& gameModel, int mainCardId);
    
    /**
     * 检查卡牌是否可点击
     * @param gameModel 游戏数据模型
     * @param cardId 卡牌ID
     * @return 是否可点击
     */
    static bool isCardClickable(const GameModel& gameModel, int cardId);

private:
    GameLogicService() = delete;  // 禁止实例化
};

#endif // __GAME_LOGIC_SERVICE_H__
