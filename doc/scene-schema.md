# UhandEngine Scene Schema 设计说明

## 1. 文档目标

本文档定义 `UhandEngine` 未来面向 AI 协作与声明式内容生产的场景描述结构。

目标不是立刻把当前运行时代码全面切换到 Schema 驱动，而是先建立一个稳定、可读、可生成、可校验的结构规范，使后续能够：

- 由 AI 生成场景草图
- 由工具把场景描述转换为运行时对象
- 让 PhaserJS 风格的 2D 场景组织方式在本项目中落地
- 为未来的编辑器、可视化工具和自动化测试提供统一数据入口

## 2. 设计原则

- **Scene First**
  - 所有运行内容优先挂载在 `Scene` 之下

- **Data Driven**
  - 场景内容优先用结构化数据表达，而不是散落在初始化代码里

- **AI Friendly**
  - 字段命名稳定、语义明确、默认值尽量可推断

- **Phaser-like**
  - 心智模型优先靠近 `Scene`、`GameObject`、`Container`、`Sprite`、`Text`

- **Incremental Adoption**
  - 允许当前代码继续保留手写初始化，同时逐步引入 Schema 驱动能力

## 3. Scene Schema 的角色

在未来架构中，`Scene Schema` 负责表达：

- 场景元信息
- 资源依赖
- 场景对象树
- 组件配置
- 输入交互目标
- 场景初始化参数

它不直接替代运行时代码，而是作为以下层之间的桥梁：

```text
AI Prompt / Tooling
        ↓
Scene Schema
        ↓
Scene Loader / Builder
        ↓
Runtime Scene + GameObject + Component
```

## 4. 顶层结构

建议未来采用 JSON 作为第一版交换格式。

顶层结构建议如下：

```json
{
  "schemaVersion": "0.1",
  "scene": {
    "key": "MainMenu",
    "name": "Main Menu",
    "width": 800,
    "height": 600,
    "backgroundColor": "#202020"
  },
  "assets": [],
  "objects": [],
  "systems": {},
  "metadata": {}
}
```

## 5. 顶层字段说明

## 5.1 `schemaVersion`

用途：

- 标识 Scene Schema 的版本
- 便于后续做兼容转换与迁移

约束：

- 必填
- 建议使用字符串版本号，如 `0.1`、`0.2`

## 5.2 `scene`

用途：

- 描述场景本身的基础信息

建议字段：

- `key`
- `name`
- `width`
- `height`
- `backgroundColor`
- `camera`
- `flags`

示例：

```json
{
  "key": "MainMenu",
  "name": "Main Menu",
  "width": 800,
  "height": 600,
  "backgroundColor": "#202020"
}
```

## 5.3 `assets`

用途：

- 声明该场景依赖的资源

建议资源类型：

- `image`
- `font`
- `audio`
- `atlas`
- `tilemap`

示例：

```json
[
  {
    "type": "font",
    "key": "pixel-font",
    "path": "assets/fzpix.ttf",
    "size": 16
  },
  {
    "type": "image",
    "key": "logo",
    "path": "assets/logo.png"
  }
]
```

## 5.4 `objects`

用途：

- 声明场景中的对象树或对象列表

这应该是 Scene Schema 的核心。

每个对象建议具备以下基础字段：

- `id`
- `name`
- `type`
- `active`
- `visible`
- `transform`
- `components`
- `children`
- `events`
- `tags`
- `layer`

## 5.5 `systems`

用途：

- 声明该场景启用的子系统或场景级配置

建议内容：

- 输入设置
- 摄像机设置
- 物理配置
- UI 层配置

## 5.6 `metadata`

用途：

- 存放非运行时强依赖的信息

例如：

- 作者
- 创建时间
- AI 生成来源
- 备注
- 编辑器坐标信息

## 6. 对象结构

建议单个对象结构如下：

```json
{
  "id": "title-text",
  "name": "TitleText",
  "type": "text",
  "active": true,
  "visible": true,
  "layer": 10,
  "tags": ["ui", "title"],
  "transform": {
    "x": 120,
    "y": 80,
    "scaleX": 1,
    "scaleY": 1,
    "rotation": 0,
    "originX": 0,
    "originY": 0
  },
  "components": [
    {
      "type": "Text",
      "props": {
        "text": "UhandEngine",
        "fontKey": "pixel-font",
        "color": "#ffffff"
      }
    }
  ],
  "events": [],
  "children": []
}
```

## 7. 推荐对象类型

第一阶段建议支持以下对象类型：

- `empty`
- `container`
- `sprite`
- `text`
- `button`
- `panel`

这些类型与 PhaserJS 的第一优先概念较容易对齐，也适合当前 SDL2 能力逐步承接。

## 8. Transform 结构建议

当前项目已有 `Transform`，但现状更偏通用三维坐标雏形。

为了对齐 PhaserJS 与 2D 引擎心智，建议 Schema 中优先定义 2D-first 结构：

```json
{
  "x": 0,
  "y": 0,
  "scaleX": 1,
  "scaleY": 1,
  "rotation": 0,
  "originX": 0,
  "originY": 0
}
```

