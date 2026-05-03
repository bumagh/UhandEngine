# AI 辅助设计工作流方案

## 文档目标

定义如何接入类似蓝湖的AI辅助设计工作流，包括AI生成原型图、AI自动切图、AI直接生成可用素材的完整流程。

---

## 工作流概述

```text
需求描述 → AI生成原型图 → AI切图/生成素材 → 资源处理 → 引擎集成
   ↓           ↓              ↓              ↓          ↓
文本提示词   图像生成      切图/素材生成    命名/打包   Scene Editor
```

---

## 1. AI 生成原型图

### 1.1 推荐的AI图像生成工具

| 工具 | 特点 | 适用场景 |
|------|------|----------|
| **DALL-E 3** | 高质量、理解能力强 | UI设计、图标生成 |
| **Midjourney** | 艺术性强、风格多样 | 背景、特效、角色 |
| **Stable Diffusion** | 开源、可控性强 | 批量生成、风格一致 |
| **Adobe Firefly** | Adobe生态集成 | 商业用途、版权友好 |

### 1.2 AI生成原型图的提示词工程

#### UI界面提示词模板

```
游戏UI界面设计，{游戏类型}风格，{具体功能}
- 风格：{视觉风格}，{配色方案}
- 布局：{布局方式}，{元素排列}
- 元素：{具体UI元素}，{交互状态}
- 尺寸：1920x1080，高清
- 格式：PNG，透明背景
```

**示例：**

```
游戏UI界面设计，科幻RPG风格，主菜单界面
- 风格：赛博朋克，霓虹配色，深色背景
- 布局：居中布局，垂直排列按钮
- 元素：标题栏、开始按钮、设置按钮、退出按钮，悬停高亮效果
- 尺寸：1920x1080，高清
- 格式：PNG，透明背景
```

#### 图标提示词模板

```
游戏图标设计，{图标类型}，{使用场景}
- 风格：{视觉风格}，{线条粗细}
- 尺寸：64x64，矢量风格
- 颜色：{配色方案}
- 背景：透明
```

**示例：**

```
游戏图标设计，设置图标，主菜单按钮
- 风格：扁平化，简洁线条
- 尺寸：64x64，矢量风格
- 颜色：白色，深色背景对比
- 背景：透明
```

#### 角色提示词模板

```
游戏角色设计，{角色类型}，{游戏类型}
- 风格：{美术风格}，{视角}
- 姿态：{动作姿态}
- 服装：{服装风格}
- 配色：{主要颜色}
- 尺寸：512x512，透明背景
```

**示例：**

```
游戏角色设计，战士角色，动作RPG游戏
- 风格：二次元，正面视角
- 姿态：站立待机，双手持剑
- 服装：轻甲，蓝色调
- 配色：蓝白配色
- 尺寸：512x512，透明背景
```

### 1.3 AI生成原型图的集成方案

#### 方案A：直接API调用

```python
# scripts/ai_generate_ui.py
import openai
from PIL import Image
import requests

def generate_ui_prompt(description: str, style: str) -> str:
    """生成UI提示词"""
    return f"""
游戏UI界面设计，{description}
- 风格：{style}
- 尺寸：1920x1080
- 格式：PNG，透明背景
- 专业游戏UI设计，高分辨率
"""

def generate_ui_image(prompt: str, output_path: str):
    """使用DALL-E 3生成UI图像"""
    client = openai.OpenAI()
    
    response = client.images.generate(
        model="dall-e-3",
        prompt=prompt,
        size="1792x1024",
        quality="hd",
        n=1,
    )
    
    # 下载图像
    image_url = response.data[0].url
    image_data = requests.get(image_url).content
    
    # 保存图像
    with open(output_path, 'wb') as f:
        f.write(image_data)
    
    print(f"UI图像已生成: {output_path}")

# 使用示例
prompt = generate_ui_prompt("主菜单界面", "科幻风格")
generate_ui_image(prompt, "output/main_menu_ui.png")
```

