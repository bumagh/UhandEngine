# SWE1.6 第 2 周提示词：建立 Phaser-like 2D 基础能力

## 1. 用途

本文档用于让 SWE1.6 基于 UhandEngine 第 1 周已完成的基础，执行第 2 周的架构推进任务。

## 2. 当前已完成状态（第 1 周）

第 1 周已完成：
- Scene-first 架构收敛
- Scene 成为运行时主单元
- GameObjectList 管理 GameObject 生命周期
- UIComponent 通过 Scene 统一管理
- MVP UI 组件（标题和三个按钮）正常显示和交互
- Native 和 Web 版本均可正常运行
- 点击事件防抖动机制已实现

当前架构状态：
- Scene 拥有 GameObjectList 和 UIComponent 链表
- GameObject 拥有 Component 链表
- Transform 组件已存在，但结构偏通用
- UIComponent 是 MVP 临时实现，非标准化组件

## 3. 第 2 周目标

建立 PhaserJS 风格的最小 2D 运行时抽象。

## 4. 重点任务

### 任务 1：Transform 升级为 2D first 结构

当前 Transform 结构偏通用，需要升级为 2D 优先：
- 明确 position (x, y)
- 明确 rotation (angle)
- 明确 scale (x, y)
- 明确 origin/pivot point
- 移除或标记 3D 相关字段为预留

### 任务 2：设计 Sprite、Text、Container 基础抽象

参考 PhaserJS，设计以下基础对象类型：
- **Sprite**: 基础图像显示对象
- **Text**: 文本显示对象（可复用当前 TextComponent）
- **Container**: 对象容器，支持父子关系

### 任务 3：补充可见性、层级、父子关系、激活状态

为 GameObject 添加基础属性：
- visible (bool): 可见性
- depth/z-index (int): 渲染层级
- parent/children: 父子关系
- active (bool): 激活状态（是否参与更新）

### 任务 4：建立基础 Display List 或渲染列表

设计渲染顺序机制：
- 按 depth/z-index 排序
- 支持动态调整层级
- 确保 Scene 能正确管理渲染顺序

### 任务 5：抽象输入系统

统一输入处理：
- 抽象 InputManager
- 统一鼠标、键盘事件
- 为后续触摸行为预留接口
- 支持输入事件分发到 GameObject

### 任务 6：梳理对象更新顺序与渲染顺序

明确生命周期顺序：
- Update 顺序（从父到子或从子到父）
- Render 顺序（按 depth 排序）
- 确保 Scene 统一调度

## 5. 可直接复制给 SWE1.6 的提示词

```text
你现在继续接手 UhandEngine 的第 2 周开发任务。

# 当前已完成状态（第 1 周）

当前仓库已经具备以下事实：
- Scene-first 架构已收敛
- Scene 成为运行时主单元
- MVP UI 组件正常显示和交互
- Native 和 Web 版本均可正常运行
- 点击事件防抖动机制已实现

# 你必须先阅读

- README.md
- doc/architecture.md
- doc/monthly-update-plan.md
- doc/phaser-ai-alignment-roadmap.md
- src/engine/Scene.h
- src/engine/Scene.c
- src/engine/transform.h
- src/engine/transform.c
- src/engine/gameObject.h
- src/engine/gameObject.c

# 你当前阶段的目标

你当前只允许聚焦以下目标：
1. 把 Transform 升级为 2D first 结构
2. 设计 Sprite、Text、Container 的基础抽象
3. 补充可见性、层级、父子关系、激活状态
4. 建立基础 Display List 或渲染列表
5. 抽象输入系统，统一鼠标、键盘和后续触摸行为
6. 梳理对象更新顺序与渲染顺序

# 你当前优先做什么

优先级从高到低：
1. Transform 升级为 2D first 结构
2. GameObject 补充基础属性（visible, depth, active, parent）
3. 设计 Sprite、Text、Container 抽象
4. 建立渲染列表和渲染顺序
5. 抽象 InputManager
6. 梳理更新和渲染顺序

# 你当前不要做什么

不要做：
- 完整 Physics 系统
- 完整 Animation / Tween 系统
- 大规模目录重构
- 与 2D 基础能力无关的功能扩展
- 破坏现有 MVP 功能

# 如果继续代码推进，只能优先考虑

1. 先设计抽象，再实现代码
2. 每个任务完成后验证 MVP 仍然正常
3. 保持与现有架构的一致性
4. 为每个新模块添加文档说明

# 你的输出格式

你的每次输出都尽量采用这个结构：
1. 任务理解
2. 当前判断
3. 改动方案
4. 影响文件
5. 风险点
6. 下一步建议

# 你的输出约束

1. 必须先总结当前已完成状态，再提修改
2. 必须明确区分"本轮立即做"和"下一轮再做"
3. 如果要改代码，优先做最小补丁
4. 如果改动影响架构，必须同步更新 doc/
5. 保持 PhaserJS 风格的命名和心智模型
6. 每完成一个任务，验证 MVP 仍然正常

# 本阶段成功标准

满足以下条件即可视为本阶段完成：
- Transform 已升级为 2D first 结构
- GameObject 已补充基础属性
- Sprite、Text、Container 抽象已设计
- 渲染列表和渲染顺序已建立
- InputManager 基础抽象已建立
- MVP 仍然正常运行
- 文档已更新

# PhaserJS 对齐参考

参考 PhaserJS 的以下概念：
- Phaser.GameObjects.Sprite
- Phaser.GameObjects.Text
- Phaser.GameObjects.Container
- Phaser.Scene.displayList
- Phaser.Input.InputPlugin
```

## 6. 更短版

```text
你现在开始执行 UhandEngine 的第 2 周任务：建立 Phaser-like 2D 基础能力。

先阅读：
- doc/monthly-update-plan.md (第 2 周部分)
- doc/phaser-ai-alignment-roadmap.md
- src/engine/transform.h
- src/engine/gameObject.h

目标：
1. Transform 升级为 2D first 结构
2. GameObject 补充基础属性（visible, depth, active, parent）
3. 设计 Sprite、Text、Container 抽象
4. 建立渲染列表和渲染顺序
5. 抽象 InputManager
6. 梳理更新和渲染顺序

不要做：
- 完整 Physics
- 完整 Animation / Tween
- 大规模重构
- 破坏现有 MVP

请先输出：
1. 当前架构摘要
2. 本周任务分解
3. 第一个任务的执行计划
4. 风险点

然后开始推进第一个任务。
```

## 7. 状态总结

- **[已完成]** 创建第 2 周提示词文档
- **[用途]** 用于指导第 2 周的 Phaser-like 2D 基础能力建设
- **[当前状态]** 无代码改动，仅提供执行 Prompt
