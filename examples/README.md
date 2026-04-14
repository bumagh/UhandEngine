# UhandEngine Examples

本目录包含演示 UhandEngine 2D 基础能力的示例代码。

## 示例列表

### 1. sprite_example.c
演示如何创建和使用 Sprite GameObject。

**功能：**
- 创建 Sprite GameObject
- 设置 Transform 2D 属性（position, rotation, scale, origin）
- 展示 visible/active/depth 属性

**注意：** 当前 Sprite 是简化实现，需要资源管理系统支持纹理加载。本示例展示 API 设计。

### 2. text_example.c
演示如何创建和使用 Text GameObject。

**功能：**
- 创建 Text GameObject
- 设置文本内容和颜色
- 展示位置和属性设置

**注意：** 当前 Text 是简化实现，需要集成 TextComponent。本示例展示 API 设计。

### 3. container_example.c
演示如何创建 Container 并管理父子关系。

**功能：**
- 创建 Container GameObject
- 添加子对象到 Container
- 演示父子关系
- 演示可见性继承（父对象不可见时子对象也不可见）
- 演示 removeChild 操作

### 4. properties_example.c
演示 GameObject 的 visible、active、depth 属性的使用。

**功能：**
- 设置不同的 depth（渲染层级）
- 控制可见性（visible）
- 控制激活状态（active）
- 动态修改属性
- 展示属性对渲染和更新的影响

### 5. scene_example.c
综合示例，演示如何在 Scene 中使用 GameObject 的 2D 基础能力。

**功能：**
- 创建 Scene
- 添加多个 GameObject 到 Scene
- 演示父子关系
- 演示可见性控制
- 演示激活状态控制
- 演示 Transform 2D 属性
- 调用 Scene 生命周期方法

## 编译示例

### 编译单个示例

```bash
cd src
gcc -o ../examples/sprite_example ../examples/sprite_example.c engine/gameObject.c engine/transform.c engine/algorithm.c engine/component.c -I../include -g -Wall
```

### 运行示例

```bash
cd examples
./sprite_example
```

## 第 2 周新增功能总结

这些示例展示了第 2 周完成的核心功能：

1. **Transform 2D First 结构**
   - position (x, y)
   - rotation (弧度)
   - scale (scaleX, scaleY)
   - origin/pivot (originX, originY)

2. **GameObject 基础属性**
   - visible (可见性)
   - active (激活状态)
   - depth (渲染层级)
   - parent/children (父子关系)

3. **GameObject 类型抽象**
   - GAMEOBJECT_TYPE_BASE
   - GAMEOBJECT_TYPE_SPRITE
   - GAMEOBJECT_TYPE_TEXT
   - GAMEOBJECT_TYPE_CONTAINER

4. **辅助函数**
   - createSprite()
   - createText()
   - createContainer()
   - addChild()
   - removeChild()
   - setVisible()
   - setActive()
   - setDepth()

5. **Scene 渲染支持**
   - Scene_RenderGameObjects()
   - 支持 visible/active 检查
   - 支持父子关系可见性继承

## 下一步

这些示例为第 2 周的 2D 基础能力提供了完整的 API 演示。后续可以：

1. 实现资源管理系统，让 Sprite 能够实际加载纹理
2. 集成 TextComponent，让 Text 能够实际渲染文本
3. 优化渲染顺序，实现按 depth 排序
4. 实现输入系统，支持键盘和鼠标事件
