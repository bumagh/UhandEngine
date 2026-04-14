# 主循环重构清单

## 重构目标

将 UhandEngine 的主循环从"原型功能集合"重构为"Scene-first 运行时骨架"。

## 重构前状态

### main.c 直接管理的内容

- 全局 `GameObjectList *goList`
- 全局 `UIComponent` 变量（titleText、startButton、settingsButton、quitButton）
- 直接调用 `GameObjectList_CallUpdate()`
- 直接更新和渲染每个 UIComponent
- 直接释放所有资源

### 问题

- Scene 处于"备用模块"状态，未接入主循环
- GameObjectList 承担了世界容器职责，但 Scene 未成为拥有者
- main.c 职责过重，不利于后续扩展
- 缺少清晰的运行时分层

## 重构执行步骤

### 步骤 1：扩展 Scene 结构

**文件：** `src/engine/Scene.h`、`src/engine/Scene.c`

**改动：**
- 将 `GameObject *gameObjects` 改为 `GameObjectList *gameObjectList`
- 添加 `Component *uiComponents` 字段（临时 MVP 支持）
- 添加 `Scene_Create()` / `Scene_Destroy()` 接口
- 添加生命周期方法：`Scene_Awake()` / `Scene_Start()` / `Scene_Update()` / `Scene_DestroyAll()`
- 添加 UI 管理方法：`Scene_AddUIComponent()` / `Scene_UpdateUI()` / `Scene_RenderUI()`

**验证：** Scene 编译通过，接口清晰

### 步骤 2：修改 main.c 全局变量

**文件：** `src/main.c`

**改动：**
- 删除全局 `GameObjectList *goList`
- 删除全局 `UIComponent` 变量
- 添加全局 `Scene *mainScene`

**验证：** main.c 编译通过

### 步骤 3：修改 gameInit()

**文件：** `src/main.c`

**改动：**
- 调用 `Scene_Create()` 创建 Scene
- 调用 `Scene_Awake()` / `Scene_Start()`
- 将 UI 组件通过 `Scene_AddUIComponent()` 添加到 Scene

**验证：** Scene 正确创建，UI 组件正确添加

### 步骤 4：修改 tick()

**文件：** `src/main.c`

**改动：**
- 将 `GameObjectList_CallUpdate(goList)` 改为 `Scene_Update(mainScene)`
- 将逐个 UI 组件的更新改为 `Scene_UpdateUI(mainScene)`
- 将逐个 UI 组件的渲染改为 `Scene_RenderUI(mainScene, renderer)`

**验证：** 主循环通过 Scene 驱动

### 步骤 5：修改 quit()

**文件：** `src/main.c`

**改动：**
- 删除逐个释放 UI 组件的代码
- 删除释放 GameObjectList 的代码
- 调用 `Scene_Destroy(mainScene)` 释放所有资源

**验证：** 资源正确释放，无内存泄漏

## 重构后状态

### Scene 成为运行时主单元

- Scene 拥有 GameObjectList 作为内部对象容器
- Scene 提供完整的生命周期管理
- main.c 通过 Scene 初始化、更新和销毁所有资源
- GameObjectList 的职责已并入 Scene

### main.c 职责减轻

- main.c 只负责平台层初始化（SDL、窗口、渲染器）
- main.c 只负责主循环调度
- 所有游戏对象和 UI 的管理都委托给 Scene

### 数据流清晰

```
main.c
 ├─ init()        // 平台层初始化
 ├─ loadMedia()   // 资源加载
 ├─ gameInit()    // Scene 创建与初始化
 └─ loop()
     └─ tick()
         ├─ Scene_Update()      // GameObject 更新
         ├─ Scene_UpdateUI()    // UI 组件更新
         └─ Scene_RenderUI()    // UI 组件渲染
```

## 构建验证

### Web 构建

```bash
cd src
make EMCC=C:\emsdk\upstream\emscripten\emcc.bat emcc
```

**验证点：**
- 构建成功，无错误
- 生成 `web/UhandEngine/index.html`、`index.js`、`index.wasm`、`index.data`
- 浏览器中可打开并正常运行

### Native 构建

```bash
cd src
make
```

**验证点：**
- 构建成功，无错误
- 生成 `bin/UhandEngine/UhandEngine.exe`
- 可执行文件正常运行

## 重构收益

### 架构层面

1. **Scene-first 架构确立**
   - Scene 成为真正的运行时主单元
   - 为后续多场景、场景切换留出空间

2. **职责边界清晰**
   - 平台层（main.c）：SDL、窗口、渲染器、主循环
   - 运行时层（Scene）：GameObject、UIComponent、生命周期

3. **扩展性提升**
   - 新功能可以挂载到 Scene
   - 后续子系统（Input、Asset、Camera）有明确挂载位置

### 开发体验

1. **新开发者更容易理解**
   - 主流程清晰：平台初始化 -> Scene 创建 -> 主循环
   - 不需要理解 main.c 中的零散细节

2. **AI 友好**
   - Scene 作为明确的运行时单元，便于 AI 生成代码
   - 生命周期方法标准化，便于 AI 推断

## 后续改进方向

### 短期（第 2 周）

- 明确平台层与运行时层分界
- 将 UIComponent 从 Scene 临时字段改为正式子系统
- 建立 InputManager 抽象

### 中期（第 3-4 周）

- 将 loadMedia() 升级为 AssetManager
- 建立 Camera 抽象
- 补充更多场景示例

### 长期

- Scene 支持切换、暂停、恢复
- Scene 支持序列化与反序列化
- Scene 支持多线程更新
