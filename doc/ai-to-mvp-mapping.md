# AI 输出到 MVP 映射说明

## 概述

本文档说明如何将 AI 生成的 Scene Schema 映射到 UhandEngine MVP 的代码实现。

## AI 输出示例

AI 生成的 Scene Schema 存储在 `examples/scene-schema-example.json`，包含：

- 场景配置（背景色、尺寸）
- 资源定义（字体）
- 对象列表（标题文本、按钮）
- 组件配置（Text、Clickable）
- 事件系统（pointerdown）

## 映射关系

### 1. 场景配置

**AI 输出**：
```json
{
  "scene": {
    "backgroundColor": "#202020",
    "width": 800,
    "height": 600
  }
}
```

**MVP 实现**：
```c
SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
SDL_RenderClear(renderer);
```

### 2. 对象映射

**AI 输出**：
```json
{
  "id": "title-text",
  "name": "TitleText",
  "transform": {
    "x": 400,
    "y": 150
  }
}
```

**MVP 实现**：
```c
UIComponent *titleText = createUIComponent(
    w/2 - 100,  // x
    h/2 - 120,  // y
    200,        // width
    60,         // height
    titleBgColor,
    "Main Menu",
    font,
    titleColor,
    NULL
);
```

### 3. 组件映射

#### Text 组件

**AI 输出**：
```json
{
  "type": "Text",
  "props": {
    "text": "Main Menu",
    "color": "#ffffff",
    "fontSize": 48
  }
}
```

**MVP 实现**：
```c
SDL_Color titleColor = {255, 255, 255, 255};
createUIComponent(..., "Main Menu", font, titleColor, NULL);
```

#### Clickable 组件

**AI 输出**：
```json
{
  "type": "Clickable",
  "props": {
    "action": "start-game"
  }
}
```

**MVP 实现**：
```c
void onStartButtonClick(void *data) {
    printf("Start button clicked!\n");
}

startButton = createUIComponent(..., onStartButtonClick);
```

### 4. 事件映射

**AI 输出**：
```json
{
  "event": "pointerdown",
  "action": "start-game"
}
```

**MVP 实现**：
```c
// UIComponent 内部处理
void updateUIComponent(Component *comp) {
    UIComponent *uiComp = (UIComponent *)comp;
    int mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
    if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
        mouseX > uiComp->rect.x && mouseX < (uiComp->rect.x + uiComp->rect.w) &&
        mouseY > uiComp->rect.y && mouseY < (uiComp->rect.y + uiComp->rect.h))
    {
        if (uiComp->onClick) {
            uiComp->onClick(comp);
        }
    }
}
```

## 当前 MVP 限制

当前 MVP 采用手动映射方式，不包含自动解析器。原因是：

1. MVP 目标是验证可行性，不需要完整解析器
2. 手动映射更简单、更可控
3. 可以快速迭代和调整

## 未来改进方向

1. **自动解析器**：编写脚本自动将 Scene Schema 转换为 C 代码
2. **运行时加载**：在运行时直接加载和解析 Scene Schema JSON
3. **完整组件支持**：支持更多组件类型（Sprite、Container 等）
4. **事件系统**：实现更完整的事件系统

## 使用流程

1. 使用 AI 客户端生成 Scene Schema：
   ```bash
   python scripts/ai-client.py "创建一个包含标题和按钮的主菜单场景" -o output.json
   ```

2. 手动将 Scene Schema 映射到 main.c：
   - 根据 `objects` 数组创建 UIComponent
   - 根据 `components` 配置组件属性
   - 根据 `events` 配置事件处理

3. 重新编译和构建：
   ```bash
   make
   make EMCC=C:\emsdk\upstream\emscripten\emcc.bat emcc
   ```

4. 在浏览器中验证结果。

## 示例

完整的示例请参考：
- AI 输出：`examples/scene-schema-example.json`
- MVP 实现：`src/main.c` 中的 `gameInit()` 函数
