# 第1周：架构收敛分析

## 当前 main.c 流程分析

### 启动流程
```
main()
  ├─ init()
  │   ├─ srand(time(NULL))
  │   ├─ Mix_OpenAudio()
  │   ├─ TTF_Init()
  │   ├─ SDL_Init()
  │   ├─ SDL_CreateWindow()
  │   └─ SDL_CreateRenderer()
  ├─ loadMedia()
  │   └─ TTF_OpenFont()
  ├─ gameInit()
  │   ├─ Scene_Create()
  │   ├─ Scene_Awake()
  │   ├─ Scene_Start()
  │   └─ 创建 UI 组件并添加到 Scene
  └─ loop()
      └─ tick() (每帧调用)
```

### 主循环 (tick)
```
tick()
  ├─ 计算 deltaTime
  ├─ Scene_Update()          // 更新 GameObject
  ├─ SDL_PollEvent()         // 处理输入事件
  ├─ handleButtons()         // 按钮事件处理
  ├─ Scene_UpdateUI()        // 更新 UIComponent
  ├─ SDL_SetRenderDrawColor()
  ├─ SDL_RenderClear()
  ├─ Scene_RenderUI()        // 渲染 UIComponent
  ├─ SDL_RenderPresent()
  └─ 更新 lastTime
```

### 退出流程
```
quit()
  ├─ Scene_Destroy()
  ├─ SDL_DestroyRenderer()
  ├─ SDL_DestroyWindow()
  ├─ SDL_Quit()
  ├─ TTF_CloseFont()
  └─ TTF_Quit()
```

## 当前模块职责边界

### Scene 职责
- **当前职责**：
  - 管理 GameObjectList
  - 管理 UIComponents 链表
  - 管理 RenderQueue
  - 提供生命周期方法（Awake、Start、Update）
  - 提供渲染方法（RenderUI、RenderGameObjects）

- **问题**：
  - Scene 不是真正的运行入口，main.c 仍然是控制中心
  - Scene 的职责过于分散，同时管理 GameObject 和 UIComponent
  - 缺少场景切换和场景管理功能

### GameObject 职责
- **当前职责**：
  - 基础属性（type、name、id）
  - 2D 基础属性（visible、active、depth）
  - 父子关系（parent、children）
  - 组件系统（components、transform）
  - 生命周期函数（Awake、Start、Update、LateUpdate、FixedUpdate、Destroy）

- **问题**：
  - GameObject 类型枚举与 Component 类型枚举重复
  - GameObject 的 render 函数指针与 Component 的 draw 函数指针职责重叠
  - GameObject 的链表节点（next）暴露了内部实现

### Component 职责
- **当前职责**：
  - 组件类型（TRANSFORM、SPRITE、TEXT、CUSTOM）
  - 函数指针（update、draw、destroy）

- **问题**：
  - Component 缺少通用数据字段
  - Component 的类型系统不够灵活
  - Component 与 GameObject 的关系不够清晰

## 当前架构问题

### 1. Scene 不是真正的运行入口
- **现状**：main.c 控制整个生命周期，Scene 只是被动被调用
- **影响**：场景切换、场景管理无法实现
- **建议**：让 Scene 成为运行入口，main.c 只负责平台初始化

### 2. 职责边界不清晰
- **现状**：Scene 同时管理 GameObject 和 UIComponent
- **影响**：代码耦合度高，难以扩展
- **建议**：明确 Scene 的职责范围，GameObject 和 UIComponent 应该有统一的管理方式

### 3. 平台层与运行时层混合
- **现状**：main.c 同时包含平台初始化（SDL）和游戏逻辑
- **影响**：难以支持多平台和测试
- **建议**：分离平台层和运行时层

### 4. 全局变量过多
- **现状**：main.c 中有大量全局变量（window、renderer、font、gQuit 等）
- **影响**：代码难以维护和测试
- **建议**：将全局变量封装到 Engine 或 Scene 中

### 5. 输入处理分散
- **现状**：输入处理在 handleButtons() 中，与 Scene 脱离
- **影响**：难以实现统一的输入管理
- **建议**：将输入处理集成到 Scene 或独立的 InputManager

## Scene-first 架构草案

### 目标
让 Scene 成为真正的运行入口，main.c 只负责平台初始化。

### 架构设计
```
main.c (平台层)
  ├─ 平台初始化（SDL、Emscripten）
  ├─ Engine 初始化
  ├─ Scene 初始化
  ├─ 主循环（委托给 Engine）
  └─ 平台清理

Engine (运行时层)
  ├─ 场景管理（Scene 切换、Scene 栈）
  ├─ 输入管理（InputManager）
  ├─ 资源管理（AssetManager）
  ├─ 渲染管理（RenderManager）
  └─ 时间管理（TimeManager）

Scene (场景层)
  ├─ GameObject 管理（统一管理所有对象）
  ├─ 生命周期管理（Awake、Start、Update、Destroy）
  ├─ 渲染管理（RenderQueue）
  └─ 场景特定逻辑

GameObject (对象层)
  ├─ Transform（位置、旋转、缩放）
  ├─ Component 组件系统
  ├─ 父子关系
  └─ 生命周期
```

### 重构清单
1. **创建 Engine 结构**
   - 封装全局变量（window、renderer、font）
   - 提供场景管理功能
   - 提供主循环控制

2. **重构 Scene**
   - Scene 成为运行入口
   - 统一管理 GameObject（不再区分 UIComponent）
   - 添加场景切换功能

3. **重构 GameObject**
   - 移除链表节点（next）
   - 统一类型系统
   - 明确 Component 与 GameObject 的关系

4. **分离平台层**
   - 创建 platform.h 封装平台相关代码
   - main.c 只调用 platform 初始化和清理

5. **统一输入管理**
   - 将 InputManager 集成到 Scene
   - 提供统一的输入事件分发

## 命名规范建议

### 文件命名
- C 文件：小写，用下划线分隔（如 `game_object.c`）
- 头文件：小写，用下划线分隔（如 `game_object.h`）

### 函数命名
- 模块前缀 + 动作 + 对象（如 `Scene_AddGameObject`）
- 驼峰命名用于回调（如 `onStartButtonClick`）

### 结构体命名
- 大驼峰（如 `GameObject`）
- 类型枚举：全大写 + 下划线（如 `GAMEOBJECT_TYPE_SPRITE`）

### 变量命名
- 局部变量：小驼峰（如 `gameObject`）
- 全局变量：g 前缀 + 小驼峰（如 `gWindow`）
- 成员变量：小驼峰（如 `transform`）

## 下一步行动

1. 创建 Engine 结构体
2. 重构 main.c，分离平台层和运行时层
3. 重构 Scene，使其成为运行入口
4. 统一 GameObject 和 UIComponent
5. 更新架构文档
