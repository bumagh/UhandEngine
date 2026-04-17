# Scene-first 架构设计

## 设计目标

让 Scene 成为真正的运行入口，main.c 只负责平台初始化和清理。

## 架构层次

```
┌─────────────────────────────────────────────────────────────┐
│                        main.c                              │
│                    (平台层 - Platform Layer)                 │
│  - SDL/Emscripten 初始化                                    │
│  - 平台特定资源加载                                         │
│  - 主循环调度                                               │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      Engine                                 │
│                  (引擎层 - Engine Layer)                     │
│  - 场景管理（Scene 切换、Scene 栈）                          │
│  - 全局资源（Window、Renderer、Font）                        │
│  - 输入管理（InputManager）                                  │
│  - 资源管理（AssetManager）                                  │
│  - 时间管理（TimeManager）                                    │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                       Scene                                 │
│                  (场景层 - Scene Layer)                       │
│  - GameObject 管理（统一管理所有对象）                        │
│  - 生命周期管理（Awake、Start、Update、Destroy）              │
│  - 渲染管理（RenderQueue）                                   │
│  - 场景特定逻辑                                              │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                   GameObject                                 │
│                  (对象层 - Object Layer)                      │
│  - Transform（位置、旋转、缩放）                              │
│  - Component 组件系统                                        │
│  - 父子关系                                                  │
│  - 生命周期                                                  │
└─────────────────────────────────────────────────────────────┘
```

## 核心结构设计

### Engine 结构

```c
typedef struct Engine {
    // 平台资源
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *defaultFont;
    
    // 场景管理
    Scene *currentScene;
    Scene *sceneStack[MAX_SCENE_STACK];
    int sceneStackTop;
    
    // 子系统
    InputManager *inputManager;
    AssetManager *assetManager;
    TimeManager *timeManager;
    
    // 运行状态
    int isRunning;
    int targetFPS;
    
    // 平台特定
    #ifdef __EMSCRIPTEN__
    int isWebPlatform;
    #endif
} Engine;
```

### Scene 结构（重构）

```c
typedef struct Scene {
    // 场景标识
    const char *name;
    const char *id;
    
    // 对象管理（统一管理，不再区分 UIComponent）
    GameObject *rootObject;  // 根对象（替代 GameObjectList）
    
    // 渲染队列
    RenderQueue *renderQueue;
    
    // 生命周期状态
    int isAwake;
    int isStarted;
    
    // 场景特定数据
    void *userData;
    
    // 生命周期函数指针
    void (*Awake)(Scene *self);
    void (*Start)(Scene *self);
    void (*Update)(Scene *self);
    void (*Destroy)(Scene *self);
    
    // 渲染函数
    void (*Render)(Scene *self, SDL_Renderer *renderer);
    
    // 场景切换回调
    void (*OnEnter)(Scene *self, Scene *from);
    void (*OnExit)(Scene *self, Scene *to);
} Scene;
```

### GameObject 结构（重构）

```c
typedef struct GameObject {
    // 基础属性
    const char *name;
    const char *id;
    GameObjectType type;
    
    // 2D 基础属性
    int visible;
    int active;
    int depth;  // 渲染层级
    
    // 父子关系
    GameObject *parent;
    GameObject *firstChild;  // 第一个子对象
    GameObject *nextSibling; // 下一个兄弟对象
    
    // 组件系统
    Component *firstComponent;
    Transform *transform;
    
    // 生命周期
    void (*Awake)(GameObject *self);
    void (*Start)(GameObject *self);
    void (*Update)(GameObject *self);
    void (*Destroy)(GameObject *self);
} GameObject;
```

## 运行流程

### 初始化流程

```
main()
  ├─ Engine_Create()
  │   ├─ SDL_Init()
  │   ├─ SDL_CreateWindow()
  │   ├─ SDL_CreateRenderer()
  │   ├─ InputManager_Create()
  │   ├─ AssetManager_Create()
  │   └─ TimeManager_Create()
  ├─ Engine_LoadInitialScene(sceneName)
  │   └─ Scene_Create() -> Scene_Awake() -> Scene_Start()
  └─ Engine_Run()
      └─ 主循环
```

### 主循环（Engine 层）

```c
void Engine_Run(Engine *engine) {
    engine->isRunning = 1;
    
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(Engine_Tick, engine, 0, 1);
    #else
    while (engine->isRunning) {
        Engine_Tick(engine);
    }
    #endif
}

void Engine_Tick(Engine *engine) {
    // 1. 时间管理
    TimeManager_Update(engine->timeManager);
    
    // 2. 输入处理
    InputManager_Update(engine->inputManager);
    
    // 3. 场景更新
    if (engine->currentScene) {
        Scene_Update(engine->currentScene);
    }
    
    // 4. 渲染
    if (engine->currentScene) {
        SDL_RenderClear(engine->renderer);
        Scene_Render(engine->currentScene, engine->renderer);
        SDL_RenderPresent(engine->renderer);
    }
    
    // 5. 帧率控制
    TimeManager_WaitForTargetFPS(engine->timeManager);
}
```

