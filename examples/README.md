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
演示如何使用 TextComponent 实际渲染文本。

**功能：**
- 初始化 SDL_ttf
- 加载中文字体文件
- 创建 TextComponent
- 渲染中英文文本
- 支持多种颜色和位置

**SDL 集成要点：**
- 需要 SDL2.dll 和 SDL2_ttf.dll 在运行目录
- 字体文件需要放在 examples 目录
- 使用 TTF_RenderUTF8_Blended 支持中文渲染
- 控制台编码需要设置为 UTF-8 (chcp 65001)

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

### 6. render_example.c
演示 SDL 基础渲染能力。

**功能：**
- 绘制彩色矩形
- 绘制旋转图形
- 渲染文本
- 演示 SDL 渲染循环

## 编译和运行

### 使用 makefile 编译

```bash
cd examples
make
```

编译单个示例：
```bash
make text_example
```

### 运行示例

```bash
cd examples
text_example
```

### SDL 集成要求

所有图形化示例需要以下依赖：

1. **DLL 文件**（放在 examples 目录）
   - SDL2.dll
   - SDL2_ttf.dll

2. **字体文件**（放在 examples 目录）
   - fzpix.ttf（或其他 TTF 字体文件）

3. **控制台编码**（Windows）
   - 示例中已添加 `system("chcp 65001 > nul");` 设置 UTF-8 编码

4. **链接库**（makefile 中已配置）
   - -lSDL2 -lSDL2main
   - -lSDL2_ttf

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

## SDL 集成经验总结

### 遇到的问题和解决方案

1. **找不到 SDL2.dll**
   - 问题：运行时提示找不到 SDL2.dll
   - 解决：将 SDL2.dll 和 SDL2_ttf.dll 复制到 examples 目录

2. **控制台中文乱码**
   - 问题：Windows 控制台默认 GBK 编码，中文显示乱码
   - 解决：在 main 函数开头添加 `system("chcp 65001 > nul");` 设置 UTF-8 编码

3. **字体加载失败**
   - 问题：TTF_OpenFont 找不到字体文件
   - 解决：将字体文件复制到 examples 目录，使用相对路径加载

4. **SDL_MAIN_HANDLED**
   - 问题：链接时提示 undefined reference to 'WinMain'
   - 解决：在包含 SDL 头文件前定义 `#define SDL_MAIN_HANDLED`

5. **窗口黑屏**
   - 问题：示例程序窗口显示但全黑
   - 解决：确保在主循环中调用 SDL_RenderPresent(renderer)

## 下一步

这些示例为第 2 周的 2D 基础能力提供了完整的 API 演示。后续可以：

1. ~~实现资源管理系统，让 Sprite 能够实际加载纹理~~
2. ~~集成 TextComponent，让 Text 能够实际渲染文本~~（已完成）
3. 优化渲染顺序，实现按 depth 排序
4. 实现输入系统，支持键盘和鼠标事件
5. 实现动画系统，支持精灵动画
