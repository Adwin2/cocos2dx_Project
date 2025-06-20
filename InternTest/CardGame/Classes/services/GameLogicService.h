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
     * 检查游戏是否结束
     * @param gameModel 游戏数据模型
     * @return 游戏是否结束
     */
    static bool checkGameOver(const GameModel& gameModel);
    
    /**
     * 计算匹配得分
     * @param card1 第一张卡牌
     * @param card2 第二张卡牌
     * @param level 当前关卡
     * @return 得分
     */
    static int calculateMatchScore(const CardModel& card1, const CardModel& card2, int level);
    
    /**
     * 检查是否有可用的匹配
     * @param gameModel 游戏数据模型
     * @return 是否有可用匹配
     */
    static bool hasAvailableMatches(const GameModel& gameModel);
    
    /**
     * 获取所有可能的匹配
     * @param gameModel 游戏数据模型
     * @return 可匹配的卡牌ID列表
     */
    static std::vector<int> getAvailableMatches(const GameModel& gameModel);

private:
    GameLogicService() = delete;  // 禁止实例化
};

#endif // __GAME_LOGIC_SERVICE_H__
