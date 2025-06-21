#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__

#include "cocos2d.h"

USING_NS_CC;

/**
 * 游戏静态配置类
 * 职责：存储所有游戏相关的静态配置数据
 */
class GameConfig
{
public:
    // 游戏基础配置
    struct GameSettings
    {
        static const int MAIN_CARDS_COUNT;          // 主牌区卡牌数量
        static const int BOTTOM_CARDS_COUNT;        // 底牌数量
        static const int SPARE_CARDS_COUNT;         // 备用底牌数量
    };

    // 动画系统配置
    struct AnimationSettings
    {
        static const float FLIP_ANIMATION_DURATION;    // 翻牌动画时长
        static const float FLIP_SCALE_FACTOR;          // 翻牌缩放因子
        static const bool AUTO_FLIP_ENABLED;           // 是否启用自动翻牌
        static const float BOUNCE_SCALE_FACTOR;        // 弹跳缩放因子
        static const float BOUNCE_DURATION;            // 弹跳动画时长
    };
    
    // 卡牌配置
    struct CardSettings
    {
        static const float CARD_WIDTH;                  // 卡牌宽度
        static const float CARD_HEIGHT;                 // 卡牌高度
    };
    
    // UI配置
    struct UISettings
    {
        static const float MENU_FONT_SIZE;              // 菜单字体大小
        static const int   MAIN_CARD_COLS;
        static const int   MAIN_CARD_ROWS;
        static const float   MAIN_CARD_SPACING; 
    };
    
    // 颜色配置
    struct ColorSettings
    {
        static const Color3B BACKGROUND_COLOR;          // 背景颜色
        static const Color3B RED_CARD_COLOR;            // 红色卡牌颜色
        static const Color3B BLACK_CARD_COLOR;          // 黑色卡牌颜色
        static const Color3B CARD_BACK_COLOR;           // 卡牌背面颜色
        static const Color3B SCORE_TEXT_COLOR;          // 分数文字颜色
        static const Color3B WIN_TEXT_COLOR;            // 胜利文字颜色
    };
    
    // 位置配置
    struct PositionSettings
    {
        static const Vec2 BOTTOM_CARD_OFFSET;           // 底牌偏移位置
        static const Vec2 MAIN_CARDS_OFFSET;            // 主牌偏移位置，从屏幕中央开始计算
        static const Vec2 SPARE_CARD_OFFSET;            // 备用底牌偏移位置
    };
    
    // 资源路径配置
    struct ResourcePaths
    {
        static const std::string FONT_PATH;             // 字体路径
        static const std::string CLOSE_NORMAL_IMAGE;    // 关闭按钮普通状态
        static const std::string CLOSE_SELECTED_IMAGE;  // 关闭按钮选中状态
    };
    
private:
    GameConfig() = delete;  // 禁止实例化
};

#endif // __GAME_CONFIG_H__
