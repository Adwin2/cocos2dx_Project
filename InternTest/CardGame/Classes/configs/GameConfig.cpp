#include "GameConfig.h"

// 游戏基础配置实现
const float GameConfig::GameSettings::CARD_ANIMATION_DURATION = 0.5f;

// 卡牌配置实现
const float GameConfig::CardSettings::CARD_WIDTH = 80.0f;
const float GameConfig::CardSettings::CARD_HEIGHT = 120.0f;
const float GameConfig::CardSettings::CARD_SPACING = 100.0f;

// UI配置实现
const float GameConfig::UISettings::SCORE_LABEL_FONT_SIZE = 24.0f;
const float GameConfig::UISettings::INSTRUCTION_FONT_SIZE = 18.0f;
const float GameConfig::UISettings::MENU_FONT_SIZE = 20.0f;

// 颜色配置实现
const Color3B GameConfig::ColorSettings::BACKGROUND_COLOR = Color3B(0, 100, 0);
const Color3B GameConfig::ColorSettings::RED_CARD_COLOR = Color3B::RED;
const Color3B GameConfig::ColorSettings::BLACK_CARD_COLOR = Color3B::BLACK;
const Color3B GameConfig::ColorSettings::CARD_BACK_COLOR = Color3B::BLUE;
const Color3B GameConfig::ColorSettings::SCORE_TEXT_COLOR = Color3B::WHITE;
const Color3B GameConfig::ColorSettings::WIN_TEXT_COLOR = Color3B::YELLOW;

// 位置配置实现
const Vec2 GameConfig::PositionSettings::BOTTOM_CARD_OFFSET = Vec2(100, 0);
const Vec2 GameConfig::PositionSettings::MAIN_CARDS_START_OFFSET = Vec2(0, 60);
const Vec2 GameConfig::PositionSettings::SCORE_LABEL_OFFSET = Vec2(100, -50);
const Vec2 GameConfig::PositionSettings::INSTRUCTION_OFFSET = Vec2(0, 50);

// 资源路径配置实现
const std::string GameConfig::ResourcePaths::FONT_PATH = "fonts/arial.ttf";
const std::string GameConfig::ResourcePaths::CLOSE_NORMAL_IMAGE = "CloseNormal.png";
const std::string GameConfig::ResourcePaths::CLOSE_SELECTED_IMAGE = "CloseSelected.png";