#### 方案B：使用Stable Diffusion本地部署

```python
# scripts/ai_generate_ui_sd.py
from diffusers import StableDiffusionControlNetPipeline
import torch

def generate_ui_with_controlnet(sketch_path: str, prompt: str, output_path: str):
    """使用ControlNet基于草图生成UI"""
    pipe = StableDiffusionControlNetPipeline.from_pretrained(
        "runwayml/stable-diffusion-v1-5",
        controlnet="lllyasviel/sd-controlnet-canny"
    ).to("cuda")
    
    # 加载草图
    sketch = Image.open(sketch_path).convert("RGB")
    
    # 生成图像
    image = pipe(prompt, image=sketch, num_inference_steps=20).images[0]
    
    # 保存
    image.save(output_path)
    print(f"UI图像已生成: {output_path}")
```

#### 方案C：使用Midjourney API

```python
# scripts/ai_generate_ui_mj.py
import requests
import json

def generate_ui_midjourney(prompt: str, discord_webhook: str):
    """使用Midjourney生成UI"""
    payload = {
        "content": f"/imagine prompt: {prompt} --ar 16:9 --v 5"
    }
    
    response = requests.post(discord_webhook, json=payload)
    print(f"Midjourney请求已发送")
```

---

## 2. AI 自动切图

### 2.1 AI切图技术方案

#### 方案A：使用计算机视觉自动检测UI元素

```python
# scripts/ai_auto_slice.py
import cv2
import numpy as np
from PIL import Image
import json

def detect_ui_elements(image_path: str):
    """检测图像中的UI元素"""
    image = cv2.imread(image_path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    # 边缘检测
    edges = cv2.Canny(gray, 50, 150)
    
    # 查找轮廓
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    elements = []
    for contour in contours:
        x, y, w, h = cv2.boundingRect(contour)
        
        # 过滤太小的元素
        if w > 20 and h > 20:
            elements.append({
                'x': x,
                'y': y,
                'width': w,
                'height': h,
                'type': detect_element_type(image[y:y+h, x:x+w])
            })
    
    return elements

def detect_element_type(element_image):
    """检测元素类型"""
    # 简单的元素类型检测
    aspect_ratio = element_image.shape[1] / element_image.shape[0]
    
    if aspect_ratio > 3:
        return 'button'
    elif aspect_ratio < 0.3:
        return 'icon'
    else:
        return 'panel'

def slice_ui_elements(image_path: str, output_dir: str):
    """切分UI元素"""
    elements = detect_ui_elements(image_path)
    image = Image.open(image_path)
    
    for i, element in enumerate(elements):
        # 切取元素
        element_image = image.crop((
            element['x'],
            element['y'],
            element['x'] + element['width'],
            element['y'] + element['height']
        ))
        
        # 生成文件名
        filename = f"ui_element_{i:03d}_{element['type']}.png"
        output_path = f"{output_dir}/{filename}"
        
        # 保存
        element_image.save(output_path)
        print(f"已切取: {filename}")
    
    # 保存元素清单
    with open(f"{output_dir}/elements.json", 'w') as f:
        json.dump(elements, f, indent=2)
```

#### 方案B：使用AI模型检测UI元素

```python
# scripts/ai_ui_detection.py
import torch
from transformers import DetrImageProcessor, DetrForObjectDetection
from PIL import Image

def detect_ui_elements_with_ai(image_path: str):
    """使用AI模型检测UI元素"""
    # 加载预训练模型
    processor = DetrImageProcessor.from_pretrained("facebook/detr-resnet-50")
    model = DetrForObjectDetection.from_pretrained("facebook/detr-resnet-50")
    
    # 加载图像
    image = Image.open(image_path)
    inputs = processor(images=image, return_tensors="pt")
    
    # 检测
    outputs = model(**inputs)
    
    # 解析结果
    target_sizes = torch.tensor([image.size[::-1]])
    results = processor.post_process_object_detection(outputs, target_sizes=target_sizes, threshold=0.9)[0]
    
    elements = []
    for score, label, box in zip(results["scores"], results["labels"], results["boxes"]):
        elements.append({
            'type': model.config.id2label[label.item()],
            'confidence': score.item(),
            'box': box.tolist()
        })
    
    return elements
```

