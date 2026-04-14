# UhandEngine Web 发布执行清单

## 1. 文档用途

本文档用于在 UhandEngine 当前 Web-first MVP 基础上，执行一次最小可控的 Web 发布前检查。

目标不是一次性把项目做成完整商业产品，而是保证当前 Web 版本已经达到：

- 可重新构建
- 可重新验证
- 可对外演示
- 可静态部署
- 可作为后续推广原型使用

本文档可与以下文档配合使用：

- `doc/mvp-demo-steps.md`
- `doc/mvp-acceptance.md`
- `doc/ai-to-mvp-mapping.md`
- `doc/swe1.6-release-promotion-engine-first-prompt.md`

## 2. 发布前范围确认

在执行发布前，先确认本次发布范围。

当前建议发布范围应仅包含：

- 当前 Web 菜单场景
- 当前最小 UI 和交互能力
- 当前 AI 生成 Scene Schema 的工具链说明
- 当前示例 JSON 与映射说明

当前不应作为发布承诺的内容：

- 完整 Scene-first 架构改造
- 完整 Scene Schema runtime loader
- 完整编辑器
- 完整 Physics / Animation / Tween
- 完整多示例产品化体系

## 3. 构建前检查

发布前必须先检查以下事项：

### 3.1 构建环境

- 已安装 Emscripten SDK
- `emcc` 路径可用
- 本地 `src/makefile` 可正常执行 Web 构建命令
- Python 环境可用（如果要演示 AI 工具链）

### 3.2 关键文件存在

确认以下文件存在并可读：

- `src/main.c`
- `src/makefile`
- `web/html_template/shell.html`
- `assets/`
- `scripts/ai-client.py`
- `examples/scene-schema-example.json`

### 3.3 安全检查

发布前确认：

- `.env` 没有提交到仓库
- API Key 没有写入源码
- API Key 没有写入前端 HTML/JS
- API Key 没有写入 README 或 doc 示例文本

## 4. Web 构建检查

使用当前项目约定的构建方式重新构建 Web 版本。

推荐命令：

```bash
cd src
make EMCC=C:\emsdk\upstream\emscripten\emcc.bat emcc
```

构建完成后，应确认以下文件已生成：

- `web/UhandEngine/index.html`
- `web/UhandEngine/index.js`
- `web/UhandEngine/index.wasm`
- `web/UhandEngine/index.data`

## 5. 本地运行检查

### 5.1 页面基础可用性

打开 `web/UhandEngine/index.html` 后，确认：

- 页面可以正常加载
- 页面不是空白
- 页面没有阻断运行的致命错误

### 5.2 当前 MVP 视觉检查

确认页面至少显示：

- `Main Menu`
- `Start Game`
- `Settings`
- `Quit`

### 5.3 当前 MVP 交互检查

点击按钮后，确认控制台有对应输出：

- `Start button clicked!`
- `Settings button clicked!`
- `Quit button clicked!`

### 5.4 容错说明

如果存在非阻断 warning，但页面可正常运行，应：

- 记录 warning
- 判断是否影响对外演示
- 如不影响，可进入下一步

## 6. AI 工具链检查

如果本次发布要同时展示 AI 能力，发布前还要验证以下内容：

### 6.1 AI 脚本可运行

确认以下命令可工作：

```bash
python scripts/ai-client.py "创建一个包含标题和按钮的主菜单场景"
```

### 6.2 AI 输出格式检查

确认 AI 输出满足：

- 是结构化 JSON
- 与 `scene-schema.md` 大体兼容
- 可以与当前示例映射说明对应上

### 6.3 示例链路检查

确认以下链路是可解释的：

1. 用户输入自然语言
2. AI 生成 Scene Schema
3. 当前 MVP 通过示例 JSON + 手工映射方式接入
4. Web 页面展示当前场景结果

## 7. 静态部署检查

当前 Web 构建产物应适合静态托管部署。

发布前确认：

- 资源路径相对关系正确
- 页面不依赖本地私有路径
- 所有必要资源都包含在发布目录中
- 浏览器打开发布目录中的 `index.html` 时可正常工作

如果准备部署到静态托管平台，建议额外确认：

- 入口文件明确
- 无服务器端必需依赖
- 发布目录清晰

## 8. 对外演示材料检查

在对外发布或推广前，至少确认以下文档齐备：

- `README.md`
- `doc/mvp-demo-steps.md`
- `doc/mvp-acceptance.md`
- `doc/ai-to-mvp-mapping.md`
- `doc/web-release-checklist.md`

如果要给 AI 代理继续接手，还应具备：

- `doc/swe1.6-current-state-prompt.md`
- `doc/swe1.6-release-promotion-engine-first-prompt.md`

## 9. 发布前人工检查清单

发布前逐项人工确认：

- 当前演示边界是否清楚
- README 是否没有夸大当前能力
- 文档是否与当前代码状态一致
- 演示步骤是否能被新读者执行
- AI 功能说明是否没有泄露密钥
- 是否明确说明当前仍是 MVP / 原型阶段

## 10. 发布完成判定

满足以下条件，即可认为当前 Web 版本具备最小发布条件：

- Web 构建成功
- 页面可正常打开
- 页面具备可见菜单和可点击按钮
- 文档可说明如何演示
- AI 工具链可单独运行
- 安全边界没有明显问题
- 仓库阅读入口清晰

## 11. 发布后建议立即补做的事项

发布完成后，建议立即进入下一轮最小迭代：

- 把当前 menu demo 的职责从 `main.c` 轻量下沉
- 为 `Scene` 补一个更明确的 runtime owner 台阶
- 为多个示例建立统一入口
- 为 Web 演示准备更清晰的示例导航页

## 12. 最小发布说明模板

对外说明当前版本时，建议使用类似表述：

> 当前版本是 UhandEngine 的 Web-first MVP。
> 它验证了三个核心方向：
> 1. 基于 C + SDL2 + Emscripten 的 Web 运行能力
> 2. 最小 Text / Clickable 场景演示能力
> 3. AI 生成 Scene Schema 并接入当前演示流程的可行性
>
> 当前版本仍是原型，不代表完整引擎能力，但已经具备后续继续演进、展示和推广的基础。
