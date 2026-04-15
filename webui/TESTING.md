# WebUI 前后端联调测试指南

## 前置条件

1. 已安装 Node.js
2. 已安装前端和后端依赖
3. 后端服务器已启动

## 启动步骤

### 1. 启动后端服务器

在新的终端窗口中运行：

```bash
cd d:\3learn\3GameEngine\UhandEngine\webui\backend
node server.js
```

服务器将在 http://localhost:18081 启动

### 2. 启动前端开发服务器

在另一个终端窗口中运行：

```bash
cd d:\3learn\3GameEngine\UhandEngine\webui
npm run dev
```

前端将在 http://localhost:13000 启动

## 测试步骤

### 测试 1: 健康检查

1. 打开浏览器访问 http://localhost:18081/api/health
2. 应该看到类似以下响应：
```json
{
  "success": true,
  "status": "ok",
  "timestamp": "2024-01-15T10:00:00.000Z"
}
```

### 测试 2: 项目结构加载

1. 打开浏览器访问 http://localhost:13000
2. 点击左侧的 Project Browser 图标
3. 应该看到项目文件树加载
4. 如果后端未启动，会显示错误信息

### 测试 3: 文件内容加载

1. 在 Project Browser 中点击任意文件
2. 右侧应该显示文件内容
3. 检查控制台是否有错误

### 测试 4: 引擎信息查询

1. 访问 http://localhost:18081/api/engine/info
2. 应该看到引擎信息：
```json
{
  "success": true,
  "info": {
    "name": "UhandEngine",
    "version": "0.1.0",
    "description": "AI-first 2D game engine built with C and SDL2",
    "systems": [...],
    "examples": [...]
  }
}
```

## 常见问题

### 问题 1: 前端无法连接后端

**症状**: 前端显示 "Failed to load project structure"

**解决方案**:
1. 确认后端服务器正在运行
2. 检查端口是否正确（后端 18081，前端 13000）
3. 检查浏览器控制台的错误信息
4. 确认 vite.config.ts 中的代理配置正确

### 问题 2: CORS 错误

**症状**: 浏览器控制台显示 CORS 错误

**解决方案**:
1. 确认后端使用了 cors 中间件
2. 确认代理配置正确
3. 尝试直接访问后端 API 测试

### 问题 3: 端口被占用

**症状**: 启动后端时显示 "EADDRINUSE"

**解决方案**:
1. 修改 backend/server.js 中的 PORT
2. 同时修改 webui/vite.config.ts 中的代理目标端口
3. 或者关闭占用端口的程序

## 性能测试

### 测试大文件加载

尝试加载较大的源文件，检查：
- 加载时间
- 渲染性能
- 内存使用

### 测试并发请求

快速点击多个文件，检查：
- 是否所有请求都成功
- 是否有请求失败
- UI 响应是否流畅

## 下一步

联调测试通过后，可以继续：
1. 集成 AI API（OpenAI/Claude）
2. 实现场景编辑器
3. 添加更多 UI 组件
4. 实现文件保存功能
5. 添加编译和运行功能
