#include "CardModel.h"

CardModel::CardModel()
    : _suit(HEARTS), _value(ACE), _faceUp(true), _id(0), _layer(0), _gridIndex(0)
{
}

CardModel::CardModel(Suit suit, Value value, int id)
    : _suit(suit), _value(value), _faceUp(true), _id(id), _layer(0), _gridIndex(0)
{
}

CardModel::CardModel(Suit suit, Value value, int id, int layer, int gridIndex)
    : _suit(suit), _value(value), _faceUp(true), _id(id), _layer(layer), _gridIndex(gridIndex)
{
}

bool CardModel::isRed() const
{
    return _suit == HEARTS || _suit == DIAMONDS;
}

bool CardModel::isBlack() const
{
    return _suit == CLUBS || _suit == SPADES;
}

std::string CardModel::getValueString() const
{
    switch (_value)
    {
        case ACE: return "A";
        case JACK: return "J";
        case QUEEN: return "Q";
        case KING: return "K";
        default: return std::to_string(_value);
    }
}

std::string CardModel::getSuitString() const
{
    switch (_suit)
    {
        case HEARTS: return "♥";
        case DIAMONDS: return "♦";
        case CLUBS: return "♣";
        case SPADES: return "♠";
        default: return "";
    }
}

bool CardModel::operator==(const CardModel& other) const
{
    return _suit == other._suit && _value == other._value;
}

bool CardModel::operator!=(const CardModel& other) const
{
    return !(*this == other);
}
