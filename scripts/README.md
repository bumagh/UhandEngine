# UhandEngine AI Client

AI 客户端脚本，用于调用 Claude API 生成 Scene Schema。

## 安装依赖

```bash
pip install -r ../requirements.txt
```

## 配置

1. 复制环境变量示例文件：
```bash
cp ../.env.example ../.env
```

2. 编辑 `.env` 文件，填入你的 API Key：
```env
CLAUDE_API_KEY=your_claude_api_key_here
CLAUDE_API_URL=https://gaccode.com/claudecode
CLAUDE_MODEL=claude-sonnet-4-6
```

## 使用方法

### 基本用法

```bash
python ai-client.py "创建一个包含标题和按钮的主菜单场景"
```

### 保存到文件

```bash
python ai-client.py "创建一个包含标题和按钮的主菜单场景" -o output.json
```

### 使用命令行参数

```bash
python ai-client.py "创建一个包含标题和按钮的主菜单场景" --api-key your_key --api-url https://gaccode.com/claudecode --model claude-sonnet-4-6
```

## 输出格式

脚本会输出符合 Scene Schema 定义的 JSON 格式，例如：

```json
{
  "scene": {
    "name": "主菜单场景",
    "gameObjects": [
      {
        "name": "标题文本",
        "id": "title_text",
        "components": [
          {
            "type": "UIComponent",
            "properties": {
              "text": "UhandEngine MVP",
              "x": 200,
              "y": 100
            }
          }
        ]
      }
    ]
  }
}
```

## 安全提示

- **不要**将 `.env` 文件提交到 Git 仓库
- **不要**在代码中硬编码 API Key
- 定期轮换 API Key
