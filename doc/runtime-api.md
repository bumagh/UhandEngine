# UhandEngine Runtime API 设计说明

## 1. 文档目标

本文档用于定义 `UhandEngine` 运行时 API 的分层结构、核心模块边界、推荐命名方式以及未来演进方向。

目标不是立刻冻结所有接口，而是先建立一个稳定的 API 心智模型，让后续：

- 运行时代码的重构更有边界
- AI 代理生成代码时有清晰目标
- 新模块可以按统一规则接入
- 项目能逐步向 PhaserJS 的 2D API 体验靠拢

## 2. API 设计原则

- **Scene First**
  - 所有运行时入口优先围绕 `Scene` 展开

- **Small Stable Surface**
  - 先定义稳定的小接口，不急于暴露过多能力

- **Phaser-like Mental Model**
  - API 命名和职责尽量贴近 `Scene`、`GameObject`、`Text`、`Sprite`、`Input`、`Camera` 等概念

- **C-friendly**
  - 遵循 C 语言风格，使用明确函数名前缀、显式所有权和清晰参数

- **AI-friendly**
  - 命名稳定、参数顺序可预测、职责单一、文档先行

## 3. 运行时分层

建议未来运行时 API 分为以下几层：

```text
App Layer
├─ 引擎启动
├─ 主循环
└─ 平台生命周期

Scene Layer
├─ Scene 创建/切换
├─ Scene 生命周期
└─ Scene 对象管理

Object Layer
├─ GameObject
├─ Container
├─ Sprite
└─ Text

Component Layer
├─ Transform
├─ Render
├─ Input
├─ Animation
└─ Custom Components

System Layer
├─ Asset
├─ Input
├─ Event
├─ Camera
├─ Animation
└─ Audio

Platform Layer
├─ SDL Native
└─ Web / Emscripten
```

## 4. 推荐 API 命名策略

建议未来统一使用前缀风格命名，避免全局符号混乱。

推荐前缀：`UH_`

例如：

- `UH_App_Init`
- `UH_App_Run`
- `UH_Scene_Create`
- `UH_Scene_AddGameObject`
- `UH_GameObject_AddComponent`
- `UH_Input_IsKeyPressed`
- `UH_Asset_LoadFont`

## 5. 命名规则建议

- 模块名使用 `PascalCase` 语义词
- C 函数命名统一使用：`UH_<Module>_<Action>`
- 布尔查询函数优先使用：`Is` / `Has` / `Can`
- 构造函数优先使用：`Create`
- 释放函数优先使用：`Destroy` 或 `Free`

例如：

- `UH_Scene_Create`
- `UH_Scene_Destroy`
- `UH_GameObject_SetActive`
- `UH_Component_IsEnabled`

## 6. App Layer

`App Layer` 用于组织引擎级启动与主循环控制。

建议职责：

- 初始化 SDL 与子系统
- 创建窗口与渲染器
- 启动主循环
- 管理当前活动场景
- 处理应用退出

建议接口：

```text
UH_App_Init()
UH_App_LoadPlatform()
UH_App_SetScene()
UH_App_Run()
UH_App_Tick()
UH_App_Shutdown()
```

与当前代码关系：

- 当前 `main.c` 中的 `init()`、`loop()`、`quit()` 可逐步收敛到该层

## 7. Scene Layer

`Scene Layer` 是未来最核心的 API 层。

建议职责：

- 创建与销毁场景
- 管理场景生命周期
- 管理对象列表
- 提供场景级资源与系统访问

建议接口：

```text
UH_Scene_Create(const char* key)
UH_Scene_Destroy(UH_Scene* scene)
UH_Scene_Preload(UH_Scene* scene)
UH_Scene_CreateObjects(UH_Scene* scene)
UH_Scene_Update(UH_Scene* scene, float deltaTime)
UH_Scene_AddGameObject(UH_Scene* scene, UH_GameObject* go)
UH_Scene_RemoveGameObject(UH_Scene* scene, UH_GameObject* go)
UH_Scene_FindByName(UH_Scene* scene, const char* name)
```

Phaser-like 对齐：

- 对齐 `Scene.preload`
- 对齐 `Scene.create`
- 对齐 `Scene.update`

## 8. GameObject Layer

`GameObject Layer` 用于表达场景中的基础实体。

建议职责：

- 提供对象基础属性
- 管理组件列表
- 管理启用/可见状态
- 管理父子层级关系

建议接口：

```text
UH_GameObject_Create(const char* name)
UH_GameObject_Destroy(UH_GameObject* go)
UH_GameObject_AddComponent(UH_GameObject* go, UH_Component* component)
UH_GameObject_RemoveComponent(UH_GameObject* go, UH_Component* component)
UH_GameObject_GetComponent(UH_GameObject* go, const char* type)
UH_GameObject_SetActive(UH_GameObject* go, int active)
UH_GameObject_SetVisible(UH_GameObject* go, int visible)
```

推荐基础字段：

- `name`
- `id`
- `active`
- `visible`
- `transform`
- `parent`
- `children`
- `components`

## 9. Object Type APIs

为了向 PhaserJS 对齐，建议在 `GameObject` 之上逐步形成更明确的对象类型 API。

## 9.1 Text

建议接口：

```text
UH_Text_Create(const char* name, const char* text)
UH_Text_SetContent(UH_Text* textObject, const char* content)
UH_Text_SetFont(UH_Text* textObject, const char* fontKey, int size)
UH_Text_SetColor(UH_Text* textObject, UH_Color color)
```

## 9.2 Sprite

建议接口：