#### 方案C：使用专门UI检测模型

```python
# scripts/ai_ui_segmentation.py
import cv2
import numpy as np
from segment_anything import sam_model_registry, SamAutomaticMaskGenerator

def segment_ui_elements(image_path: str):
    """使用SAM模型分割UI元素"""
    # 加载SAM模型
    sam = sam_model_registry["vit_h"](checkpoint="sam_vit_h_4b8939.pth")
    mask_generator = SamAutomaticMaskGenerator(sam)
    
    # 加载图像
    image = cv2.imread(image_path)
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    
    # 生成掩码
    masks = mask_generator.generate(image_rgb)
    
    elements = []
    for mask in masks:
        elements.append({
            'segmentation': mask['segmentation'],
            'bbox': mask['bbox'],
            'area': mask['area']
        })
    
    return elements
```

### 2.2 AI智能命名

```python
# scripts/ai_smart_naming.py
import openai
from PIL import Image
import base64

def encode_image(image_path: str) -> str:
    """编码图像为base64"""
    with open(image_path, "rb") as image_file:
        return base64.b64encode(image_file.read()).decode('utf-8')

def generate_element_name(image_path: str, context: str) -> str:
    """使用AI生成元素名称"""
    client = openai.OpenAI()
    
    base64_image = encode_image(image_path)
    
    response = client.chat.completions.create(
        model="gpt-4-vision-preview",
        messages=[
            {
                "role": "user",
                "content": [
                    {
                        "type": "text",
                        "text": f"""
这是一个游戏UI元素，请根据其外观和功能描述一个合适的名称。

上下文：{context}

命名规范：
- 小写字母
- 使用下划线分隔
- 语义化命名
- 格式：{类型}_{功能}_{状态}

请只返回名称，不要其他文字。
"""
                    },
                    {
                        "type": "image_url",
                        "image_url": {
                            "url": f"data:image/png;base64,{base64_image}"
                        }
                    }
                ]
            }
        ],
        max_tokens=50
    )
    
    return response.choices[0].message.content.strip()

# 使用示例
name = generate_element_name("ui_element_001_button.png", "主菜单界面")
print(f"AI生成的名称: {name}")
```

---

## 3. AI 直接生成可用素材

### 3.1 AI生成Sprite Sheet

```python
# scripts/ai_generate_spritesheet.py
import openai
from PIL import Image, ImageDraw
import json

def generate_animation_frames(character_description: str, action: str, frame_count: int):
    """生成动画帧序列"""
    client = openai.OpenAI()
    
    frames = []
    for i in range(frame_count):
        prompt = f"""
游戏角色设计，{character_description}
动作：{action}
帧序：第{i+1}帧，共{frame_count}帧
姿态：{get_pose_for_frame(i, frame_count, action)}
尺寸：64x64
背景：透明
风格：像素艺术，游戏角色
"""
        
        response = client.images.generate(
            model="dall-e-3",
            prompt=prompt,
            size="1024x1024",
            quality="hd",
            n=1,
        )
        
        # 下载并处理图像
        image_url = response.data[0].url
        # ... 下载并调整为64x64
        frames.append(processed_image)
    
    return frames

def create_spritesheet(frames, output_path: str):
    """创建Sprite Sheet"""
    # 计算Sprite Sheet尺寸
    cols = 8
    rows = (len(frames) + cols - 1) // cols
    
    sheet_width = cols * 64
    sheet_height = rows * 64
    
    # 创建空白Sprite Sheet
    sheet = Image.new('RGBA', (sheet_width, sheet_height), (0, 0, 0, 0))
    
    # 将帧放置到Sprite Sheet
    for i, frame in enumerate(frames):
        x = (i % cols) * 64
        y = (i // cols) * 64
        sheet.paste(frame, (x, y))
    
    # 保存Sprite Sheet
    sheet.save(output_path)
    
    # 生成JSON描述
    json_data = {
        "frames": []
    }
    
    for i in range(len(frames)):
        x = (i % cols) * 64
        y = (i // cols) * 64
        json_data["frames"].append({
            "filename": f"frame_{i:03d}.png",
            "frame": {"x": x, "y": y, "w": 64, "h": 64},
            "rotated": false,
            "trimmed": false,
            "spriteSourceSize": {"x": 0, "y": 0, "w": 64, "h": 64},
            "sourceSize": {"w": 64, "h": 64}
        })
    
    json_path = output_path.replace('.png', '.json')
    with open(json_path, 'w') as f:
        json.dump(json_data, f, indent=2)
```

