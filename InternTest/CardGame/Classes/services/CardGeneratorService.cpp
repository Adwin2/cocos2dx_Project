#include "CardGeneratorService.h"
#include "../configs/GameConfig.h"
#include <algorithm>
#include <random>
#include <chrono>

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

void CardGeneratorService::generateInitialCards(GameModel& gameModel, int mainCardCount)
{
    // 生成底牌
    CardModel bottomCard = generateRandomCard(gameModel.getNextCardId());
    gameModel.setBottomCard(bottomCard);
    
    // 生成主牌区卡牌
    std::vector<CardModel> mainCards = generateRandomCards(mainCardCount, gameModel.getNextCardId());
    gameModel.setMainCards(mainCards);
}

void CardGeneratorService::generateNewBottomCard(GameModel& gameModel)
{
    CardModel newBottomCard = generateRandomCard(gameModel.getNextCardId());
    gameModel.setBottomCard(newBottomCard);
}

CardModel CardGeneratorService::generateCard(CardModel::Suit suit, CardModel::Value value, int id)
{
    return CardModel(suit, value, id);
}

std::vector<CardModel> CardGeneratorService::generateFullDeck(int startId)
{
    std::vector<CardModel> deck;
    deck.reserve(52);
    
    int currentId = startId;
    
    // 生成四种花色，每种13张牌
    for (int suit = CardModel::HEARTS; suit <= CardModel::SPADES; suit++)
    {
        for (int value = CardModel::ACE; value <= CardModel::KING; value++)
        {
            deck.push_back(CardModel(
                static_cast<CardModel::Suit>(suit),
                static_cast<CardModel::Value>(value),
                currentId++
            ));
        }
    }
    
    return deck;
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
