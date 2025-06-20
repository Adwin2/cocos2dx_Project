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
        static const int MAIN_CARDS_COUNT = 9;          // 主牌区卡牌数量
        static const int GRID_ROWS = 3;                 // 网格行数
        static const int GRID_COLS = 3;                 // 网格列数
        static const int MATCH_SCORE = 10;              // 匹配得分
        static const float CARD_ANIMATION_DURATION;     // 卡牌动画时长
    };
    
    // 卡牌配置
    struct CardSettings
    {
        static const float CARD_WIDTH;                  // 卡牌宽度
        static const float CARD_HEIGHT;                 // 卡牌高度
        static const float CARD_SPACING;                // 卡牌间距
    };
    
    // UI配置
    struct UISettings
    {
        static const float SCORE_LABEL_FONT_SIZE;       // 分数标签字体大小
        static const float INSTRUCTION_FONT_SIZE;       // 说明文字字体大小
        static const float MENU_FONT_SIZE;              // 菜单字体大小
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
        static const Vec2 MAIN_CARDS_START_OFFSET;      // 主牌区起始偏移
        static const Vec2 SCORE_LABEL_OFFSET;           // 分数标签偏移
        static const Vec2 INSTRUCTION_OFFSET;           // 说明文字偏移
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
