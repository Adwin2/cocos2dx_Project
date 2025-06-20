#ifndef __RESOURCE_SERVICE_H__
#define __RESOURCE_SERVICE_H__

#include "cocos2d.h"
#include "../models/CardModel.h"
#include <string>

USING_NS_CC;

/**
 * 资源管理服务
 * 职责：提供游戏资源路径管理和加载服务
 */
class ResourceService
{
public:
    // 资源路径常量
    struct Paths
    {
        // 卡牌相关
        static const std::string CARD_BACKGROUND;           // 卡牌背景
        static const std::string CARD_BACK;                 // 卡牌背面
        
        // 花色图标路径
        static const std::string SUIT_HEARTS;              // 红桃
        static const std::string SUIT_DIAMONDS;            // 方块
        static const std::string SUIT_CLUBS;               // 梅花
        static const std::string SUIT_SPADES;              // 黑桃
        
        // 数字资源目录
        static const std::string NUMBER_DIR;               // 数字目录
        
        // 字体
        static const std::string FONT_ARIAL;               // Arial字体
        static const std::string FONT_MARKER;              // Marker字体
        
        // UI资源
        static const std::string CLOSE_NORMAL;             // 关闭按钮普通状态
        static const std::string CLOSE_SELECTED;           // 关闭按钮选中状态
    };
    
    /**
     * 获取卡牌数值图片路径
     * @param value 卡牌数值
     * @param isRed 是否为红色
     * @param isLarge 是否为大尺寸
     * @return 图片路径
     */
    static std::string getCardValueImagePath(CardModel::Value value, bool isRed, bool isLarge = true);
    
    /**
     * 获取花色图片路径
     * @param suit 花色
     * @return 图片路径
     */
    static std::string getSuitImagePath(CardModel::Suit suit);
    
    /**
     * 获取完整卡牌的资源路径组合
     * @param cardModel 卡牌模型
     * @return 包含背景、数值、花色路径的结构
     */
    struct CardResources
    {
        std::string backgroundPath;     // 背景图片路径
        std::string valuePath;          // 数值图片路径
        std::string suitPath;           // 花色图片路径
    };
    
    static CardResources getCardResources(const CardModel& cardModel);
    
    /**
     * 预加载所有卡牌资源
     */
    static void preloadCardResources();
    
    /**
     * 检查资源文件是否存在
     * @param path 资源路径
     * @return 是否存在
     */
    static bool isResourceExists(const std::string& path);

private:
    ResourceService() = delete;  // 禁止实例化
    
    /**
     * 获取数值字符串
     * @param value 卡牌数值
     * @return 数值字符串
     */
    static std::string getValueString(CardModel::Value value);
};

#endif // __RESOURCE_SERVICE_H__
