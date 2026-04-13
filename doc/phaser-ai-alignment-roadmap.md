# UhandEngine 面向 AI 版游戏引擎的 PhaserJS 对齐路线

## 1. 目标说明

本路线文档用于定义 `UhandEngine` 为什么要优先对齐 `PhaserJS`，以及在架构、API、工具链和 AI 协作方式上如何逐步靠拢。

本项目的目标不是复制 PhaserJS 的内部实现，而是借鉴其：

- 易理解的 2D 场景组织方式
- 面向游戏脚本开发的 API 设计
- 适合快速原型和 Web 发布的工作流
- 对 AI 生成与维护友好的概念边界

## 2. 对齐原则

## 2.1 优先对齐“概念”，再对齐“接口”

先让项目拥有与 PhaserJS 相似的核心心智模型：

- Scene
- GameObject
- Display List
- Input
- Loader
- Camera
- Animation

然后再考虑是否提供相近命名或兼容层。

## 2.2 优先对齐 2D 场景能力

PhaserJS 的优势主要体现在 2D 游戏开发效率上，因此 UhandEngine 的首阶段不应该一开始就追求复杂 3D，而是先把这些做好：

- 2D 坐标系
- Sprite / Text / Container
- Camera
- Input
- Animation
- Tilemap

## 2.3 优先对齐“AI 易操作性”

面向 AI 的引擎需要具备以下特征：

- 命名统一
- 模块边界清晰
- 文档与代码可映射
- 场景与资源具备结构化描述
- 常用任务能被模板化

## 3. 当前基础与目标映射

| 当前模块 | 当前状态 | PhaserJS 对应概念 | 后续动作 |
| --- | --- | --- | --- |
| `main.c` 主循环 | 已存在 | Game boot / step loop | 拆分为 App、SceneManager、RenderLoop |
| `GameObject` | 已存在 | GameObject | 增加 owner、active、visible、tag、layer |
| `Component` | 已存在 | Component-like extension | 形成统一协议与元数据 |
| `Transform` | 已存在 | Transform | 扩为 2D first transform |
| `GameObjectList` | 已存在 | Display List / Update List | 融入 Scene 内部结构 |
| `Scene` | 初级 | Scene | 成为主调度单元 |
| `UIComponent` | 已存在 | Text / UI object | 拆分为 Text、Panel、Button |
| `event` | 已存在 | Event emitter | 统一为 engine event bus |
| `loadMedia()` | 初级 | Loader | 建立 AssetManager |

## 4. 需要优先建立的 Phaser-like 能力

## 4.1 Scene System

目标：让所有运行内容都附属于场景。

建议能力：

- `Scene_Init`
- `Scene_Preload`
- `Scene_Create`
- `Scene_Update`
- `Scene_Destroy`
- 场景切换
- 场景暂停 / 恢复

## 4.2 Display Object / GameObject System

目标：形成可渲染对象层级。

建议能力：

- 基础对象属性：`x`、`y`、`scaleX`、`scaleY`、`rotation`、`visible`
- 对象树或容器结构
- 绘制顺序控制
- 命中测试

## 4.3 Asset / Loader System

目标：替代零散的 `loadMedia()`。

建议能力：

- 字体加载
- 图片加载
- 音频加载
- Web 端预加载
- 资源缓存表
- 资源 key 管理

## 4.4 Input System

目标：把 SDL 输入事件转成统一高层事件。

建议能力：

- Pointer 输入
- Keyboard 输入
- 交互区域注册
- 点击、按下、抬起、悬停
- 后续触摸与多指扩展

## 4.5 Animation / Tween System

目标：提升 2D 表现力与 AI 生成样例质量。

建议能力：

- 精灵帧动画
- 属性补间
- 时间线
- 缓动函数

## 4.6 Camera System

目标：让场景具备视口抽象。

建议能力：

- 跟随目标
- 平移缩放
- 边界限制
- UI 与世界层分离

## 5. 面向 AI 的引擎增强项

## 5.1 Prompt-Friendly API

建议未来 API 保持：

- 单一职责明确
- 函数命名稳定
- 参数顺序固定
- 生命周期接口清晰

例如：

- `UH_Scene_Create()`
- `UH_GameObject_AddComponent()`
- `UH_Asset_LoadTexture()`
- `UH_Input_IsKeyPressed()`

## 5.2 可声明场景

建议在 2-4 周内定义场景描述结构，例如 JSON 或简化 DSL：

```json
{
  "scene": "MainMenu",
  "objects": [
    { "type": "text", "name": "title", "x": 100, "y": 80, "text": "UhandEngine" },
    { "type": "button", "name": "start", "x": 120, "y": 180, "text": "Start" }
  ]
}
```

这样 AI 可以直接生成：

- 场景草图
- UI 结构
- 测试关卡
- 资源绑定代码

## 5.3 建立技能化任务模板

未来建议沉淀出以下 AI 技能模板：

- 创建新场景
- 添加可点击按钮
- 创建角色对象
- 接入贴图资源
- 添加补间动画
- 生成测试关卡
- 导出 Web 示例

## 6. 推荐演进顺序

推荐按以下优先级推进：

1. Scene 成为核心单元
2. Transform 改为 2D first
3. 把 GameObjectList 合并到 Scene 管理
4. 引入 AssetManager
5. 抽象 InputManager
6. 引入 Sprite / Text / Container
7. 引入 Camera
8. 引入 Animation / Tween
9. 定义场景声明格式
10. 建立 AI Prompt / Skills / Rules

## 7. 成功判定标准

当 UhandEngine 满足以下条件时，可认为已经初步完成 PhaserJS 对齐：

- 新场景可以在固定模板下快速创建
- 常见 2D 对象有稳定抽象
- 输入、资源、相机、动画有统一入口
- Web 发布流程稳定
- AI 可以根据规则文档可靠生成 60% 以上样板代码
- 文档、目录、命名对 AI 足够友好

## 8. 结论

UhandEngine 当前最适合的发展路径不是盲目扩张功能，而是：

- 保留 C + SDL2 + Web 的跨平台内核优势
- 在 API 设计与使用体验上向 PhaserJS 靠拢
- 在工程组织与文档规范上向 AI 原生协作靠拢

这样未来它既可以成为一个轻量引擎，也可以成为一个**适合 AI 参与构建游戏内容的实验型平台**。