### 3.2 AI生成九宫格资源

```python
# scripts/ai_generate_nineslice.py
import openai
from PIL import Image, ImageDraw
import math

def generate_nineslice(description: str, size: tuple = (400, 300)):
    """生成九宫格资源"""
    client = openai.OpenAI()
    
    prompt = f"""
游戏UI面板设计，{description}
- 风格：适合九宫格拉伸
- 边框：明显的边框区域，便于切片
- 内容：中心区域可拉伸
- 尺寸：{size[0]}x{size[1]}
- 背景：透明
"""
    
    response = client.images.generate(
        model="dall-e-3",
        prompt=prompt,
        size="1024x1024",
        quality="hd",
        n=1,
    )
    
    # 下载图像
    image_url = response.data[0].url
    image = download_and_resize(image_url, size)
    
    # 自动检测九宫格边界
    border_size = detect_border_size(image)
    
    # 切片
    slices = slice_nineslice(image, border_size)
    
    return slices, border_size

def detect_border_size(image):
    """检测边框大小"""
    # 简单的边框检测算法
    width, height = image.size
    
    # 检测左边框
    left_border = 0
    for x in range(width):
        if is_border_pixel(image, x, height//2):
            left_border = x
        else:
            break
    
    # 检测右边框
    right_border = 0
    for x in range(width-1, -1, -1):
        if is_border_pixel(image, x, height//2):
            right_border = width - 1 - x
        else:
            break
    
    # 检测上边框
    top_border = 0
    for y in range(height):
        if is_border_pixel(image, width//2, y):
            top_border = y
        else:
            break
    
    # 检测下边框
    bottom_border = 0
    for y in range(height-1, -1, -1):
        if is_border_pixel(image, width//2, y):
            bottom_border = height - 1 - y
        else:
            break
    
    return {
        'left': left_border,
        'right': right_border,
        'top': top_border,
        'bottom': bottom_border
    }

def slice_nineslice(image, border_size):
    """切片九宫格"""
    width, height = image.size
    slices = {}
    
    # 切片位置
    positions = {
        'tl': (0, 0),
        'tm': (border_size['left'], 0),
        'tr': (width - border_size['right'], 0),
        'ml': (0, border_size['top']),
        'mm': (border_size['left'], border_size['top']),
        'mr': (width - border_size['right'], border_size['top']),
        'bl': (0, height - border_size['bottom']),
        'bm': (border_size['left'], height - border_size['bottom']),
        'br': (width - border_size['right'], height - border_size['bottom'])
    }
    
    # 切片尺寸
    sizes = {
        'tl': (border_size['left'], border_size['top']),
        'tm': (width - border_size['left'] - border_size['right'], border_size['top']),
        'tr': (border_size['right'], border_size['top']),
        'ml': (border_size['left'], height - border_size['top'] - border_size['bottom']),
        'mm': (width - border_size['left'] - border_size['right'], height - border_size['top'] - border_size['bottom']),
        'mr': (border_size['right'], height - border_size['top'] - border_size['bottom']),
        'bl': (border_size['left'], border_size['bottom']),
        'bm': (width - border_size['left'] - border_size['right'], border_size['bottom']),
        'br': (border_size['right'], border_size['bottom'])
    }
    
    # 执行切片
    for key, pos in positions.items():
        box = (pos[0], pos[1], pos[0] + sizes[key][0], pos[1] + sizes[key][1])
        slices[key] = image.crop(box)
    
    return slices
```

