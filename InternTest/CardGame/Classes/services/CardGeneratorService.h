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
     * @param bottomCardCount 底牌区卡牌数量
     * @param spareCardCount 备用牌区卡牌数量
      
     */
    static void generateInitialCards(GameModel& gameModel, int mainCardCount, int bottomCardCount, int spareCardCount);
    
    /**
     * 添加一张随机主牌到主牌栈（统一栈式接口）
     * @param gameModel 游戏数据模型
     */
    static void addRandomCardbyNumAndWhere(int num, int where, GameModel& gameModel);

    /**
     * 生成指定花色和数值的卡牌
     * @param suit 花色
     * @param value 数值
     * @param id 卡牌ID
     * @return 生成的卡牌
     */
    static CardModel generateCard(CardModel::Suit suit, CardModel::Value value, int id = 0);
    
    /**
     * 洗牌算法
     * @param cards 要洗的卡牌列表
     */
    static void shuffleCards(std::vector<CardModel>& cards);

private:
    CardGeneratorService() = delete;  // 禁止实例化
    
    static const int _Bottom = 1;
    static const int _Spare = 2;
    static const int _Main = 3;
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
