# Native 版本运行说明

## 快速启动

### Windows

双击运行 `run.bat`

### 手动启动

```bash
cd bin/UhandEngine
UhandEngine.exe
```

## 验证功能

运行后，你应该看到：

- 窗口正常打开
- 深灰色背景
- "Main Menu" 标题
- 三个按钮：Start Game、Settings、Quit

点击按钮后，查看控制台输出，应该看到：

- Start button clicked!
- Settings button clicked!
- Quit button clicked!

按 `Q` 键可以退出程序。

## 故障排除

### 黑屏问题

如果运行后黑屏，检查：

1. 字体文件是否存在：`assets/fzpix.ttf`
2. 控制台是否有错误输出
3. SDL DLL 文件是否齐全：
   - SDL2.dll
   - SDL2_ttf.dll
   - SDL2_mixer.dll

### 字体加载失败

如果控制台输出 "Failed to load font"，说明字体文件路径不正确或文件损坏。确保：

- 字体文件在 `assets/` 目录下
- 文件名为 `fzpix.ttf`
- 从正确的工作目录运行程序

### DLL 缺失

如果提示 DLL 缺失，确保以下 DLL 文件在同一目录：

- SDL2.dll
- SDL2_ttf.dll
- SDL2_mixer.dll

这些文件应该已经包含在 `bin/UhandEngine/` 目录中。