### 3.3 AI生成图标集

```python
# scripts/ai_generate_iconset.py
import openai
from PIL import Image

def generate_icon_set(icon_descriptions: list, style: str):
    """生成图标集"""
    client = openai.OpenAI()
    
    icons = {}
    for description in icon_descriptions:
        prompt = f"""
游戏图标设计，{description}
- 风格：{style}
- 尺寸：64x64
- 背景：透明
- 简洁、清晰、易识别
"""
        
        response = client.images.generate(
            model="dall-e-3",
            prompt=prompt,
            size="1024x1024",
            quality="hd",
            n=1,
        )
        
        # 下载并处理
        image_url = response.data[0].url
        icon = download_and_resize(image_url, (64, 64))
        
        # 生成名称
        name = generate_icon_name(description)
        icons[name] = icon
    
    return icons

def generate_icon_name(description: str) -> str:
    """生成图标名称"""
    # 简单的名称生成逻辑
    keywords = description.split()
    return f"icon_{'_'.join(keywords[:2])}.png"
```

---

## 4. 完整工作流集成

### 4.1 一键生成完整UI方案

```python
# scripts/ai_generate_complete_ui.py
import os
import json
from ai_generate_ui import generate_ui_image
from ai_auto_slice import slice_ui_elements
from ai_smart_naming import generate_element_name
from ai_generate_nineslice import generate_nineslice

def generate_complete_ui(description: str, style: str, output_dir: str):
    """一键生成完整UI方案"""
    
    # 1. 创建输出目录
    os.makedirs(output_dir, exist_ok=True)
    os.makedirs(f"{output_dir}/images/ui", exist_ok=True)
    
    # 2. 生成UI原型图
    print("正在生成UI原型图...")
    prompt = f"游戏UI界面设计，{description}，风格：{style}"
    ui_image_path = f"{output_dir}/images/ui/main_ui.png"
    generate_ui_image(prompt, ui_image_path)
    
    # 3. 自动切图
    print("正在自动切图...")
    slice_output_dir = f"{output_dir}/images/ui/sliced"
    os.makedirs(slice_output_dir, exist_ok=True)
    elements = slice_ui_elements(ui_image_path, slice_output_dir)
    
    # 4. AI智能命名
    print("正在AI智能命名...")
    renamed_elements = []
    for element in elements:
        old_name = element['filename']
        new_name = generate_element_name(
            f"{slice_output_dir}/{old_name}",
            description
        )
        
        # 重命名文件
        old_path = f"{slice_output_dir}/{old_name}"
        new_path = f"{slice_output_dir}/{new_name}"
        os.rename(old_path, new_path)
        
        renamed_elements.append({
            'old_name': old_name,
            'new_name': new_name,
            'type': element['type']
        })
    
    # 5. 生成资源清单
    print("正在生成资源清单...")
    asset_list = {
        "version": "1.0",
        "generated_by": "AI",
        "ui_image": ui_image_path,
        "elements": renamed_elements
    }
    
    with open(f"{output_dir}/assets.json", 'w') as f:
        json.dump(asset_list, f, indent=2)
    
    print(f"完整UI方案已生成到: {output_dir}")
    print(f"- UI原型图: {ui_image_path}")
    print(f"- 切图元素: {len(elements)}个")
    print(f"- 资源清单: {output_dir}/assets.json")

# 使用示例
generate_complete_ui(
    description="科幻RPG游戏主菜单",
    style="赛博朋克风格，霓虹配色",
    output_dir="output/scifi_menu_ui"
)
```