### 场景更新（Scene 层）

```c
void Scene_Update(Scene *scene) {
    // 递归更新所有 GameObject
    UpdateGameObjectTree(scene->rootObject);
    
    // 场景特定逻辑
    if (scene->Update) {
        scene->Update(scene);
    }
}

void UpdateGameObjectTree(GameObject *obj) {
    if (!obj || !obj->active) return;
    
    // 更新当前对象
    if (obj->Update) {
        obj->Update(obj);
    }
    
    // 更新组件
    UpdateComponents(obj);
    
    // 递归更新子对象
    GameObject *child = obj->firstChild;
    while (child) {
        UpdateGameObjectTree(child);
        child = child->nextSibling;
    }
}
```

### 场景渲染（Scene 层）

```c
void Scene_Render(Scene *scene, SDL_Renderer *renderer) {
    // 按深度排序渲染
    RenderGameObjectTree(scene->rootObject, renderer);
    
    // 场景特定渲染
    if (scene->Render) {
        scene->Render(scene, renderer);
    }
}

void RenderGameObjectTree(GameObject *obj, SDL_Renderer *renderer) {
    if (!obj || !obj->visible) return;
    
    // 先渲染子对象（从后往前，保证父对象在子对象之后）
    GameObject *child = obj->firstChild;
    while (child) {
        RenderGameObjectTree(child, renderer);
        child = child->nextSibling;
    }
    
    // 渲染当前对象
    RenderGameObject(obj, renderer);
}
```

## 场景管理

### 场景切换

```c
void Engine_LoadScene(Engine *engine, const char *sceneName) {
    Scene *oldScene = engine->currentScene;
    Scene *newScene = SceneFactory_Create(sceneName);
    
    if (newScene) {
        // 退出旧场景
        if (oldScene) {
            if (oldScene->OnExit) {
                oldScene->OnExit(oldScene, newScene);
            }
            Scene_Destroy(oldScene);
        }
        
        // 进入新场景
        engine->currentScene = newScene;
        Scene_Awake(newScene);
        Scene_Start(newScene);
        
        if (newScene->OnEnter) {
            newScene->OnEnter(newScene, oldScene);
        }
    }
}
```

### 场景栈（用于暂停/恢复）

```c
void Engine_PushScene(Engine *engine, Scene *scene) {
    if (engine->sceneStackTop < MAX_SCENE_STACK) {
        engine->sceneStack[engine->sceneStackTop++] = engine->currentScene;
        engine->currentScene = scene;
        Scene_Awake(scene);
        Scene_Start(scene);
    }
}

void Engine_PopScene(Engine *engine) {
    if (engine->sceneStackTop > 0) {
        Scene_Destroy(engine->currentScene);
        engine->currentScene = engine->sceneStack[--engine->sceneStackTop];
    }
}
```

## 重构步骤

### 阶段 1：创建 Engine 结构
1. 创建 `engine.h` 和 `engine.c`
2. 定义 Engine 结构体
3. 实现 Engine_Create() 和 Engine_Destroy()
4. 将全局变量（window、renderer、font）移入 Engine

### 阶段 2：重构 Scene
1. 移除 Scene 中的 GameObjectList 和 UIComponents
2. 使用 rootObject 替代 GameObjectList
3. 统一 GameObject 和 UIComponent
4. 实现场景管理函数

### 阶段 3：重构 GameObject
1. 移除链表节点（next）
2. 使用父子关系（firstChild、nextSibling）
3. 明确 Component 与 GameObject 的关系

### 阶段 4：分离平台层
1. 创建 `platform.h` 封装平台相关代码
2. main.c 只调用 platform 初始化和清理
3. 将 SDL 特定代码移入 platform 层

### 阶段 5：集成子系统
1. 将 InputManager 集成到 Engine
2. 将 AssetManager 集成到 Engine
3. 统一输入事件分发

## 验收标准

- [ ] main.c 只负责平台初始化和清理
- [ ] Scene 成为真正的运行入口
- [ ] GameObject 使用父子关系而非链表
- [ ] 平台层与运行时层明确分离
- [ ] 场景切换功能正常工作
- [ ] 新开发者能在 10 分钟内读懂主流程

## 风险与应对

### 风险 1：重构过程中破坏现有功能
**应对**：
- 保留现有代码作为备份
- 分阶段重构，每个阶段都测试
- 使用 git 分支管理重构进度

### 风险 2：Web 平台兼容性问题
**应对**：
- 每个阶段都在 Web 平台测试
- 保持 ASYNCIFY 等编译标志
- 确保 Emscripten 主循环正常工作

### 风险 3：重构工作量超出预期
**应对**：
- 优先完成核心架构（Engine + Scene）
- 其他子系统可以后续集成
- 如果时间不足，可以简化部分功能