```text
UH_Sprite_Create(const char* name, const char* textureKey)
UH_Sprite_SetTexture(UH_Sprite* sprite, const char* textureKey)
UH_Sprite_SetFrame(UH_Sprite* sprite, int frame)
```

## 9.3 Container

建议接口：

```text
UH_Container_Create(const char* name)
UH_Container_AddChild(UH_Container* container, UH_GameObject* child)
UH_Container_RemoveChild(UH_Container* container, UH_GameObject* child)
```

## 10. Component Layer

组件层应遵循 `component-contract.md`。

建议职责：

- 定义通用组件协议
- 提供生命周期钩子
- 作为对象扩展能力单位

建议通用接口：

```text
UH_Component_SetOwner(UH_Component* component, UH_GameObject* owner)
UH_Component_SetEnabled(UH_Component* component, int enabled)
UH_Component_IsEnabled(UH_Component* component)
UH_Component_Destroy(UH_Component* component)
```

建议优先标准化的组件 API：

- `UH_Transform_*`
- `UH_TextComponent_*`
- `UH_Clickable_*`
- `UH_SpriteRenderer_*`
- `UH_AnimationPlayer_*`

## 11. Transform API

建议把 `Transform` 逐步升级为 2D-first：

```text
UH_Transform_SetPosition(UH_Transform* transform, float x, float y)
UH_Transform_SetScale(UH_Transform* transform, float scaleX, float scaleY)
UH_Transform_SetRotation(UH_Transform* transform, float rotation)
UH_Transform_SetOrigin(UH_Transform* transform, float originX, float originY)
```

说明：

- 当前代码保留 `z` 也可以，但对外主文档应优先推广 2D API

## 12. Asset System API

建议由当前 `loadMedia()` 演进而来。

建议职责：

- 统一管理字体、图片、音频等资源
- 通过 key 获取资源
- 支持 Web 端预加载

建议接口：

```text
UH_Asset_Init()
UH_Asset_LoadFont(const char* key, const char* path, int size)
UH_Asset_LoadTexture(const char* key, const char* path)
UH_Asset_LoadAudio(const char* key, const char* path)
UH_Asset_GetFont(const char* key)
UH_Asset_GetTexture(const char* key)
UH_Asset_GetAudio(const char* key)
UH_Asset_Shutdown()
```

## 13. Input System API

建议把 SDL 原始事件包装为统一输入接口。

建议职责：

- 键盘状态访问
- 指针位置访问
- 点击命中派发
- 后续多端输入统一

建议接口：

```text
UH_Input_BeginFrame()
UH_Input_ProcessSDLEvent(void* event)
UH_Input_IsKeyPressed(int key)
UH_Input_IsPointerDown()
UH_Input_GetPointerPosition(float* x, float* y)
UH_Input_EndFrame()
```

## 14. Event System API

建议保留现有 `event.c` 的方向，但逐步 API 化。

建议接口：

```text
UH_Event_CreateBus()
UH_Event_On(UH_EventBus* bus, const char* eventType, UH_Callback callback, void* target)
UH_Event_Off(UH_EventBus* bus, const char* eventType, UH_Callback callback, void* target)
UH_Event_Emit(UH_EventBus* bus, const char* eventType, void** args, int argCount)
UH_Event_DestroyBus(UH_EventBus* bus)
```

## 15. Camera API

建议作为 Phaser-like 2D 体验的重要组成部分。

建议接口：

```text
UH_Camera_Create()
UH_Camera_SetPosition(UH_Camera* camera, float x, float y)
UH_Camera_SetZoom(UH_Camera* camera, float zoom)
UH_Camera_Follow(UH_Camera* camera, UH_GameObject* target)
UH_Camera_SetBounds(UH_Camera* camera, float x, float y, float w, float h)
```

## 16. Animation / Tween API

建议后续引入最小动画接口：

```text
UH_Animation_Create(const char* key)
UH_Animation_AddFrame(UH_Animation* animation, int frame)
UH_Animation_Play(UH_GameObject* target, const char* key)

UH_Tween_Create(UH_GameObject* target)
UH_Tween_To(UH_Tween* tween, const char* property, float value, float duration)
UH_Tween_Start(UH_Tween* tween)
```

## 17. 错误处理建议

建议 API 在 C 语言下保持显式错误处理：

- 初始化类接口返回 `int` 或状态码
- 创建类接口返回指针，失败返回 `NULL`
- 关键错误写入日志系统

例如：

- `int UH_App_Init();`
- `UH_Scene* UH_Scene_Create(const char* key);`

## 18. 与当前代码的渐进接入顺序

建议按以下顺序逐步把当前代码收敛到 Runtime API：

1. 把 `main.c` 中流程抽象为 `App Layer`
2. 让 `Scene` 成为真正主运行单元
3. 规范 `GameObject` / `Component` API
4. 补 `Asset` / `Input` 系统 API
5. 增加 `Text` / `Container` / `Sprite` 类型 API
6. 再扩展 `Camera` / `Animation` / `Tween`

## 19. AI 使用建议

AI 在为 UhandEngine 设计或实现运行时接口时，应优先检查：

- 是否符合 `Scene First`
- 是否尽量靠近 Phaser-like 心智
- 是否能保持 Native + Web 双端兼容
- 是否命名稳定且可预测
- 是否应同步更新 `scene-schema.md` 或 `component-contract.md`

## 20. 结论

`Runtime API` 文档的价值在于，为 UhandEngine 提供一套“从当前原型到未来引擎”的中间接口蓝图。

它不会要求项目一次性重写，但会让未来所有模块演进都更统一、更可预测，也更适合 AI 协作与 PhaserJS 对齐。