### 4.2 与WebUI场景编辑器集成

```typescript
// webui/src/components/AIAssetGenerator.tsx
import React, { useState } from 'react'
import { Wand2, Download, RefreshCw } from 'lucide-react'

interface AIAssetGeneratorProps {
  onAssetsGenerated: (assets: Asset[]) => void
}

export default function AIAssetGenerator({ onAssetsGenerated }: AIAssetGeneratorProps) {
  const [prompt, setPrompt] = useState('')
  const [style, setStyle] = useState('')
  const [isGenerating, setIsGenerating] = useState(false)
  const [progress, setProgress] = useState(0)

  const handleGenerate = async () => {
    setIsGenerating(true)
    setProgress(0)

    try {
      // 调用后端API生成UI
      const response = await fetch('/api/ai/generate-ui', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ prompt, style })
      })

      const data = await response.json()
      
      if (data.success) {
        setProgress(100)
        onAssetsGenerated(data.assets)
      }
    } catch (error) {
      console.error('AI生成失败:', error)
    } finally {
      setIsGenerating(false)
    }
  }

  return (
    <div className="bg-gray-800 rounded-lg p-6">
      <h3 className="text-lg font-semibold mb-4 flex items-center gap-2">
        <Wand2 className="w-5 h-5" />
        AI 资源生成器
      </h3>

      <div className="space-y-4">
        <div>
          <label className="block text-sm font-medium mb-2">UI描述</label>
          <textarea
            value={prompt}
            onChange={(e) => setPrompt(e.target.value)}
            placeholder="例如：科幻RPG游戏主菜单，包含开始、设置、退出按钮"
            className="w-full bg-gray-700 rounded px-3 py-2 text-sm"
            rows={3}
          />
        </div>

        <div>
          <label className="block text-sm font-medium mb-2">视觉风格</label>
          <input
            type="text"
            value={style}
            onChange={(e) => setStyle(e.target.value)}
            placeholder="例如：赛博朋克风格，霓虹配色"
            className="w-full bg-gray-700 rounded px-3 py-2 text-sm"
          />
        </div>

        {isGenerating && (
          <div className="space-y-2">
            <div className="flex items-center gap-2">
              <RefreshCw className="w-4 h-4 animate-spin" />
              <span className="text-sm">正在生成...</span>
            </div>
            <div className="w-full bg-gray-700 rounded-full h-2">
              <div
                className="bg-blue-600 h-2 rounded-full transition-all"
                style={{ width: `${progress}%` }}
              />
            </div>
          </div>
        )}

        <button
          onClick={handleGenerate}
          disabled={isGenerating || !prompt}
          className="w-full bg-blue-600 hover:bg-blue-700 disabled:bg-gray-600 disabled:cursor-not-allowed px-4 py-2 rounded flex items-center justify-center gap-2"
        >
          <Wand2 className="w-4 h-4" />
          生成UI资源
        </button>
      </div>
    </div>
  )
}
```

### 4.3 后端API实现

```javascript
// webui/backend/server.js
const { spawn } = require('child_process');
const path = require('path');

// AI生成UI API
app.post('/api/ai/generate-ui', async (req, res) => {
  try {
    const { prompt, style } = req.body;
    
    // 调用Python脚本
    const scriptPath = path.join(__dirname, '../../scripts/ai_generate_complete_ui.py');
    const outputDir = path.join(__dirname, '../../temp/ai_generated');
    
    const pythonProcess = spawn('python', [
      scriptPath,
      '--description', prompt,
      '--style', style,
      '--output', outputDir
    ]);

    pythonProcess.stdout.on('data', (data) => {
      console.log(`Python输出: ${data}`);
    });

    pythonProcess.on('close', (code) => {
      if (code === 0) {
        // 读取生成的资源清单
        const assetsPath = path.join(outputDir, 'assets.json');
        const assets = JSON.parse(fs.readFileSync(assetsPath, 'utf8'));
        
        res.json({
          success: true,
          assets: assets
        });
      } else {
        res.json({
          success: false,
          error: 'AI生成失败'
        });
      }
    });
  } catch (error) {
    console.error('AI生成UI错误:', error);
    res.json({
      success: false,
      error: error.message
    });
  }
});
```

