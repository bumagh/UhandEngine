#!/usr/bin/env python3
"""
UhandEngine AI Client
调用 Claude API 生成 Scene Schema
"""

import os
import sys
import json
import argparse
from typing import Dict, Any

try:
    import requests
except ImportError:
    print("Error: requests library not installed. Run: pip install requests")
    sys.exit(1)


def load_scene_schema_template() -> str:
    """加载 Scene Schema 模板作为 Prompt 参考"""
    schema_path = os.path.join(os.path.dirname(__file__), "../doc/scene-schema.md")
    if os.path.exists(schema_path):
        with open(schema_path, 'r', encoding='utf-8') as f:
            return f.read()
    return ""


def generate_scene_schema(user_prompt: str, api_key: str, api_url: str, model: str) -> Dict[str, Any]:
    """
    调用 Claude API 生成 Scene Schema

    Args:
        user_prompt: 用户自然语言描述
        api_key: Claude API Key
        api_url: Claude API 端点
        model: Claude 模型名称

    Returns:
        Scene Schema JSON
    """
    # 加载 Scene Schema 模板
    schema_template = load_scene_schema_template()

    # 构建 Prompt
    system_prompt = """你是一个游戏引擎的 AI 助手，负责将用户的自然语言描述转换为 Scene Schema JSON 格式。

请严格遵循 Scene Schema 的结构，输出有效的 JSON。

Scene Schema 结构参考：
```json
{
  "scene": {
    "name": "场景名称",
    "gameObjects": [
      {
        "name": "对象名称",
        "id": "唯一ID",
        "components": [
          {
            "type": "组件类型",
            "properties": {
              "属性名": "属性值"
            }
          }
        ]
      }
    ]
  }
}
```

只输出 JSON，不要包含其他文字。"""

    if schema_template:
        system_prompt += f"\n\n详细的 Scene Schema 定义：\n{schema_template}"

    # 构建请求
    headers = {
        "x-api-key": api_key,
        "Content-Type": "application/json",
        "anthropic-version": "2023-06-01"
    }

    payload = {
        "model": model,
        "max_tokens": 4096,
        "system": system_prompt,
        "messages": [
            {
                "role": "user",
                "content": user_prompt
            }
        ]
    }

    try:
        response = requests.post(api_url, headers=headers, json=payload, timeout=30)
        response.raise_for_status()

        result = response.json()

        # 提取 AI 回复
        if "content" in result and len(result["content"]) > 0:
            ai_response = result["content"][0]["text"]

            # 尝试解析 JSON
            try:
                # 清理可能的 markdown 代码块标记
                ai_response = ai_response.strip()
                if ai_response.startswith("```json"):
                    ai_response = ai_response[7:]
                if ai_response.startswith("```"):
                    ai_response = ai_response[3:]
                if ai_response.endswith("```"):
                    ai_response = ai_response[:-3]
                ai_response = ai_response.strip()

                scene_schema = json.loads(ai_response)
                return scene_schema
            except json.JSONDecodeError as e:
                print(f"Error: Failed to parse AI response as JSON: {e}")
                print(f"Raw response: {ai_response}")
                return {"error": "Failed to parse JSON", "raw_response": ai_response}
        else:
            print(f"Error: Unexpected API response format: {result}")
            return {"error": "Unexpected API response format", "response": result}

    except requests.exceptions.RequestException as e:
        print(f"Error: API request failed: {e}")
        return {"error": str(e)}


def main():
    parser = argparse.ArgumentParser(description="UhandEngine AI Client - Generate Scene Schema")
    parser.add_argument("prompt", help="自然语言描述的场景")
    parser.add_argument("--output", "-o", help="输出文件路径", default=None)
    parser.add_argument("--api-key", help="Claude API Key (或通过环境变量 CLAUDE_API_KEY)")
    parser.add_argument("--api-url", help="Claude API 端点 (或通过环境变量 CLAUDE_API_URL)")
    parser.add_argument("--model", help="Claude 模型名称 (或通过环境变量 CLAUDE_MODEL)")

    args = parser.parse_args()

    # 从环境变量或命令行参数获取配置
    api_key = args.api_key or os.environ.get("CLAUDE_API_KEY")
    api_url = args.api_url or os.environ.get("CLAUDE_API_URL", "https://api.anthropic.com/v1/messages")
    model = args.model or os.environ.get("CLAUDE_MODEL", "claude-3-5-sonnet-20241022")

    if not api_key:
        print("Error: CLAUDE_API_KEY not set. Please set it as environment variable or use --api-key")
        sys.exit(1)

    print(f"Generating Scene Schema for: {args.prompt}")
    print(f"Using model: {model}")
    print(f"API URL: {api_url}")
    print("Calling Claude API...")

    # 生成 Scene Schema
    scene_schema = generate_scene_schema(args.prompt, api_key, api_url, model)

    # 输出结果
    if args.output:
        with open(args.output, 'w', encoding='utf-8') as f:
            json.dump(scene_schema, f, indent=2, ensure_ascii=False)
        print(f"Scene Schema saved to: {args.output}")
    else:
        print("\nScene Schema:")
        print(json.dumps(scene_schema, indent=2, ensure_ascii=False))


if __name__ == "__main__":
    main()
