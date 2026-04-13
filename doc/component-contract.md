# UhandEngine Component Contract 设计说明

## 1. 文档目标

本文档用于定义 `UhandEngine` 中 `Component` 的标准协议，作为当前 `Component` 基础实现向未来可扩展组件系统演进的中间规范。

该文档的目标是统一以下内容：

- 组件在运行时中的职责
- 组件与 `GameObject` / `Scene` 的关系
- 组件字段与生命周期约定
- AI 生成组件代码时应遵守的边界
- PhaserJS 对齐过程中可复用的组件扩展模型

## 2. 当前现状

当前项目中的 `Component` 已具备以下基础能力：

- `name`
- `next`
- `update`
- `draw`
- `free`

这为“组件链表 + 更新 / 渲染 / 释放”的模式提供了雏形，但还缺少稳定的统一契约，例如：

- 宿主对象引用
- 组件类型标识
- 启用/禁用状态
- 初始化与销毁边界
- 可序列化属性结构
- 与 Scene Schema 的映射规范

## 3. 组件契约设计原则

- **Small Core**
  - 先定义最小公共协议，不急于引入复杂 ECS 框架

- **Owner Explicit**
  - 组件必须能明确知道自己的宿主对象

- **Lifecycle Consistent**
  - 组件生命周期应与 `GameObject` 和 `Scene` 保持一致

- **Schema Friendly**
  - 组件应能够由 `Scene Schema` 声明并实例化

- **AI Friendly**
  - 命名与职责边界稳定，便于 AI 自动生成和维护

- **Phaser-like Evolution**
  - 允许未来构建更贴近 `Sprite`、`Text`、`Input`、`Tween` 的对象能力

## 4. 组件在架构中的位置

建议未来关系如下：

```text
Scene
 └─ GameObject
     ├─ Transform
     ├─ Render Component
     ├─ Input Component
     ├─ Text Component
     ├─ Animation Component
     └─ Custom Components
```

在这个模型下：

- `Scene` 是运行时拥有者
- `GameObject` 是组件宿主
- `Component` 是能力附着单元

## 5. 推荐最小组件协议

建议未来统一后的组件协议至少具备以下语义字段：

- `type`
- `name`
- `owner`
- `enabled`
- `next`
- `onAttach`
- `onDetach`
- `awake`
- `start`
- `update`
- `lateUpdate`
- `draw`
- `destroy`
- `free`

## 6. 字段语义说明

## 6.1 `type`

用途：

- 标识组件的稳定类型
- 供 Scene Schema、编辑器和 AI 使用

建议：

- 使用稳定字符串
- 例如：`Transform`、`Text`、`Clickable`、`SpriteRenderer`

## 6.2 `name`

用途：

- 组件实例名或展示名

说明：

- `type` 与 `name` 可以相同
- 若后续支持多同类组件，可保留实例名差异

## 6.3 `owner`

用途：

- 指向当前组件挂载的 `GameObject`

价值：

- 让组件能访问宿主 Transform、标签、场景上下文
- 是当前组件契约中最值得尽快补齐的字段之一

## 6.4 `enabled`

用途：

- 标识组件是否参与更新或渲染

建议：

- `enabled = false` 时，默认不执行 `update` / `draw`
- 是否仍允许 `destroy` 由运行时统一决定

## 6.5 `next`

用途：

- 保持当前项目使用链表组织组件的兼容性

说明：

- 这是现阶段最适合小步演进的方式
- 后续如果改为数组或池化结构，也应保留逻辑层契约不变

## 7. 生命周期建议

建议组件生命周期与 `GameObject` 生命周期对齐：

1. `onAttach`
2. `awake`
3. `start`
4. `update`
5. `lateUpdate`
6. `draw`
7. `destroy`
8. `onDetach`
9. `free`

## 7.1 `onAttach`

用途：

- 组件被添加到 `GameObject` 时调用
- 用于建立 owner 关系和初始状态

## 7.2 `awake`

用途：

- 在场景初始化早期调用
- 用于准备内部引用与状态

## 7.3 `start`

用途：

- 在所有对象和核心组件都准备好后调用

## 7.4 `update`

用途：

- 每帧逻辑更新

## 7.5 `lateUpdate`

用途：

- 在普通更新之后处理依赖顺序问题

## 7.6 `draw`

用途：

- 渲染阶段调用

说明：

- 对纯逻辑组件可为空
- 对渲染类组件必须稳定实现

