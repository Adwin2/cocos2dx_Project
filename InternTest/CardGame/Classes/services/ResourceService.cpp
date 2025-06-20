#include "ResourceService.h"

// 资源路径常量定义
const std::string ResourceService::Paths::CARD_BACKGROUND = "res/card_general.png";
const std::string ResourceService::Paths::CARD_BACK = "res/card_general.png";  // 暂时使用同一张图片作为背面

const std::string ResourceService::Paths::SUIT_HEARTS = "res/suits/heart.png";
const std::string ResourceService::Paths::SUIT_DIAMONDS = "res/suits/diamond.png";
const std::string ResourceService::Paths::SUIT_CLUBS = "res/suits/club.png";
const std::string ResourceService::Paths::SUIT_SPADES = "res/suits/spade.png";

const std::string ResourceService::Paths::NUMBER_DIR = "res/number/";

const std::string ResourceService::Paths::FONT_ARIAL = "fonts/arial.ttf";
const std::string ResourceService::Paths::FONT_MARKER = "fonts/Marker Felt.ttf";

const std::string ResourceService::Paths::CLOSE_NORMAL = "CloseNormal.png";
const std::string ResourceService::Paths::CLOSE_SELECTED = "CloseSelected.png";

std::string ResourceService::getCardValueImagePath(CardModel::Value value, bool isRed, bool isLarge)
{
    std::string prefix = isLarge ? "big_" : "small_";
    std::string color = isRed ? "red_" : "black_";
    std::string valueStr = getValueString(value);

    return Paths::NUMBER_DIR + prefix + color + valueStr + ".png";
}

std::string ResourceService::getSuitImagePath(CardModel::Suit suit)
{
    switch (suit)
    {
        case CardModel::HEARTS:
            return Paths::SUIT_HEARTS;
        case CardModel::DIAMONDS:
            return Paths::SUIT_DIAMONDS;
        case CardModel::CLUBS:
            return Paths::SUIT_CLUBS;
        case CardModel::SPADES:
            return Paths::SUIT_SPADES;
        default:
            return Paths::SUIT_HEARTS;
    }
}

ResourceService::CardResources ResourceService::getCardResources(const CardModel& cardModel)
{
    CardResources resources;
    
    if (cardModel.isFaceUp())
    {
        resources.backgroundPath = Paths::CARD_BACKGROUND;
        resources.valuePath = getCardValueImagePath(cardModel.getValue(), cardModel.isRed(), true);
        resources.suitPath = getSuitImagePath(cardModel.getSuit());
    }
    else
    {
        resources.backgroundPath = Paths::CARD_BACK;
        resources.valuePath = "";  // 背面不显示数值
        resources.suitPath = "";   // 背面不显示花色
    }
    
    return resources;
}

void ResourceService::preloadCardResources()
{
    auto textureCache = Director::getInstance()->getTextureCache();
    
    // 预加载卡牌背景
    textureCache->addImage(Paths::CARD_BACKGROUND);
    textureCache->addImage(Paths::CARD_BACK);
    
    // 预加载花色图片
    textureCache->addImage(Paths::SUIT_HEARTS);
    textureCache->addImage(Paths::SUIT_DIAMONDS);
    textureCache->addImage(Paths::SUIT_CLUBS);
    textureCache->addImage(Paths::SUIT_SPADES);
    
    // 预加载所有数值图片
    std::vector<CardModel::Value> values = {
        CardModel::ACE, CardModel::TWO, CardModel::THREE, CardModel::FOUR,
        CardModel::FIVE, CardModel::SIX, CardModel::SEVEN, CardModel::EIGHT,
        CardModel::NINE, CardModel::TEN, CardModel::JACK, CardModel::QUEEN, CardModel::KING
    };
    
    for (auto value : values)
    {
        // 预加载大尺寸红色和黑色数字
        textureCache->addImage(getCardValueImagePath(value, true, true));
        textureCache->addImage(getCardValueImagePath(value, false, true));
        
        // 预加载小尺寸红色和黑色数字
        textureCache->addImage(getCardValueImagePath(value, true, false));
        textureCache->addImage(getCardValueImagePath(value, false, false));
    }
}

bool ResourceService::isResourceExists(const std::string& path)
{
    return FileUtils::getInstance()->isFileExist(path);
}

std::string ResourceService::getValueString(CardModel::Value value)
{
    switch (value)
    {
        case CardModel::ACE: return "A";
        case CardModel::TWO: return "2";
        case CardModel::THREE: return "3";
        case CardModel::FOUR: return "4";
        case CardModel::FIVE: return "5";
        case CardModel::SIX: return "6";
        case CardModel::SEVEN: return "7";
        case CardModel::EIGHT: return "8";
        case CardModel::NINE: return "9";
        case CardModel::TEN: return "10";
        case CardModel::JACK: return "J";
        case CardModel::QUEEN: return "Q";
        case CardModel::KING: return "K";
        default: return "A";
    }
}
