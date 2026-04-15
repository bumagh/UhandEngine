# AI API 配置指南

## 概述

UhandEngine WebUI 集成了 AI 助手功能，支持通过 OpenAI API 进行智能对话和代码生成。

## 配置方式

### 方式一：通过 WebUI 配置（推荐）

**这是最简单的方式，无需编辑文件。**

1. 获取 OpenAI API 密钥
   - 访问 https://platform.openai.com/
   - 注册或登录账号
   - 进入 API Keys 页面
   - 创建新的 API 密钥
   - 复制密钥（格式：`sk-...`）

2. 启动 WebUI
   ```bash
   cd webui
   npm run dev
   ```
   访问 http://localhost:13000

3. 打开设置界面
   - 点击侧边栏的设置图标（齿轮）
   - 进入 AI Configuration 页面

4. 配置 AI
   - 选择 API Provider（OpenAI / Anthropic / Custom）
   - 输入 API Key
   - 选择或输入 Model（如 gpt-3.5-turbo, gpt-4）
   - 输入 Base URL（默认为 https://api.openai.com/v1）

5. 测试配置
   - 点击 "Test Configuration" 按钮
   - 如果成功，会显示 "Configuration test successful"

6. 保存配置
   - 点击 "Save Configuration" 按钮
   - 配置会保存到浏览器本地存储

7. 使用 AI
   - 返回 AI Assistant 页面
   - 开始与 AI 对话

### 方式二：通过环境变量配置

**适合服务器部署或需要持久化配置的场景。**

1. 获取 OpenAI API 密钥
   - 同上

2. 配置后端环境变量

在 `webui/backend/` 目录下创建 `.env` 文件：

```bash
cd webui/backend
```

创建 `.env` 文件：

```env
AI_PROVIDER=openai
OPENAI_API_KEY=sk-your-actual-api-key-here
OPENAI_MODEL=gpt-3.5-turbo
```

或者复制示例文件：

```bash
cp .env.example .env
```

然后编辑 `.env` 文件，填入你的 API 密钥。

3. 安装依赖

```bash
cd webui/backend
npm install
```

4. 重启后端服务器

停止当前的后端服务器（Ctrl+C），然后重新启动：

```bash
node server.js
```

应该看到：
```
OpenAI client initialized
UhandEngine WebUI Backend running on port 18081
```

5. 测试 AI 功能

1. 打开浏览器访问 http://localhost:13000
2. 点击 AI Assistant 图标
3. 应该看到没有错误提示
4. 输入消息测试 AI 对话

## 支持的模型

### OpenAI 模型

- **gpt-3.5-turbo**: 快速、经济，适合日常对话
- **gpt-4**: 更强大，适合复杂任务
- **gpt-4-turbo**: 最新版本，性能更好

在 `.env` 中设置：
```env
OPENAI_MODEL=gpt-4
```

## 功能说明

### 对话功能

- 自然语言对话
- 上下文感知
- 项目信息集成

### 代码生成

- 基于提示生成代码
- 考虑项目上下文
- 遵循引擎代码风格

### 代码分析

- 检测代码问题
- 提供改进建议
- 解释代码逻辑

## 成本控制

### OpenAI 定价（参考）

- gpt-3.5-turbo: 输入 $0.001/1K tokens，输出 $0.002/1K tokens
- gpt-4: 输入 $0.03/1K tokens，输出 $0.06/1K tokens

### 成本估算

假设每次对话使用 1000 tokens：
- gpt-3.5-turbo: 约 $0.003/次
- gpt-4: 约 $0.09/次

建议使用 gpt-3.5-turbo 进行日常对话，仅在需要时使用 gpt-4。

## 故障排除

### 问题：AI not configured

**原因**: `.env` 文件不存在或 API 密钥未设置

**解决**:
1. 确认 `webui/backend/.env` 文件存在
2. 确认 `OPENAI_API_KEY` 已设置
3. 重启后端服务器

### 问题：Failed to connect to AI service

**原因**: 网络问题或 API 密钥无效

**解决**:
1. 检查网络连接
2. 验证 API 密钥是否有效
3. 检查 OpenAI 账户余额

### 问题：AI response failed

**原因**: API 请求失败或超时

**解决**:
1. 检查后端日志
2. 验证 API 密钥权限
3. 尝试更换模型

## 安全建议

1. **不要提交 .env 文件到 Git**
   - `.env` 已在 `.gitignore` 中
   - 仅提交 `.env.example`

2. **定期轮换 API 密钥**
   - 在 OpenAI 平台定期更新密钥
   - 删除不再使用的密钥

3. **监控使用量**
   - 在 OpenAI 平台查看使用统计
   - 设置预算警告

4. **限制 API 密钥权限**
   - 在 OpenAI 平台设置密钥权限
   - 仅授予必要的权限

## 下一步

配置完成后，你可以：

1. 在 AI Assistant 中进行对话
2. 请求代码生成
3. 请求代码分析
4. 获取引擎系统解释

## 高级配置

### 使用 Claude API

如需使用 Anthropic Claude，修改 `.env`：

```env
AI_PROVIDER=anthropic
ANTHROPIC_API_KEY=sk-ant-your-key-here
ANTHROPIC_MODEL=claude-3-opus-20240229
```

注意：Claude 集成尚未完全实现，当前仅支持 OpenAI。

### 自定义系统提示词

修改 `webui/backend/ai-service.js` 中的 `buildSystemPrompt` 方法来自定义 AI 行为。