后续如需要，可扩展：

- `z`
- `scrollFactorX`
- `scrollFactorY`
- `alpha`
- `flipX`
- `flipY`

## 9. Components 字段建议

建议对象的扩展能力统一通过 `components` 描述：

```json
[
  {
    "type": "Text",
    "props": {
      "text": "Start",
      "fontKey": "pixel-font",
      "color": "#ffffff"
    }
  },
  {
    "type": "Clickable",
    "props": {
      "hitWidth": 160,
      "hitHeight": 40,
      "action": "start-game"
    }
  }
]
```

设计要求：

- `type` 必填
- `props` 建议始终存在
- 不要把运行时函数直接写入 Schema
- 事件回调应通过 `action`、`eventKey` 或命令名间接表达

## 10. 事件结构建议

建议使用声明式事件绑定，而不是在 Schema 里嵌入代码。

示例：

```json
[
  {
    "event": "pointerdown",
    "action": "start-game"
  }
]
```

这样未来可以由：

- 场景构建器映射到输入系统
- AI 根据 `action` 生成处理骨架
- 编辑器可视化展示交互关系

## 11. 一个最小可用场景示例

```json
{
  "schemaVersion": "0.1",
  "scene": {
    "key": "MainMenu",
    "name": "Main Menu",
    "width": 800,
    "height": 600,
    "backgroundColor": "#202020"
  },
  "assets": [
    {
      "type": "font",
      "key": "pixel-font",
      "path": "assets/fzpix.ttf",
      "size": 16
    }
  ],
  "objects": [
    {
      "id": "title-text",
      "name": "TitleText",
      "type": "text",
      "active": true,
      "visible": true,
      "layer": 10,
      "tags": ["ui", "title"],
      "transform": {
        "x": 120,
        "y": 80,
        "scaleX": 1,
        "scaleY": 1,
        "rotation": 0,
        "originX": 0,
        "originY": 0
      },
      "components": [
        {
          "type": "Text",
          "props": {
            "text": "UhandEngine",
            "fontKey": "pixel-font",
            "color": "#ffffff"
          }
        }
      ],
      "events": [],
      "children": []
    },
    {
      "id": "start-button",
      "name": "StartButton",
      "type": "button",
      "active": true,
      "visible": true,
      "layer": 20,
      "tags": ["ui", "button"],
      "transform": {
        "x": 120,
        "y": 180,
        "scaleX": 1,
        "scaleY": 1,
        "rotation": 0,
        "originX": 0,
        "originY": 0
      },
      "components": [
        {
          "type": "Text",
          "props": {
            "text": "Start",
            "fontKey": "pixel-font",
            "color": "#ffffff"
          }
        },
        {
          "type": "Clickable",
          "props": {
            "hitWidth": 180,
            "hitHeight": 40,
            "action": "start-game"
          }
        }
      ],
      "events": [
        {
          "event": "pointerdown",
          "action": "start-game"
        }
      ],
      "children": []
    }
  ],
  "systems": {
    "input": {
      "pointer": true,
      "keyboard": true
    }
  },
  "metadata": {
    "author": "AI",
    "note": "Main menu prototype"
  }
}
```

## 12. Scene Builder 的职责建议

未来建议新增 `SceneBuilder` 或 `SceneLoader`，职责如下：

- 读取 Scene Schema
- 校验字段合法性
- 加载所需资源
- 创建 `Scene`
- 创建 `GameObject`
- 绑定 `Component`
- 挂接输入和事件

建议不要让 `Scene` 本身承担“解析 JSON”的职责，而是把解析和构建放在边界模块中。

## 13. 校验规则建议

对于 AI 生成和工具导入场景，建议至少校验：

- `schemaVersion` 是否存在
- `scene.key` 是否存在
- `objects` 是否为数组
- `id` 是否唯一
- `type` 是否为受支持类型
- `components[].type` 是否存在
- 资源 `key` 是否唯一
- 事件动作名是否符合命名规范

## 14. 命名规范建议

建议使用以下规范：

- `scene.key`：`PascalCase`，例如 `MainMenu`
- `id`：`kebab-case`，例如 `start-button`
- `name`：`PascalCase`，例如 `StartButton`
- `action`：`kebab-case`，例如 `start-game`
- `component.type`：`PascalCase`，例如 `Text`

这样更适合：

- AI 生成
- 文本 diff
- 编辑器显示
- 运行时映射

## 15. 与当前代码的关系

当前代码尚未直接消费 Scene Schema，这是正常的。

建议按以下顺序逐步接入：

1. 先完成 `Scene` 成为主运行单元
2. 再定义 `SceneBuilder` 接口
3. 先支持 `text` / `button` / `container` 这类最小对象集
4. 再扩展 `sprite`、`camera`、`animation`

## 16. 结论

`Scene Schema` 是 UhandEngine 走向 AI-first 引擎的关键中间层。

它的价值在于：

- 让场景结构脱离零散手写初始化
- 让 AI 能稳定地产生场景草图
- 让运行时、编辑器和工具链共享统一结构
- 让 PhaserJS 风格的 Scene 组织方式更容易落地
