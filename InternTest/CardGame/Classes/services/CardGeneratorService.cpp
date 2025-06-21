#include "CardGeneratorService.h"
#include "../configs/GameConfig.h"
#include <algorithm>
#include <random>
#include <chrono>

// 
CardModel CardGeneratorService::generateRandomCard(int id)
{
    CardModel::Suit suit = generateRandomSuit();
    CardModel::Value value = generateRandomValue();
    return CardModel(suit, value, id);
}

std::vector<CardModel> CardGeneratorService::generateRandomCards(int count, int startId)
{
    std::vector<CardModel> cards;
    cards.reserve(count);
    
    for (int i = 0; i < count; i++)
    {
        cards.push_back(generateRandomCard(startId + i));
    }
    
    return cards;
}

void CardGeneratorService::generateInitialCards(GameModel& gameModel, int mainCardCount, int bottomCardCount, int spareCardCount)
{
    // 生成初始底牌区（缺省：1张卡牌）
    addRandomCardbyNumAndWhere(bottomCardCount, _Bottom, gameModel);
    // 生成初始备用牌区（缺省：2张卡牌）
    addRandomCardbyNumAndWhere(spareCardCount, _Spare, gameModel);
    // 生成主牌区 （缺省：9张卡牌）
    addRandomCardbyNumAndWhere(mainCardCount, _Main, gameModel);
}

// 指定添加牌的数量和位置
void CardGeneratorService::addRandomCardbyNumAndWhere(int num, int where, GameModel& gameModel)
{
    if (where == 1) {
        for (int i = 0; i < num; i++) {
            CardModel card = generateRandomCard(gameModel.getNextCardId());
            gameModel.addToBottomStack(card);
        }
    } else if (where == 2) {
        for (int i = 0; i < num; i++) {
            CardModel card = generateRandomCard(gameModel.getNextCardId());
            gameModel.addToSpareStack(card);
        }
    }else {
        for (int i = 0; i < num; i++) {
        CardModel card = generateRandomCard(gameModel.getNextCardId());
        gameModel.addToMainStack(card);
        }    
    }
}

// 生成指定花色和数值的卡牌，便于测试匹配逻辑
CardModel CardGeneratorService::generateCard(CardModel::Suit suit, CardModel::Value value, int id)
{
    return CardModel(suit, value, id);
}

void CardGeneratorService::shuffleCards(std::vector<CardModel>& cards)
{
    // 使用当前时间作为随机种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    
    std::shuffle(cards.begin(), cards.end(), generator);
}

CardModel::Suit CardGeneratorService::generateRandomSuit()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 3);
    
    return static_cast<CardModel::Suit>(dis(gen));
}

CardModel::Value CardGeneratorService::generateRandomValue()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 13);
    
    return static_cast<CardModel::Value>(dis(gen));
}
