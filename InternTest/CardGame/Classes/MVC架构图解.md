# MVC架构图解与数据流向

## 🏗️ 整体架构图

```
┌─────────────────────────────────────────────────────────────┐
│                        应用程序层                              │
├─────────────────────────────────────────────────────────────┤
│  AppDelegate.cpp                                           │
│  ├── 窗口初始化                                              │
│  ├── 资源预加载 (ResourceService::preloadCardResources)      │
│  └── 场景启动 (CardGameSceneMVC::createScene)               │
└─────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────┐
│                        视图层 (Views)                        │
├─────────────────────────────────────────────────────────────┤
│  CardGameSceneMVC.cpp (主场景)                              │
│  ├── UI创建 (createUI)                                      │
│  ├── 用户交互处理                                            │
│  └── 显示更新回调                                            │
│                                │                            │
│  CardView.cpp (卡牌视图)                                    │
│  ├── 卡牌显示 (updateSprites)                               │
│  ├── 动画效果 (playMatchAnimation)                          │
│  └── 触摸事件 (onTouchBegan)                                │
└─────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────┐
│                      控制器层 (Controllers)                   │
├─────────────────────────────────────────────────────────────┤
│  GameController.cpp                                        │
│  ├── 游戏流程控制 (startNewGame, restartGame)                │
│  ├── 用户操作处理 (onCardClicked)                            │
│  ├── 模型视图协调                                            │
│  └── 事件回调管理                                            │
└─────────────────────────────────────────────────────────────┘
                                │
                    ┌───────────┼───────────┐
                    ▼           ▼           ▼
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│   管理器层    │ │   服务层     │ │   模型层     │
│ (Managers)  │ │ (Services)  │ │  (Models)   │
└─────────────┘ └─────────────┘ └─────────────┘
```

## 📊 详细分层架构

### 1. 配置层 (configs/)
```
GameConfig.h/cpp
├── GameSettings (游戏基础配置)
├── CardSettings (卡牌尺寸配置)
├── UISettings (界面配置)
├── ColorSettings (颜色主题)
├── PositionSettings (位置配置)
└── ResourcePaths (资源路径)

作用: 集中管理所有静态配置，便于调整和维护
依赖: 无 (最底层)
被依赖: 所有其他层
```

### 2. 模型层 (models/)
```
CardModel.h/cpp
├── 卡牌数据 (花色、数值、状态)
├── 数据操作 (翻牌、比较)
└── 字符串转换

GameModel.h/cpp
├── 游戏状态 (分数、关卡、游戏状态)
├── 卡牌集合 (底牌、主牌区)
└── 数据管理 (添加、删除、查询)

作用: 存储运行时数据，不包含业务逻辑
依赖: configs/
被依赖: services/, managers/, controllers/
```

### 3. 服务层 (services/)
```
GameLogicService.h/cpp
├── 匹配规则 (canMatch)
├── 得分计算 (calculateMatchScore)
├── 游戏结束判断 (checkGameOver)
└── 可用匹配查询 (getAvailableMatches)

CardGeneratorService.h/cpp
├── 随机卡牌生成 (generateRandomCard)
├── 完整牌组生成 (generateFullDeck)
├── 洗牌算法 (shuffleCards)
└── 游戏初始化 (generateInitialCards)

ResourceService.h/cpp
├── 资源路径管理 (getCardValueImagePath)
├── 资源组合 (getCardResources)
├── 资源预加载 (preloadCardResources)
└── 资源检查 (isResourceExists)

作用: 提供无状态的业务逻辑服务
依赖: models/, configs/
被依赖: controllers/, managers/
```

### 4. 视图层 (views/)
```
CardView.h/cpp
├── 卡牌显示 (updateSprites)
├── 动画效果 (playMatchAnimation, playMismatchAnimation)
├── 用户交互 (initTouchEvents, onTouchBegan)
├── 状态切换 (setFaceUp, setHighlighted)
└── 资源更新 (updateCardResources)

作用: 负责界面显示和用户交互，不包含业务逻辑
依赖: models/, services/, configs/
被依赖: managers/
```

### 5. 管理器层 (managers/)
```
CardViewManager.h/cpp
├── 视图生命周期 (createCardView, removeCardView)
├── 布局管理 (layoutCards, calculatePosition)
├── 动画协调 (playMatchAnimation, highlightMatchableCards)
├── 批量操作 (updateFromGameModel, clearAllCardViews)
└── 事件转发 (onCardClicked)

作用: 管理视图组件，作为控制器的成员变量
依赖: views/, models/, configs/
被依赖: controllers/
```

