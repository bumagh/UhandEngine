# UhandEngine 原型设计工作流方案

## 文档目标

定义从 UI 设计稿到游戏引擎资源的完整工作流，包括切图、资源管理、命名规范和自动化流程。

---

## 工作流概述

```text
设计稿 → 导出资源 → 资源处理 → 引擎集成 → 运行时使用
  ↓         ↓          ↓          ↓          ↓
Figma/Sketch  PNG/SVG  命名/打包  AssetManager  Scene Editor
```

---

## 1. 设计阶段

### 1.1 推荐设计工具

- **Figma**（推荐）：Web 原生、协作友好、支持插件
- **Sketch**：Mac 原生、插件生态丰富
- **Adobe XD**：Adobe 生态集成
- **Photoshop**：传统位图设计

### 1.2 设计规范

#### 尺寸规范
- **基础分辨率**：1920x1080（桌面）、1280x720（Web）
- **UI 基准**：16px 基准网格
- **安全区域**：考虑不同屏幕比例

#### 颜色规范
- **主色调**：定义品牌色
- **辅助色**：定义功能色
- **中性色**：定义灰度色阶
- **状态色**：定义成功/警告/错误色

#### 字体规范
- **标题字体**：大号粗体
- **正文字体**：中号常规
- **注释字体**：小号常规
- **数字字体**：等宽字体

---

## 2. 资源导出

### 2.1 导出格式

| 资源类型 | 推荐格式 | 说明 |
|---------|---------|------|
| 静态图片 | PNG (24-bit with alpha) | 支持透明，质量好 |
| 矢量图标 | SVG | 可缩放，文件小 |
| 动画序列 | PNG 序列或 Sprite Sheet | 便于控制帧序 |
| 字体文件 | TTF/OTF | 引擎已支持 |
| 音频文件 | WAV/OGG/MP3 | 引擎已支持 |

### 2.2 导出设置

#### PNG 导出
- **颜色深度**：24-bit with alpha
- **压缩**：无损或轻微压缩
- **尺寸**：2x 或 3x 高清版本

#### SVG 导出
- **优化**：移除冗余代码
- **兼容性**：确保引擎可解析
- **分组**：按功能分组导出

---

## 3. 资源命名规范

### 3.1 命名原则

- **小写**：全部使用小写字母
- **下划线**：使用下划线分隔单词
- **语义化**：名称应表达资源用途
- **前缀**：按类型添加前缀

### 3.2 命名格式

```
{类型}_{功能}_{状态}_{尺寸}.{扩展名}
```

### 3.3 示例

| 资源类型 | 命名示例 | 说明 |
|---------|---------|------|
| 按钮 | `btn_start_normal.png` | 开始按钮正常状态 |
| 按钮 | `btn_start_hover.png` | 开始按钮悬停状态 |
| 按钮 | `btn_start_pressed.png` | 开始按钮按下状态 |
| 图标 | `icon_settings.png` | 设置图标 |
| 背景 | `bg_main_menu.png` | 主菜单背景 |
| 角色 | `char_player_idle_01.png` | 玩家角色待机第1帧 |
| 角色 | `char_player_run_01.png` | 玩家角色跑动第1帧 |
| UI 面板 | `panel_dialog.png` | 对话框面板 |
| 九宫格 | `9_panel_bg.png` | 九宫格背景 |

### 3.4 特殊命名

#### 九宫格切片
```
{名称}_9_tl.{扩展名}  // 左上
{名称}_9_tm.{扩展名}  // 上中
{名称}_9_tr.{扩展名}  // 右上
{名称}_9_ml.{扩展名}  // 左中
{名称}_9_mm.{扩展名}  // 中中
{名称}_9_mr.{扩展名}  // 右中
{名称}_9_bl.{扩展名}  // 左下
{名称}_9_bm.{扩展名}  // 下中
{名称}_9_br.{扩展名}  // 右下
```

#### 动画序列
```
{名称}_{动作}_{帧号:03d}.{扩展名}
```
示例：`player_run_001.png`, `player_run_002.png`, ...

---

## 4. 资源目录结构

### 4.1 推荐目录结构

```
assets/
├── images/
│   ├── ui/
│   │   ├── buttons/
│   │   ├── panels/
│   │   ├── icons/
│   │   └── backgrounds/
│   ├── characters/
│   ├── items/
│   ├── effects/
│   └── backgrounds/
├── fonts/
├── audio/
│   ├── music/
│   ├── sfx/
│   └── voice/
└── spritesheets/
    ├── ui_spritesheet.json
    ├── player_spritesheet.json
    └── ...
```

### 4.2 目录说明

- `images/ui/`：UI 相关图片
- `images/characters/`：角色图片
- `images/items/`：道具图片
- `images/effects/`：特效图片
- `images/backgrounds/`：背景图片
- `fonts/`：字体文件
- `audio/music/`：背景音乐
- `audio/sfx/`：音效
- `audio/voice/`：语音
- `spritesheets/`：Sprite Sheet 描述文件