---

## 5. 蓝湖风格集成方案

### 5.1 设计稿上传与解析

```python
# scripts/lanhu_integration.py
import requests
import json

class LanhuIntegration:
    """蓝湖API集成"""
    
    def __init__(self, api_key: str):
        self.api_key = api_key
        self.base_url = "https://api.lanhuapp.com"
    
    def upload_design(self, file_path: str):
        """上传设计稿"""
        url = f"{self.base_url}/api/v1/design/upload"
        
        with open(file_path, 'rb') as f:
            files = {'file': f}
            headers = {'Authorization': f'Bearer {self.api_key}'}
            
            response = requests.post(url, files=files, headers=headers)
            return response.json()
    
    def get_design_assets(self, design_id: str):
        """获取设计稿资源"""
        url = f"{self.base_url}/api/v1/design/{design_id}/assets"
        headers = {'Authorization': f'Bearer {self.api_key}'}
        
        response = requests.get(url, headers=headers)
        return response.json()
    
    def auto_slice_design(self, design_id: str):
        """自动切图"""
        url = f"{self.base_url}/api/v1/design/{design_id}/auto-slice"
        headers = {'Authorization': f'Bearer {self.api_key}'}
        
        response = requests.post(url, headers=headers)
        return response.json()
```

### 5.2 AI增强的蓝湖工作流

```python
# scripts/ai_enhanced_lanhu.py
from lanhu_integration import LanhuIntegration
from ai_smart_naming import generate_element_name
from ai_generate_spritesheet import generate_animation_frames

class AIEnhancedLanhu:
    """AI增强的蓝湖工作流"""
    
    def __init__(self, lanhu_api_key: str):
        self.lanhu = LanhuIntegration(lanhu_api_key)
    
    def process_design_with_ai(self, design_file: str):
        """使用AI处理设计稿"""
        
        # 1. 上传到蓝湖
        print("上传设计稿到蓝湖...")
        upload_result = self.lanhu.upload_design(design_file)
        design_id = upload_result['design_id']
        
        # 2. 蓝湖自动切图
        print("蓝湖自动切图...")
        slice_result = self.lanhu.auto_slice_design(design_id)
        assets = slice_result['assets']
        
        # 3. AI智能命名
        print("AI智能命名...")
        for asset in assets:
            old_name = asset['name']
            new_name = generate_element_name(asset['path'], asset['context'])
            asset['ai_name'] = new_name
        
        # 4. 生成动画资源（如果需要）
        print("生成动画资源...")
        for asset in assets:
            if asset['type'] == 'character':
                frames = generate_animation_frames(
                    asset['description'],
                    asset['action'],
                    8
                )
                asset['animation_frames'] = frames
        
        return {
            'design_id': design_id,
            'assets': assets
        }
```

---

## 6. 成本与性能优化

### 6.1 成本控制

```python
# scripts/cost_control.py
import openai

class CostControlledAIGenerator:
    """成本控制的AI生成器"""
    
    def __init__(self, max_daily_budget: float = 10.0):
        self.max_daily_budget = max_daily_budget
        self.daily_spent = 0.0
        self.cost_per_image = 0.04  # DALL-E 3 HD价格
    
    def can_generate(self, image_count: int = 1) -> bool:
        """检查是否可以生成"""
        estimated_cost = image_count * self.cost_per_image
        return (self.daily_spent + estimated_cost) <= self.max_daily_budget
    
    def generate_with_cost_check(self, prompt: str):
        """带成本检查的生成"""
        if not self.can_generate():
            raise Exception("超过每日预算限制")
        
        # 生成图像
        client = openai.OpenAI()
        response = client.images.generate(
            model="dall-e-3",
            prompt=prompt,
            size="1024x1024",
            quality="hd",
            n=1,
        )
        
        # 更新花费
        self.daily_spent += self.cost_per_image
        
        return response
```

