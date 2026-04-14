# UhandEngine MVP 演示步骤

## 1. 目标

本文档用于说明如何在当前仓库状态下完成 UhandEngine Web-first MVP 的最小演示。

当前 MVP 重点展示三件事：

- Web 版本可以运行
- 最小菜单场景可以显示并交互
- AI 可以生成 Scene Schema，并已有一条接入演示链路

## 2. 演示前准备

### 2.1 环境

需要具备以下环境：

- Windows
- Python
- Emscripten SDK
- 浏览器

### 2.2 当前关键文件

演示会涉及以下文件：

- `src/main.c`
- `src/makefile`
- `web/UhandEngine/index.html`
- `scripts/ai-client.py`
- `examples/scene-schema-example.json`
- `doc/ai-to-mvp-mapping.md`

## 3. 演示路径

## 3.1 演示一：Web MVP 可运行

在项目根目录确认 Web 构建产物存在：

- `web/UhandEngine/index.html`
- `web/UhandEngine/index.js`
- `web/UhandEngine/index.wasm`
- `web/UhandEngine/index.data`

如需重新构建：

```bash
cd src
make EMCC=C:\emsdk\upstream\emscripten\emcc.bat emcc
```

然后在浏览器中打开：

- `web/UhandEngine/index.html`

### 预期结果

页面应显示：

- 深灰背景
- 标题 `Main Menu`
- `Start Game` 按钮
- `Settings` 按钮
- `Quit` 按钮

## 3.2 演示二：最小交互闭环

在浏览器中点击三个按钮，观察控制台输出：

- 点击 `Start Game`
  - 输出 `Start button clicked!`
- 点击 `Settings`
  - 输出 `Settings button clicked!`
- 点击 `Quit`
  - 输出 `Quit button clicked!`

### 说明

这一步用于证明：

- 文本显示能力已具备
- Clickable / Button 能力已具备
- Web 场景中已经形成最小显示 + 交互闭环

## 3.3 演示三：AI 生成 Scene Schema

在项目根目录执行：

```bash
python scripts/ai-client.py "创建一个包含标题和按钮的主菜单场景"
```

也可以输出到文件：

```bash
python scripts/ai-client.py "创建一个包含标题和按钮的主菜单场景" -o output.json
```

### 预期结果

脚本返回结构化 JSON，内容包括：

- scene 基本配置
- text 对象
- button 对象
- Text / Clickable 组件
- pointerdown 等事件描述

## 3.4 演示四：AI 输出已接入当前 MVP

查看以下文件：

- `examples/scene-schema-example.json`
- `doc/ai-to-mvp-mapping.md`
- `src/main.c`

### 说明方式

演示时可以这样解释：

1. AI 先生成 `scene-schema-example.json`
2. 当前 MVP 采用“手工映射”的最小接入方案
3. `main.c` 中当前的菜单实现，已经参考 AI 输出完成映射
4. 这证明了“自然语言 -> Scene Schema -> MVP 演示场景”的链路已经可验证

## 4. 5 分钟演示建议话术

建议按如下顺序演示：

1. 先打开浏览器页面
2. 展示菜单 UI 和按钮点击
3. 打开 `scene-schema-example.json`
4. 展示 `scripts/ai-client.py` 的调用方式
5. 展示 `doc/ai-to-mvp-mapping.md`，说明当前采用手工映射
6. 总结：Web 已跑通、AI 已接入、AI 协作链路已建立

## 5. 当前演示边界

当前演示明确不包含：

- 完整 Scene Loader
- 完整编辑器
- 完整 Animation / Tween
- 完整 Physics
- 完整资源管理器

## 6. 演示后建议

演示完成后，下一阶段建议优先考虑：

- 为 `Scene` 补一个最小 runtime owner 台阶
- 为 Scene Schema 补一个最小半自动映射步骤
- 把当前菜单 demo 从 `main.c` 中轻量下沉
