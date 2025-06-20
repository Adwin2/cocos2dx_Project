#ifndef __CARD_GENERATOR_SERVICE_H__
#define __CARD_GENERATOR_SERVICE_H__

#include "../models/CardModel.h"
#include "../models/GameModel.h"
#include <vector>
#include <random>

/**
 * 卡牌生成服务
 * 职责：提供卡牌生成相关的无状态服务
 */
class CardGeneratorService
{
public:
    /**
     * 生成随机卡牌
     * @param id 卡牌ID
     * @return 随机生成的卡牌
     */
    static CardModel generateRandomCard(int id = 0);
    
    /**
     * 生成指定数量的随机卡牌
     * @param count 卡牌数量
     * @param startId 起始ID
     * @return 卡牌列表
     */
    static std::vector<CardModel> generateRandomCards(int count, int startId = 1);
    
    /**
     * 为游戏模型生成初始卡牌
     * @param gameModel 游戏数据模型
     * @param mainCardCount 主牌区卡牌数量
     */
    static void generateInitialCards(GameModel& gameModel, int mainCardCount);
    
    /**
     * 生成新的底牌
     * @param gameModel 游戏数据模型
     */
    static void generateNewBottomCard(GameModel& gameModel);
    
    /**
     * 生成指定花色和数值的卡牌
     * @param suit 花色
     * @param value 数值
     * @param id 卡牌ID
     * @return 生成的卡牌
     */
    static CardModel generateCard(CardModel::Suit suit, CardModel::Value value, int id = 0);
    
    /**
     * 生成一副完整的扑克牌（不包含大小王）
     * @param startId 起始ID
     * @return 完整的扑克牌列表
     */
    static std::vector<CardModel> generateFullDeck(int startId = 1);
    
    /**
     * 洗牌算法
     * @param cards 要洗的卡牌列表
     */
    static void shuffleCards(std::vector<CardModel>& cards);

private:
    CardGeneratorService() = delete;  // 禁止实例化
    
    /**
     * 生成随机花色
     * @return 随机花色
     */
    static CardModel::Suit generateRandomSuit();
    
    /**
     * 生成随机数值
     * @return 随机数值
     */
    static CardModel::Value generateRandomValue();
};

#endif // __CARD_GENERATOR_SERVICE_H__
