# 🎯 Z-order 基础概念

什么是Z-order？
Z-order（Z轴顺序）是2D图形系统中控制渲染层级的概念：

Z轴: 垂直于屏幕的虚拟轴，从屏幕内部指向用户
数值越大: 越靠近用户，显示在上层
数值越小: 越远离用户，显示在下层
用户眼睛
    ↑
    |  Z-order = 10 (最上层)
    |  Z-order = 5  (中间层)  
    |  Z-order = 1  (底层)
    |
  屏幕平面
🏗️ 在Cocos2d-x中的Z-order
Node层级系统:

```c++
// 添加子节点时指定Z-order
parentNode->addChild(childNode, zOrder);

// 动态修改Z-order 
childNode->setLocalZOrder(newZOrder);

// 重新排序子节点
parentNode->reorderChild(childNode, newZOrder);
```

渲染规则:
同一父节点下: Z-order决定渲染顺序
不同父节点: 父节点的Z-order决定整体层级
相同Z-order: 按添加顺序渲染（后添加的在上层）
🎮 在您当前项目中的Z-order应用

1. 卡牌视图的层级设计
在当前打开的 GameController.cpp文件中，第227行：
spareCardView->setLocalZOrder(20); // 设置高Z-order

2. 项目中的Z-order分层
    CardViewManager中的Z-order设置：

    ```cpp
    // CardViewManager.cpp - createBottomCardStackView()
    int zOrder = 5 + static_cast<int>(i);  // 底牌栈：5, 6, 7, 8...
    _parentNode->addChild(cardView, zOrder);

    // CardViewManager.cpp - createSpareCardStackView()  
    int zOrder = 2 + static_cast<int>(i);  // 备用栈：2, 3, 4...
    _parentNode->addChild(cardView, zOrder);

    // GameController.cpp - 动画时的临时Z-order
    spareCardView->setLocalZOrder(20);     // 动画中：20 (最高)
    ```

3. 项目的Z-order层级图

Z-order 层级分布：
┌─────────────────────────────────────┐
│ 20: 动画中的卡牌 (临时最高层)          │
├─────────────────────────────────────┤
│ 10-15: 新移动的底牌 (确保在栈顶)      │
├─────────────────────────────────────┤
│ 5-9: 底牌栈 (5=底层, 9=栈顶)         │
├─────────────────────────────────────┤
│ 2-4: 备用栈 (2=底层, 4=栈顶)         │
├─────────────────────────────────────┤
│ 1: 主牌区 (最小版本只有1张)           │
└─────────────────────────────────────┘

🔧 Z-order的实际应用场景
场景1: 卡牌堆叠效果

```cpp
// 创建堆叠效果
for (size_t i = 0; i < cardStack.size(); i++) {
    int zOrder = baseZOrder + static_cast<int>(i);
    // i=0: 底层卡牌, i=n: 栈顶卡牌
}
```

场景2: 动画过程中的层级控制

```cpp
// 在当前文件第227行
spareCardView->setLocalZOrder(20);
// 确保移动的卡牌在所有其他卡牌之上，避免被遮挡
```

场景3: UI元素的层级

```cpp
// 典型的游戏UI层级
backgroundLayer->setLocalZOrder(-10);  // 背景
gameLayer->setLocalZOrder(0);          // 游戏内容  
uiLayer->setLocalZOrder(10);           // UI界面
popupLayer->setLocalZOrder(20);        // 弹窗
```

⚠️ Z-order的常见问题
问题1: Z-order冲突  不同节点使用相同Z-order
问题2: 动画过程中的层级错误
问题3: 父子节点的层级关系

```cpp
// 注意：子节点的Z-order只在同一父节点内有效
parentA->addChild(childA, 10);
parentB->addChild(childB, 1);
// 如果parentB的Z-order > parentA，则childB仍然在childA之上
```

💡 Z-order最佳实践

1. 分层设计:

    ```cpp
    // 预留Z-order范围
    const int BACKGROUND_LAYER = -100;
    const int GAME_LAYER = 0;
    const int CARD_LAYER_BASE = 100;
    const int UI_LAYER = 1000;
    const int POPUP_LAYER = 2000;
    ```

2. 动态调整:

    ```cpp
    // 动画时临时提升
    void startCardAnimation(CardView* card) {
       card->setLocalZOrder(ANIMATION_TEMP_LAYER);
    }

    // 动画完成后恢复
    void endCardAnimation(CardView* card) {
        card->setLocalZOrder(calculateNormalZOrder(card));
    }
    ```

3. 调试技巧:

    ```cpp
    // 调试Z-order
    void debugZOrder(Node* parent) {
        for (auto child : parent->getChildren()) {
            CCLOG("Child: %p, Z-order: %d", child, child->getLocalZOrder());
        }
    }
    ```

该项目中，Z-order主要用于：

卡牌堆叠效果 - 营造真实的卡牌层次感
动画层级控制 - 确保移动的卡牌不被遮挡
交互反馈 - 突出显示当前操作的卡牌
