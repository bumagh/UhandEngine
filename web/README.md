# Web 版本运行说明

## 快速启动

### Windows

双击运行 `start-web-server.bat`，然后在浏览器中打开：

```
http://localhost:8000/UhandEngine/
```

### Linux/Mac

```bash
chmod +x start-web-server.sh
./start-web-server.sh
```

然后在浏览器中打开：

```
http://localhost:8000/UhandEngine/
```

## 手动启动

如果 Python HTTP server 脚本不可用，可以使用以下命令：

```bash
cd web
python -m http.server 8000
```

或

```bash
cd web
python3 -m http.server 8000
```

## 验证功能

打开页面后，你应该看到：

- 深灰色背景
- "Main Menu" 标题
- 三个按钮：Start Game、Settings、Quit

点击按钮后，打开浏览器控制台（F12），应该看到相应的输出：

- Start button clicked!
- Settings button clicked!
- Quit button clicked!

## 注意事项

- Web 版本必须在 HTTP server 环境下运行，不能直接用 file:// 协议打开
- 确保已安装 Python（Python 3.x）
- 端口 8000 被占用时，可以修改脚本中的端口号