### 6. 控制器层 (controllers/)
```
GameController.h/cpp
├── 游戏流程 (startNewGame, restartGame, pauseGame)
├── 用户操作 (onCardClicked, showHint, clearHints)
├── 模型更新 (handleCardMatch, generateNewBottomCard)
├── 视图同步 (updateViews, handleMatchSuccess)
├── 事件回调 (notifyScoreUpdate, notifyGameWin)
└── 状态管理 (checkGameEnd, setLevel)

作用: 协调各层，处理游戏主要流程
依赖: 所有其他层
被依赖: views/ (主场景)
```

### 7. 主场景 (CardGameSceneMVC)
```
CardGameSceneMVC.h/cpp
├── UI创建 (createUI, createLabels, createMenu)
├── 控制器初始化 (initGameController, setupControllerCallbacks)
├── 显示更新 (updateScoreDisplay, updateGameStatusDisplay)
├── 用户交互 (onRestartClicked, onHintClicked)
└── 消息显示 (showMessage, hideMessage)

作用: 游戏主界面，连接用户和控制器
依赖: controllers/, configs/
被依赖: AppDelegate
```

## 🔄 数据流向图

### 用户点击卡牌的完整流程:

```
1. 用户点击
   CardView::onTouchBegan()
   │
   ▼
2. 事件转发
   CardViewManager::onCardClicked()
   │
   ▼
3. 控制器处理
   GameController::onCardClicked()
   │
   ▼
4. 业务逻辑
   GameLogicService::processCardMatch()
   │
   ├── GameLogicService::canMatch() (检查匹配)
   ├── GameModel::removeMainCardById() (更新数据)
   ├── GameModel::addScore() (更新分数)
   └── GameLogicService::checkGameOver() (检查结束)
   │
   ▼
5. 视图更新
   GameController::handleMatchSuccess()
   │
   ├── CardViewManager::playMatchAnimation() (播放动画)
   ├── CardGeneratorService::generateNewBottomCard() (生成新底牌)
   └── CardGameSceneMVC::onScoreUpdate() (更新UI)
```

### 游戏初始化流程:

```
1. 应用启动
   AppDelegate::applicationDidFinishLaunching()
   │
   ├── ResourceService::preloadCardResources() (预加载资源)
   └── CardGameSceneMVC::createScene() (创建场景)
   │
   ▼
2. 场景初始化
   CardGameSceneMVC::init()
   │
   ├── createUI() (创建界面)
   └── initGameController() (初始化控制器)
   │
   ▼
3. 控制器初始化
   GameController::init()
   │
   └── CardViewManager::new() (创建视图管理器)
   │
   ▼
4. 游戏开始
   GameController::startNewGame()
   │
   ├── GameModel::reset() (重置数据)
   ├── CardGeneratorService::generateInitialCards() (生成卡牌)
   └── CardViewManager::updateFromGameModel() (更新视图)
```

## 🔗 依赖关系图

```
AppDelegate
    │
    ▼
CardGameSceneMVC ──────┐
    │                 │
    ▼                 ▼
GameController ────► configs/
    │
    ├──► managers/ ──► views/ ──┐
    │                          │
    ├──► services/ ──► models/ ─┤
    │                          │
    └──► configs/ ◄─────────────┘
```

## 📋 修改影响范围

| 修改位置 | 影响范围 | 重新编译 | 注意事项 |
|---------|---------|---------|---------|
| configs/ | 全部模块 | ✅ | 影响最广，需要全面测试 |
| models/ | services+, managers+, controllers+ | ✅ | 数据结构变更需要同步更新 |
| services/ | controllers+, managers+ | ✅ | 业务逻辑变更需要测试 |
| views/ | managers+ | ✅ | 界面变更相对安全 |
| managers/ | controllers+ | ✅ | 管理逻辑变更需要测试 |
| controllers/ | 主场景 | ✅ | 流程变更需要全面测试 |

## 🎯 最佳实践

### 1. 单一职责原则
- 每个类只负责一个明确的功能
- configs/ 只存配置，不含逻辑
- services/ 只含逻辑，不存状态

### 2. 依赖倒置原则
- 高层模块不依赖低层模块
- 通过接口和回调解耦

### 3. 开闭原则
- 对扩展开放，对修改封闭
- 新功能通过添加新类实现

### 4. 接口隔离原则
- 使用回调函数而非大接口
- 每个回调只负责一个事件

---

📐 **架构设计**: MVC + 分层架构  
🔄 **数据流向**: 单向数据流  
🎯 **设计原则**: SOLID 原则
