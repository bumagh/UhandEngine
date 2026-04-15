const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const fs = require('fs');
const path = require('path');
const { exec } = require('child_process');

const app = express();
const PORT = 8081;

// Middleware
app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Project root path
const PROJECT_ROOT = path.resolve(__dirname, '../../');

// API Routes

// Get project structure
app.get('/api/project/structure', (req, res) => {
  const getDirectoryStructure = (dir, baseDir = dir) => {
    const items = fs.readdirSync(dir, { withFileTypes: true });
    return items.map(item => {
      const fullPath = path.join(dir, item.name);
      const relativePath = path.relative(baseDir, fullPath);
      
      if (item.isDirectory()) {
        return {
          name: item.name,
          type: 'folder',
          path: relativePath,
          children: getDirectoryStructure(fullPath, baseDir)
        };
      } else {
        return {
          name: item.name,
          type: 'file',
          path: relativePath
        };
      }
    });
  };

  try {
    const structure = getDirectoryStructure(PROJECT_ROOT);
    res.json({ success: true, structure });
  } catch (error) {
    res.status(500).json({ success: false, error: error.message });
  }
});

// Get file content
app.get('/api/file/content', (req, res) => {
  const filePath = req.query.path;
  if (!filePath) {
    return res.status(400).json({ success: false, error: 'Path is required' });
  }

  const fullPath = path.join(PROJECT_ROOT, filePath);
  
  try {
    const content = fs.readFileSync(fullPath, 'utf-8');
    res.json({ success: true, content });
  } catch (error) {
    res.status(500).json({ success: false, error: error.message });
  }
});

// Save file content
app.post('/api/file/save', (req, res) => {
  const { path: filePath, content } = req.body;
  if (!filePath || content === undefined) {
    return res.status(400).json({ success: false, error: 'Path and content are required' });
  }

  const fullPath = path.join(PROJECT_ROOT, filePath);
  
  try {
    fs.writeFileSync(fullPath, content, 'utf-8');
    res.json({ success: true });
  } catch (error) {
    res.status(500).json({ success: false, error: error.message });
  }
});

// Compile and run example
app.post('/api/compile/run', (req, res) => {
  const { exampleName } = req.body;
  if (!exampleName) {
    return res.status(400).json({ success: false, error: 'Example name is required' });
  }

  const examplesDir = path.join(PROJECT_ROOT, 'examples');
  const examplePath = path.join(examplesDir, `${exampleName}.exe`);
  
  exec(examplePath, { cwd: examplesDir }, (error, stdout, stderr) => {
    if (error) {
      return res.status(500).json({ success: false, error: error.message, stderr });
    }
    res.json({ success: true, output: stdout });
  });
});

// Get engine info
app.get('/api/engine/info', (req, res) => {
  const info = {
    name: 'UhandEngine',
    version: '0.1.0',
    description: 'AI-first 2D game engine built with C and SDL2',
    systems: [
      'Scene',
      'GameObject',
      'Transform',
      'Component',
      'AssetManager',
      'RenderQueue',
      'InputManager',
      'Camera',
      'Animation',
      'Tween',
      'SpriteComponent',
      'Physics',
      'ComponentType (Reflection)'
    ],
    examples: [
      'sprite_example',
      'text_example',
      'container_example',
      'properties_example',
      'scene_example',
      'render_example',
      'asset_example',
      'renderqueue_example',
      'input_example',
      'camera_example',
      'animation_example',
      'tween_example',
      'integration_example',
      'game_example',
      'performance_example',
      'sprite_component_example',
      'physics_example',
      'platformer_example',
      'shooter_example',
      'puzzle_example',
      'reflection_example'
    ]
  };
  
  res.json({ success: true, info });
});

// Health check
app.get('/api/health', (req, res) => {
  res.json({ success: true, status: 'ok', timestamp: new Date().toISOString() });
});

// Start server
app.listen(PORT, () => {
  console.log(`UhandEngine WebUI Backend running on port ${PORT}`);
  console.log(`Project root: ${PROJECT_ROOT}`);
});