---

## 5. 资源处理流程

### 5.1 手动处理流程

```text
1. 从设计工具导出资源
2. 按命名规范重命名
3. 按目录结构组织
4. 手动创建 Sprite Sheet（可选）
5. 测试资源加载
```

### 5.2 自动化处理流程

```text
1. 从设计工具导出资源
2. 运行资源处理脚本
   - 自动重命名
   - 自动组织目录
   - 自动生成 Sprite Sheet
   - 自动生成资源清单
3. 测试资源加载
```

---

## 6. 资源清单文件

### 6.1 资源清单格式

建议使用 JSON 格式描述资源清单：

```json
{
  "version": "1.0",
  "export_date": "2026-05-03",
  "assets": {
    "images": [
      {
        "id": "btn_start_normal",
        "path": "images/ui/buttons/btn_start_normal.png",
        "type": "button",
        "width": 200,
        "height": 60,
        "nine_slice": false
      },
      {
        "id": "panel_bg",
        "path": "images/ui/panels/panel_bg.png",
        "type": "panel",
        "width": 400,
        "height": 300,
        "nine_slice": {
          "left": 10,
          "right": 10,
          "top": 10,
          "bottom": 10
        }
      }
    ],
    "fonts": [
      {
        "id": "font_main",
        "path": "fonts/fzpix.ttf",
        "size": 16
      }
    ],
    "audio": [
      {
        "id": "bgm_main",
        "path": "audio/music/main_theme.ogg",
        "type": "music",
        "loop": true
      }
    ],
    "spritesheets": [
      {
        "id": "player_anim",
        "path": "spritesheets/player_spritesheet.json",
        "texture": "images/characters/player_sheet.png"
      }
    ]
  }
}
```

### 6.2 Sprite Sheet 格式

```json
{
  "frames": [
    {
      "filename": "player_run_001.png",
      "frame": {
        "x": 0,
        "y": 0,
        "w": 64,
        "h": 64
      },
      "rotated": false,
      "trimmed": false,
      "spriteSourceSize": {
        "x": 0,
        "y": 0,
        "w": 64,
        "h": 64
      },
      "sourceSize": {
        "w": 64,
        "h": 64
      }
    }
  ],
  "meta": {
    "app": "TexturePacker",
    "version": "1.0",
    "image": "player_sheet.png",
    "format": "RGBA8888",
    "size": {
      "w": 512,
      "h": 512
    },
    "scale": "1"
  }
}
```

---

## 7. 引擎集成

### 7.1 资源加载

使用 AssetManager 加载资源：

```c
// 加载纹理
SDL_Texture* texture = AssetManager_LoadTexture("images/ui/buttons/btn_start_normal.png");

// 加载字体
TTF_Font* font = AssetManager_LoadFont("fonts/fzpix.ttf", 16);

// 加载音频
Mix_Music* music = AssetManager_LoadMusic("audio/music/main_theme.ogg");
Mix_Chunk* sfx = AssetManager_LoadSFX("audio/sfx/click.ogg");
```

### 7.2 资源引用计数

AssetManager 自动管理资源引用计数：

```c
// 增加引用
AssetManager_AddRef(texture);

// 减少引用
AssetManager_Release(texture);

// 自动释放（引用计数为0时）
```

### 7.3 Sprite Sheet 使用

```c
// 加载 Sprite Sheet
SpriteSheet* sheet = SpriteSheet_Load("spritesheets/player_spritesheet.json");

// 获取指定帧
SDL_Texture* frame = SpriteSheet_GetFrame(sheet, "player_run_001.png");

// 播放动画
Animation* anim = Animation_CreateFromSpriteSheet(sheet, "player_run", 0, 10, 0.1f);
```

---

## 8. 场景编辑器集成

### 8.1 资源浏览器

在 WebUI 场景编辑器中添加资源浏览器：

```typescript
// 资源浏览器组件
<ResourceBrowser
  assets={assets}
  onAssetSelect={handleAssetSelect}
  onAssetDrag={handleAssetDrag}
/>
```

### 8.2 拖拽创建

从资源浏览器拖拽资源到场景编辑器：

```typescript
// 拖拽处理
const handleAssetDrag = (asset: Asset) => {
  // 创建 GameObject
  const newObject: GameObject = {
    id: `obj-${Date.now()}`,
    name: asset.name,
    type: 'sprite',
    transform: { x: 0, y: 0, ... },
    components: ['SpriteComponent'],
    assetId: asset.id
  }
  
  // 添加到场景
  addGameObject(newObject)
}
```

### 8.3 实时预览

在场景编辑器中实时预览资源：

```typescript
// 渲染预览
const renderPreview = (asset: Asset) => {
  if (asset.type === 'image') {
    return <img src={asset.path} alt={asset.name} />
  } else if (asset.type === 'animation') {
    return <AnimationPreview spritesheet={asset.spritesheet} />
  }
}
```

---

## 9. AI 辅助工作流

### 9.1 AI 生成资源清单

使用 AI 从设计稿自动生成资源清单：