## 7.7 `destroy`

用途：

- 组件销毁前的主动清理阶段

## 7.8 `free`

用途：

- 释放内存

说明：

- 与 `destroy` 区分开，便于保留逻辑清理与底层释放的边界

## 8. 组件分类建议

未来建议将组件按角色分为以下几类：

- **Core Components**
  - 例如 `Transform`

- **Render Components**
  - 例如 `Text`、`SpriteRenderer`

- **Input Components**
  - 例如 `Clickable`、`Hoverable`

- **Behavior Components**
  - 例如 `TweenPlayer`、`FollowTarget`

- **Data Components**
  - 例如 `Health`、`Inventory`

这种分类有助于：

- AI 判断组件该挂在哪类对象上
- Scene Schema 做更稳定的配置
- 后续编辑器做过滤与展示

## 9. 与 GameObject 的关系约束

组件与 `GameObject` 的关系建议满足以下规则：

- 一个组件一次只能属于一个 `GameObject`
- `GameObject` 负责组件生命周期分发
- `GameObject` 负责组件添加、移除和查询
- 组件不应直接拥有场景级全局资源，除非通过宿主或系统获取

## 10. 与 Scene 的关系约束

组件不应直接承担 Scene 管理职责。

建议：

- `Scene` 负责对象集合与系统调度
- `GameObject` 负责组件聚合
- `Component` 负责单一能力实现

这能避免把场景逻辑散落到每个组件中。

## 11. 与 Scene Schema 的映射关系

建议组件总是能映射为如下结构：

```json
{
  "type": "Text",
  "props": {
    "text": "Start",
    "fontKey": "pixel-font",
    "color": "#ffffff"
  }
}
```

映射规则建议：

- `type` 对应组件稳定类型
- `props` 对应可序列化配置
- 回调逻辑不要直接存进组件 `props`
- 行为触发用 `events`、`action` 或脚本绑定间接表达

## 12. 当前项目推荐优先标准化的组件

结合当前代码与 PhaserJS 对齐目标，建议优先标准化以下组件：

1. `Transform`
2. `Text`
3. `Clickable`
4. `SpriteRenderer`
5. `ContainerLink`
6. `AnimationPlayer`

其中：

- `Transform` 是所有对象的基础
- `Text` 最容易承接现有 `UIComponent`
- `Clickable` 最容易承接当前鼠标交互
- `SpriteRenderer` 是后续 2D 能力关键
- `ContainerLink` 用于支持父子层级
- `AnimationPlayer` 则是迈向 Phaser-like 体验的重要一步

## 13. AI 生成组件代码时的规则

AI 在为该项目新增组件时，应遵循以下规则：

- 先确认该能力是否适合作为组件，而不是直接写进 `main.c`
- 优先复用既有 `GameObject` / `Scene` 边界
- 组件只做一类能力，不混入场景调度职责
- 如果组件可声明化，应同步给出 Schema 映射示例
- 如果组件改变架构，应同步更新文档

## 14. 推荐命名规范

建议：

- 组件类型：`PascalCase`
  - 如 `Transform`、`Text`、`Clickable`

- 组件实例名：可选，若存在可用 `camelCase` 或与类型一致

- 组件工厂函数：
  - 如 `createTransform`
  - 未来可逐步统一为 `UH_Component_CreateTransform` 风格

## 15. 一个演进中的组件基类心智模型

虽然当前代码还是简单 C 结构体，但心智上建议统一为：

```text
Component =
  stable type
  + owner reference
  + enabled state
  + lifecycle hooks
  + serializable props
```

这样未来不论底层仍是链表、数组还是对象池，实现都更容易保持一致。

## 16. 近期落地建议

建议近期按最小改动顺序推进：

1. 给基础 `Component` 增加稳定 `type` 语义
2. 增加 `owner` 指针
3. 增加 `enabled` 状态
4. 明确 `Transform` 为默认基础组件
5. 从 `UIComponent` 中拆出 `Text` / `Clickable` 的文档设计
6. 再逐步修改运行时代码

## 17. 结论

`Component Contract` 是 UhandEngine 从原型组件链表走向稳定引擎组件系统的关键规范。

它的核心价值在于：

- 为运行时提供稳定边界
- 为 Scene Schema 提供映射目标
- 为 AI 提供可靠生成模板
- 为 PhaserJS 对齐提供渐进演化路径
