# AI Integration Guide

## 概述

UhandEngine 设计为 AI-first 游戏引擎，目标是让 AI 能够高效理解和操作引擎代码。本文档描述了 AI 集成的架构和指南。

## 核心设计原则

### 1. 模块化边界清晰

每个模块有明确的职责和接口：
- **Scene**: 场景管理，GameObject 生命周期
- **GameObject**: 基础实体，组件容器
- **Component**: 可复用功能单元
- **Transform**: 2D 变换（位置、旋转、缩放、原点）
- **AssetManager**: 资源加载和管理
- **InputManager**: 输入处理
- **Camera**: 视口控制
- **Animation**: 动画播放
- **RenderQueue**: 渲染队列（按 depth 排序）

### 2. 命名约定

- **类型名**: PascalCase (GameObject, Component, Scene)
- **函数名**: PascalCase (CreateGameObject, UpdateTransform)
- **变量名**: camelCase (gameObject, transform)
- **宏定义**: UPPER_CASE (MAX_OBJECTS, DEFAULT_SIZE)

### 3. 文件组织

```
src/engine/
  - 核心系统: Scene.c, GameObject.c, Component.c
  - 子系统: Transform.c, AssetManager.c, InputManager.c
  - 渲染: RenderQueue.c
  - UI: UI/TextComponent.c

examples/
  - 每个功能一个示例: sprite_example.c, input_example.c

doc/
  - 架构文档: architecture.md
  - AI 指南: ai-integration-guide.md
```

## AI 友好的 API 设计

### 1. 创建和销毁

```c
// 创建
GameObject *go = createGameObject("Player");
Scene *scene = Scene_Create();
Camera *camera = Camera_Create(800, 600);

// 销毁
freeGameObject(go);
Scene_Destroy(scene);
Camera_Destroy(camera);
```

### 2. 属性访问

```c
// 设置属性
setDepth(go, 10);
go->visible = 1;
go->active = 1;

// 获取属性
int depth = go->depth;
float x = go->transform->x;
```

### 3. 组件操作

```c
// 添加组件
Component *comp = createTransform(0, 0);
addComponent(go, comp);

// 获取组件
Component *transform = getComponent(go, COMPONENT_TYPE_TRANSFORM);
```

## PhaserJS 对齐

UhandEngine 的 API 设计参考 PhaserJS，便于 AI 理解：

| PhaserJS | UhandEngine |
|----------|-------------|
| scene.add.sprite() | createGameObject() + Scene_AddGameObject() |
| sprite.x = 100 | transform->setPosition(transform, 100, y) |
| sprite.visible = true | go->visible = 1 |
| sprite.setDepth(10) | setDepth(go, 10) |

## AI 辅助开发工作流

### 1. 代码生成

AI 可以基于以下模式生成代码：
- 创建 GameObject 和添加组件
- 设置 Transform 属性
- 实现 Update 循环
- 添加渲染逻辑

### 2. 代码重构

AI 可以：
- 重命名符号以保持一致性
- 重组文件以符合模块边界
- 提取重复代码为函数
- 添加文档注释

### 3. 功能扩展

AI 可以：
- 添加新的 Component 类型
- 实现新的子系统
- 创建新的示例
- 扩展现有功能

## 文档要求

### 1. 头文件注释

每个头文件应包含：
- 模块用途描述
- 主要数据结构
- 公开 API 说明

### 2. 函数注释

复杂函数应包含：
- 功能描述
- 参数说明
- 返回值说明
- 使用示例

### 3. 架构文档

保持以下文档更新：
- architecture.md: 整体架构
- ai-integration-guide.md: AI 集成指南
- swe1.6-prompt.md: AI 协作提示

## 未来改进方向

1. **ECS 架构**: 考虑迁移到更纯粹的 Entity-Component-System
2. **脚本绑定**: 添加 Lua 或 Python 绑定
3. **可视化编辑器**: 构建场景编辑器
4. **AI 代码生成**: 集成 AI 代码生成工具
5. **自动化测试**: 添加单元测试和集成测试

## 总结

UhandEngine 的 AI-first 设计通过清晰的模块边界、一致的命名约定和完善的文档，使 AI 能够高效理解和扩展引擎代码。持续优化这些方面将提升 AI 协作效率。
