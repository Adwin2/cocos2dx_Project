#ifndef __CARDGAME_SCENE_MVC_H__
#define __CARDGAME_SCENE_MVC_H__

#include "cocos2d.h"
#include "controllers/GameController.h"

USING_NS_CC;

/**
 * 纸牌游戏主场景 (MVC架构版本)
 * 职责：作为游戏的主视图，协调UI显示和用户交互
 */
class CardGameSceneMVC : public Scene
{
private:
    GameController* _gameController;          // 游戏控制器
    
    // UI元素
    Label* _scoreLabel;                       // 分数标签
    Label* _levelLabel;                       // 关卡标签
    
    // 菜单
    Menu* _gameMenu;                          // 游戏菜单
    MenuItemLabel* _restartItem;              // 重新开始按钮
    // MVP版本：删除Hint功能
    // MenuItemLabel* _hintItem;              // 已删除
    MenuItemImage* _closeItem;                // 关闭按钮
    
    Size _visibleSize;                        // 可见区域大小
    Vec2 _origin;                             // 原点位置

public:
    static Scene* createScene();
    virtual bool init();
    
    // 创建UI元素
    void createUI();
    void createBackground();
    void createLabels();
    void createMenu();
    
    // 菜单回调
    void onRestartClicked(Ref* sender);
    void onCloseClicked(Ref* sender);

private:
    // 初始化游戏控制器
    void initGameController();
    
    // 设置控制器回调
    void setupControllerCallbacks();

public:
    CREATE_FUNC(CardGameSceneMVC);
};

#endif // __CARDGAME_SCENE_MVC_H__