### 6.2 缓存机制

```python
# scripts/ai_cache.py
import hashlib
import json
import os
from functools import lru_cache

class AICache:
    """AI生成结果缓存"""
    
    def __init__(self, cache_dir: str):
        self.cache_dir = cache_dir
        os.makedirs(cache_dir, exist_ok=True)
    
    def get_cache_key(self, prompt: str, params: dict) -> str:
        """生成缓存键"""
        data = {'prompt': prompt, 'params': params}
        json_str = json.dumps(data, sort_keys=True)
        return hashlib.md5(json_str.encode()).hexdigest()
    
    def get(self, prompt: str, params: dict) -> any:
        """获取缓存"""
        key = self.get_cache_key(prompt, params)
        cache_file = f"{self.cache_dir}/{key}.json"
        
        if os.path.exists(cache_file):
            with open(cache_file, 'r') as f:
                return json.load(f)
        
        return None
    
    def set(self, prompt: str, params: dict, result: any):
        """设置缓存"""
        key = self.get_cache_key(prompt, params)
        cache_file = f"{self.cache_dir}/{key}.json"
        
        with open(cache_file, 'w') as f:
            json.dump(result, f)
```

---

## 7. 质量控制

### 7.1 AI生成结果验证

```python
# scripts/quality_control.py
from PIL import Image
import cv2

class QualityControl:
    """质量控制"""
    
    @staticmethod
    def check_image_quality(image_path: str) -> dict:
        """检查图像质量"""
        image = Image.open(image_path)
        
        checks = {
            'resolution': image.size,
            'has_transparency': image.mode in ('RGBA', 'LA'),
            'file_size': os.path.getsize(image_path),
            'aspect_ratio': image.size[0] / image.size[1]
        }
        
        # 检查是否模糊
        gray = cv2.cvtColor(cv2.imread(image_path), cv2.COLOR_BGR2GRAY)
        variance = cv2.Laplacian(gray, cv2.CV_64F).var()
        checks['sharpness'] = variance > 100  # 阈值可根据需要调整
        
        return checks
    
    @staticmethod
    def validate_asset(asset: dict) -> bool:
        """验证资源是否符合规范"""
        # 检查命名规范
        if not asset['name'].islower():
            return False
        
        # 检查尺寸
        if asset['width'] > 4096 or asset['height'] > 4096:
            return False
        
        # 检查格式
        if not asset['path'].endswith(('.png', '.svg')):
            return False
        
        return True
```

---

## 8. 总结

本方案定义了完整的AI辅助设计工作流，包括：

1. **AI生成原型图**：使用DALL-E 3、Midjourney、Stable Diffusion等工具
2. **AI自动切图**：使用计算机视觉或AI模型检测UI元素
3. **AI智能命名**：使用GPT-4 Vision生成语义化名称
4. **AI直接生成素材**：生成Sprite Sheet、九宫格、图标集
5. **完整工作流集成**：一键生成完整UI方案
6. **WebUI集成**：在场景编辑器中集成AI生成功能
7. **蓝湖风格集成**：与蓝湖API集成
8. **成本与性能优化**：成本控制和缓存机制
9. **质量控制**：生成结果验证

通过此工作流，可以实现：
- 从文本描述直接生成UI原型
- 自动切分和命名UI元素
- 批量生成游戏素材
- 与现有引擎无缝集成
- 降低美术资源制作成本
- 提高开发效率
