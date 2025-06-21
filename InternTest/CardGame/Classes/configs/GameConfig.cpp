#include "GameConfig.h"

// 游戏基础配置实现
const int GameConfig::GameSettings::MAIN_CARDS_COUNT = 9;
const int GameConfig::GameSettings::BOTTOM_CARDS_COUNT = 1;
const int GameConfig::GameSettings::SPARE_CARDS_COUNT = 2;

// 动画配置实现
const float GameConfig::AnimationSettings::FLIP_ANIMATION_DURATION = 0.6f;  // 增加动画时长，更流畅
const float GameConfig::AnimationSettings::FLIP_SCALE_FACTOR = 0.0f;
const bool GameConfig::AnimationSettings::AUTO_FLIP_ENABLED = true;
const float GameConfig::AnimationSettings::BOUNCE_SCALE_FACTOR = 1.05f;     // 弹跳时的缩放比例
const float GameConfig::AnimationSettings::BOUNCE_DURATION = 0.1f;          // 弹跳动画时长

// 卡牌配置实现
const float GameConfig::CardSettings::CARD_WIDTH = 80.0f;
const float GameConfig::CardSettings::CARD_HEIGHT = 120.0f;

// UI配置实现
const float GameConfig::UISettings::MENU_FONT_SIZE = 20.0f;
const int GameConfig::UISettings::MAIN_CARD_COLS = 5;
const int GameConfig::UISettings::MAIN_CARD_ROWS = 3;
const float GameConfig::UISettings::MAIN_CARD_SPACING = 150.0f;

// 颜色配置实现 初始测试牌背使用的颜色配置
const Color3B GameConfig::ColorSettings::BACKGROUND_COLOR = Color3B(0, 100, 0);

// 位置配置实现
const Vec2 GameConfig::PositionSettings::BOTTOM_CARD_OFFSET = Vec2(960, -250);
const Vec2 GameConfig::PositionSettings::MAIN_CARDS_OFFSET = Vec2(0, 200);
const Vec2 GameConfig::PositionSettings::SPARE_CARD_OFFSET = Vec2(700, -250);

// 资源路径配置实现
const std::string GameConfig::ResourcePaths::FONT_PATH = "fonts/arial.ttf"; // MVP版本使用arial字体，支持英文
const std::string GameConfig::ResourcePaths::CLOSE_NORMAL_IMAGE = "CloseNormal.png";
const std::string GameConfig::ResourcePaths::CLOSE_SELECTED_IMAGE = "CloseSelected.png";