```python
# scripts/ai_generate_asset_list.py
import anthropic

client = anthropic.Anthropic()

def generate_asset_list(design_file: str):
    # 读取设计文件
    design_data = read_design_file(design_file)
    
    # 调用 AI 生成资源清单
    response = client.messages.create(
        model="claude-sonnet",
        max_tokens=4096,
        messages=[{
            "role": "user",
            "content": f"""
根据以下设计文件，生成资源清单 JSON：

{design_data}

请按照以下格式输出：
- 识别所有需要导出的资源
- 按命名规范命名
- 按目录结构组织
- 包含尺寸信息
"""
        }]
    )
    
    return parse_json(response.content)
```

### 9.2 AI 生成切图脚本

使用 AI 生成自动化切图脚本：

```python
# scripts/ai_generate_export_script.py
def generate_export_script(asset_list: dict):
    script = f"""
import os
import shutil

# 创建目录结构
{create_directory_script(asset_list)}

# 重命名资源
{rename_script(asset_list)}

# 生成 Sprite Sheet
{generate_spritesheet_script(asset_list)}
"""
    
    return script
```

### 9.3 AI 验证资源

使用 AI 验证资源是否符合规范：

```python
# scripts/ai_validate_assets.py
def validate_assets(asset_list: dict):
    issues = []
    
    for asset in asset_list['assets']:
        # 检查命名规范
        if not check_naming_convention(asset['id']):
            issues.append(f"命名不规范: {asset['id']}")
        
        # 检查尺寸
        if asset['width'] > 4096 or asset['height'] > 4096:
            issues.append(f"尺寸过大: {asset['id']}")
        
        # 检查格式
        if not asset['path'].endswith(('.png', '.svg')):
            issues.append(f"格式不支持: {asset['id']}")
    
    return issues
```

---

## 10. 工具推荐

### 10.1 切图工具

- **TexturePacker**：Sprite Sheet 生成
- **Shoebox**：免费 Sprite Sheet 工具
- **SpriteSheetPacker**：开源 Sprite Sheet 工具

### 10.2 资源优化

- **TinyPNG**：PNG 压缩
- **SVGO**：SVG 优化
- **ImageOptim**：图片批量优化

### 10.2 自动化脚本

- **Python**：资源处理脚本
- **Node.js**：前端资源处理
- **Shell**：批量操作

---

## 11. 最佳实践

### 11.1 设计阶段

- 使用组件化设计
- 保持一致的间距和尺寸
- 使用图层组织
- 预留不同状态的设计

### 11.2 导出阶段

- 批量导出而非单个导出
- 使用 2x 或 3x 高清版本
- 保持透明背景
- 使用无损压缩

### 11.3 命名阶段

- 严格遵循命名规范
- 使用语义化名称
- 避免特殊字符
- 保持一致性

### 11.4 集成阶段

- 先测试单个资源
- 再测试批量加载
- 验证引用计数
- 监控内存使用

---

## 12. 常见问题

### 12.1 资源加载失败

**问题**：资源无法加载
**解决**：
- 检查文件路径是否正确
- 检查文件格式是否支持
- 检查文件是否损坏
- 检查 AssetManager 初始化

### 12.2 内存占用过高

**问题**：资源占用内存过大
**解决**：
- 使用 Sprite Sheet 减少纹理数量
- 使用压缩格式
- 及时释放不用的资源
- 使用资源池

### 12.3 九宫格显示异常

**问题**：九宫格拉伸不正确
**解决**：
- 检查切片尺寸
- 检查边距设置
- 检查渲染逻辑
- 使用正确的拉伸算法

---

## 13. 未来扩展

### 13.1 热重载

支持资源热重载，无需重启引擎：

```c
// 监听文件变化
AssetManager_WatchDirectory("assets/", on_asset_changed);

// 热重载回调
void on_asset_changed(const char* path) {
    AssetManager_Reload(path);
}
```

### 13.2 资源依赖

支持资源依赖管理：

```json
{
  "id": "panel_bg",
  "dependencies": [
    "btn_close_normal",
    "btn_close_hover"
  ]
}
```

### 13.3 资源版本

支持资源版本管理：

```json
{
  "version": "2.0",
  "assets": {
    "btn_start": {
      "v1": "images/ui/buttons/btn_start_v1.png",
      "v2": "images/ui/buttons/btn_start_v2.png"
    }
  }
}
```

---

## 14. 总结

本工作流方案定义了从设计稿到引擎资源的完整流程，包括：

1. **设计阶段**：规范设计工具和设计规范
2. **导出阶段**：定义导出格式和设置
3. **命名阶段**：统一命名规范
4. **组织阶段**：定义目录结构
5. **处理阶段**：手动和自动化流程
6. **集成阶段**：引擎加载和使用
7. **AI 辅助**：自动化生成和验证

通过遵循此工作流，可以：
- 提高资源管理效率
- 减少人为错误
- 支持团队协作
- 便于 AI 辅助
- 保持项目一致性
