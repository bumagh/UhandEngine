# 第1周架构重构测试指南

## 测试目标

验证第1周架构重构后的代码是否能正常编译和运行，确保 Scene-first 架构不会破坏现有功能。

## 测试环境

- **Native 平台**：Windows（使用 gcc 编译）
- **Web 平台**：Emscripten（使用 emcc 编译）

## 测试步骤

### 步骤 1：编译 Scene 和 GameObject 模块

```bash
cd src
gcc -c engine/Scene.c -Iinclude -o engine/Scene.o
gcc -c engine/gameObject.c -Iinclude -o engine/gameObject.o
gcc -c engine/transform.c -Iinclude -o engine/transform.o
gcc -c engine/component.c -Iinclude -o engine/component.o
gcc -c engine/RenderQueue.c -Iinclude -o engine/RenderQueue.o
gcc -c platform.c -Iinclude -o platform.o
```

**预期结果**：所有模块编译成功，无错误。

### 步骤 2：编译 main.c

```bash
gcc -c main.c -Iinclude -o main.o
```

**预期结果**：main.c 编译成功，无错误。

**可能的问题**：
- SDL.h 路径错误：需要确保 SDL2 开发库已正确安装
- 缺少依赖：需要确保 SDL2_ttf 开发库已正确安装

### 步骤 3：链接生成 Native 可执行文件

```bash
gcc *.o engine/*.o -o game.exe -L../../lib -lSDL2 -lSDL2_ttf -lSDL2_mixer -mconsole
```

**预期结果**：生成 game.exe，无链接错误。

### 步骤 4：运行 Native 版本

```bash
./game.exe
```

**预期结果**：
- 窗口正常打开
- Scene 创建成功
- 程序能正常退出（按 Q 键或关闭窗口）

**验证点**：
- [ ] 窗口标题显示 "UhandEngine"
- [ ] 控制台输出 Scene 创建成功信息
- [ ] 能正常退出

### 步骤 5：编译 Web 版本

```bash
cd src
emcc game.c main.c engine/Scene.c engine/gameObject.c engine/transform.c engine/component.c engine/RenderQueue.c platform.c \
  -Wno-int-conversion \
  -Wno-implicit-function-declaration \
  -Wno-incompatible-function-pointer-types \
  -s USE_SDL=2 \
  -s USE_SDL_TTF=2 \
  -s WASM=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s ASYNCIFY=1 \
  -Iinclude \
  -o game.html \
  --shell-file shell.html \
  --preload-file simhei.ttf \
  --preload-file arial.ttf \
  -std=c99
```

**预期结果**：生成 game.html、game.js、game.wasm，无编译错误。

### 步骤 6：运行 Web 版本

```bash
# 启动本地 HTTP 服务器
python -m http.server 8000

# 在浏览器中打开
http://localhost:8000/game.html
```

**预期结果**：
- 浏览器中显示游戏窗口
- 控制台无错误信息
- 程序能正常运行

**验证点**：
- [ ] 浏览器控制台无错误
- [ ] 游戏窗口正常显示
- [ ] 中文文本正常显示（如果使用了中文字体）

## 功能验证

### Scene-first 架构验证

**验证点**：
- [ ] Scene 使用 rootObject 管理 GameObject
- [ ] GameObject 使用父子关系（firstChild/nextSibling）
- [ ] Scene 生命周期函数正常调用（Awake、Start、Update）
- [ ] GameObject 生命周期函数正常调用

### 平台层抽象验证

**验证点**：
- [ ] platform.c 正确初始化 SDL
- [ ] platform.c 正确创建窗口和渲染器
- [ ] platform.c 正确清理资源
- [ ] Web 平台使用 emscripten 主循环

## 故障排除

### 编译错误

**问题**：SDL.h: No such file or directory
**解决**：安装 SDL2 开发库
- Windows：从 SDL2 官网下载开发库
- Linux：`sudo apt-get install libsdl2-dev`
- macOS：`brew install sdl2`

**问题**：undefined reference to `SDL_xxx`
**解决**：确保链接时正确指定 SDL2 库路径和库名

### 运行错误

**问题**：窗口无法打开
**解决**：检查 SDL 初始化是否成功，查看控制台错误信息

**问题**：中文显示乱码
**解决**：确保使用了 TTF_RenderUTF8_Solid 和正确的字体文件

## 测试报告模板

测试完成后，请填写以下报告：

```
测试日期：____
测试人员：____
测试环境：____

编译结果：
- Scene.o: [通过/失败]
- gameObject.o: [通过/失败]
- main.o: [通过/失败]
- game.exe: [通过/失败]
- game.html: [通过/失败]

运行结果：
- Native 版本: [通过/失败]
- Web 版本: [通过/失败]

功能验证：
- Scene-first 架构: [通过/失败]
- 平台层抽象: [通过/失败]

问题记录：
- [问题描述]
- [问题描述]

建议：
- [改进建议]
```

## 下一步

测试通过后，可以进入第2周：建立 Phaser-like 2D 基础能力。

测试失败时，需要先修复问题，确保基础架构稳定后再继续。
