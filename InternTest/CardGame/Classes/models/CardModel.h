#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include <string>

/**
 * 卡牌数据模型
 * 职责：存储卡牌的核心数据，不包含显示逻辑
 */
class CardModel
{
public:
    // 卡牌花色
    enum Suit {
        HEARTS = 0,    // 红桃
        DIAMONDS,      // 方块
        CLUBS,         // 梅花
        SPADES         // 黑桃
    };
    
    // 卡牌数值
    enum Value {
        ACE = 1,
        TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
        JACK = 11, QUEEN = 12, KING = 13
    };

private:
    Suit _suit;        // 花色
    Value _value;      // 数值
    bool _faceUp;      // 是否正面朝上
    int _id;           // 卡牌唯一标识
    int _layer;        // 所在层级 (0=第一层, 1=第二层, 2=第三层)
    int _gridIndex;    // 在网格中的索引位置

public:
    CardModel();
    CardModel(Suit suit, Value value, int id = 0);
    CardModel(Suit suit, Value value, int id, int layer, int gridIndex);

    // Getter方法
    Suit getSuit() const { return _suit; }
    Value getValue() const { return _value; }
    bool isFaceUp() const { return _faceUp; }
    int getId() const { return _id; }
    int getLayer() const { return _layer; }
    int getGridIndex() const { return _gridIndex; }

    // Setter方法
    void setSuit(Suit suit) { _suit = suit; }
    void setValue(Value value) { _value = value; }
    void setFaceUp(bool faceUp) { _faceUp = faceUp; }
    void setId(int id) { _id = id; }
    void setLayer(int layer) { _layer = layer; }
    void setGridIndex(int gridIndex) { _gridIndex = gridIndex; }
    
    // 翻牌
    void flip() { _faceUp = !_faceUp; }
    
    // 获取卡牌颜色
    bool isRed() const;
    bool isBlack() const;
    
    // 获取数值字符串表示
    std::string getValueString() const;
    
    // 比较操作
    bool operator==(const CardModel& other) const;
    bool operator!=(const CardModel& other) const;
};

#endif // __CARD_MODEL_H__
