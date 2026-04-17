const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const path = require('path');
const fs = require('fs');
const { exec } = require('child_process');
require('dotenv').config();

// Store active Web server instances
const webServers = new Map(); // pipelineId -> { server, port, logs: [] }

const AIService = require('./ai-service');

// Load saved AI config from file if exists
const AI_CONFIG_FILE = path.join(__dirname, '.ai-config.json');
let savedAIConfig = null;
if (fs.existsSync(AI_CONFIG_FILE)) {
  try {
    savedAIConfig = JSON.parse(fs.readFileSync(AI_CONFIG_FILE, 'utf8'));
  } catch (error) {
    console.error('Failed to load AI config file:', error);
  }
}

const app = express();
const PORT = 18081;

// Middleware
app.use(cors());
app.use(bodyParser.json({ limit: '50mb' }));
app.use(bodyParser.urlencoded({ extended: true, limit: '50mb' }));

// Request logging middleware
app.use((req, res, next) => {
  console.log(`${new Date().toISOString()} - ${req.method} ${req.url}`);
  next();
});

// Project root path
const PROJECT_ROOT = path.resolve(__dirname, '../../');

// Initialize AI Service
const aiService = new AIService();

// Apply saved AI config if exists
if (savedAIConfig) {
  aiService.setConfig(savedAIConfig);
  console.log('Applied saved AI config from file');
}

// API Routes

// Get project structure
app.get('/api/project/structure', (req, res) => {
  const IGNORED_DIRS = ['node_modules', '.git', 'dist', 'build', '.vscode', 'coverage'];
  const MAX_DEPTH = 10;

  const getDirectoryStructure = (dir, baseDir = dir, depth = 0) => {
    if (depth > MAX_DEPTH) {
      return [];
    }

    try {
      const items = fs.readdirSync(dir, { withFileTypes: true });
      return items
        .filter(item => !IGNORED_DIRS.includes(item.name))
        .map(item => {
          const fullPath = path.join(dir, item.name);
          const relativePath = path.relative(baseDir, fullPath);

          try {
            if (item.isDirectory()) {
              return {
                name: item.name,
                type: 'folder',
                path: relativePath,
                children: getDirectoryStructure(fullPath, baseDir, depth + 1)
              };
            } else {
              return {
                name: item.name,
                type: 'file',
                path: relativePath
              };
            }
          } catch (err) {
            console.warn(`Error processing ${fullPath}:`, err.message);
            return null;
          }
        })
        .filter(item => item !== null);
    } catch (err) {
      console.warn(`Error reading directory ${dir}:`, err.message);
      return [];
    }
  };

  try {
    const structure = getDirectoryStructure(PROJECT_ROOT);
    res.json({ success: true, structure });
  } catch (error) {
    console.error('Project structure error:', error);
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

// AI Routes

// Check AI configuration
app.get('/api/ai/status', (req, res) => {
  res.json({
    success: true,
    configured: aiService.isConfigured(),
    provider: process.env.AI_PROVIDER || 'openai',
  });
});

// AI Chat
app.post('/api/ai/chat', async (req, res) => {
  const { messages, options = {} } = req.body;
  
  if (!messages || !Array.isArray(messages)) {
    return res.status(400).json({ success: false, error: 'Messages array is required' });
  }

  try {
    const response = await aiService.chat(messages, options);
    res.json({ success: true, data: response });
  } catch (error) {
    console.error('AI chat error:', error);
    res.status(500).json({ success: false, error: error.message });
  }
});

// Generate Code
app.post('/api/ai/generate-code', async (req, res) => {
  const { prompt, context = {} } = req.body;
  
  if (!prompt) {
    return res.status(400).json({ success: false, error: 'Prompt is required' });
  }

  try {
    const code = await aiService.generateCode(prompt, context);
    res.json({ success: true, code });
  } catch (error) {
    console.error('Code generation error:', error);
    res.status(500).json({ success: false, error: error.message });
  }
});

// Analyze Code
app.post('/api/ai/analyze', async (req, res) => {
  const { code, context = {} } = req.body;
  
  if (!code) {
    return res.status(400).json({ success: false, error: 'Code is required' });
  }

  try {
    const analysis = await aiService.analyzeCode(code, context);
    res.json({ success: true, analysis });
  } catch (error) {
    console.error('Code analysis error:', error);
    res.status(500).json({ success: false, error: error.message });
  }
});

// Save AI Configuration
app.post('/api/ai/config', (req, res) => {
  const { provider, apiKey, model, baseUrl } = req.body;
  
  console.log('Saving AI config:', { provider, hasApiKey: !!apiKey, model, baseUrl });
  
  if (!apiKey) {
    return res.status(400).json({ success: false, error: 'API key is required' });
  }

  try {
    const config = { provider, apiKey, model, baseUrl };
    
    // Apply to AI service
    aiService.setConfig(config);
    
    // Save to file for persistence
    fs.writeFileSync(AI_CONFIG_FILE, JSON.stringify(config, null, 2));
    console.log('AI config saved to file:', AI_CONFIG_FILE);
    
    res.json({ success: true, message: 'AI configuration saved' });
  } catch (error) {
    console.error('Save config error:', error);
    res.status(500).json({ success: false, error: error.message });
  }
});

// Test AI Configuration
app.post('/api/ai/config/test', async (req, res) => {
  const { provider, apiKey, model, baseUrl } = req.body;
  
  console.log('Testing AI config:', { provider, hasApiKey: !!apiKey, model, baseUrl });
  
  if (!apiKey) {
    return res.status(400).json({ success: false, error: 'API key is required' });
  }

  try {
    // Create a temporary AI service instance for testing
    const testService = new AIService();
    testService.setConfig({ provider, apiKey, model, baseUrl });
    
    console.log('AI service configured, sending test message...');
    
    // Test with a simple message
    const testResponse = await testService.chat([
      { role: 'system', content: 'You are a helpful assistant.' },
      { role: 'user', content: 'Say "Hello" in one word.' }
    ], { model, maxTokens: 10 });
    
    console.log('Test response:', testResponse);
    
    if (testResponse.content) {
      res.json({ success: true, message: 'Configuration test successful', response: testResponse.content });
    } else {
      res.status(500).json({ success: false, error: 'No response from AI' });
    }
  } catch (error) {
    console.error('Test config error:', error);
    console.error('Error stack:', error.stack);
    res.status(500).json({ success: false, error: error.message });
  }
});

// Pipeline API
app.post('/api/pipeline/analyze', async (req, res) => {
  const { requirements } = req.body;
  
  if (!requirements) {
    return res.status(400).json({ success: false, error: 'Requirements are required' });
  }

  // Check if AI is configured
  if (!aiService.isConfigured()) {
    return res.status(400).json({ 
      success: false, 
      error: 'AI not configured. Please configure AI in Settings first.' 
    });
  }

  try {
    // Analyze requirements using AI
    const response = await aiService.chat([
      { 
        role: 'system', 
        content: 'You are a game design expert. Analyze the user requirements and provide a brief summary of the game type and key features.' 
      },
      { role: 'user', content: requirements }
    ]);

    res.json({ 
      success: true, 
      analysis: response.content 
    });
  } catch (error) {
    console.error('Requirements analysis error:', error);
    res.status(500).json({ success: false, error: error.message });
  }
});

app.post('/api/pipeline/design', async (req, res) => {
  const { requirements } = req.body;
  
  if (!requirements) {
    return res.status(400).json({ success: false, error: 'Requirements are required' });
  }

  // Check if AI is configured
  if (!aiService.isConfigured()) {
    return res.status(400).json({ 
      success: false, 
      error: 'AI not configured. Please configure AI in Settings first.' 
    });
  }

  try {
    // Generate design using AI
    const response = await aiService.chat([
      { 
        role: 'system', 
        content: `You are a JSON data generator. Your ONLY task is to output valid JSON data.

Generate a game design in JSON format with this exact structure:
{
  "game_type": "string describing the game type",
  "scenes": [
    {
      "name": "scene name",
      "objects": ["object1", "object2"],
      "description": "scene description"
    }
  ],
  "components": ["component1", "component2"],
  "features": ["feature1", "feature2", "feature3"]
}

IMPORTANT RULES:
1. Output ONLY the JSON object
2. NO markdown code blocks (\`\`\`)
3. NO explanations
4. NO additional text
5. Start your response with {
6. End your response with }
7. Ensure all JSON syntax is valid` 
      },
      { role: 'user', content: `Generate a game design JSON for: ${requirements}` }
    ]);

    console.log('AI design response:', response.content);

    // Try to extract JSON from response
    let jsonStr = response.content;
    
    // Remove markdown code blocks if present
    jsonStr = jsonStr.replace(/```json\s*/g, '').replace(/```\s*/g, '');
    
    // Try to find JSON object in the content
    const jsonMatch = jsonStr.match(/\{[\s\S]*\}/);
    if (jsonMatch) {
      jsonStr = jsonMatch[0];
    }

    let design;
    try {
      design = JSON.parse(jsonStr);
    } catch (parseError) {
      console.error('JSON parse error:', parseError);
      console.error('Attempted to parse:', jsonStr);
      throw new Error(`Failed to parse AI response as JSON: ${parseError.message}`);
    }

    // Validate design structure and provide defaults
    if (!design.game_type) {
      design.game_type = 'Unknown Game Type';
    }
    if (!design.scenes || !Array.isArray(design.scenes)) {
      design.scenes = [{ name: 'Main Scene', objects: [], description: 'Default scene' }];
    }
    if (!design.components || !Array.isArray(design.components)) {
      design.components = ['sprite', 'transform'];
    }
    if (!design.features || !Array.isArray(design.features)) {
      design.features = ['Basic gameplay'];
    }

    console.log('Parsed design:', design);

    res.json({ 
      success: true, 
      design 
    });
  } catch (error) {
    console.error('Design generation error:', error);
    
    // Handle 429 rate limit errors
    if (error.message.includes('429')) {
      return res.status(429).json({ 
        success: false, 
        error: 'API rate limit exceeded. Please wait a moment and try again.' 
      });
    }

    res.status(500).json({ success: false, error: error.message });
  }
});

// Pipeline save files endpoint
app.post('/api/pipeline/save-files', async (req, res) => {
  const { pipelineId, files } = req.body;

  if (!pipelineId) {
    return res.status(400).json({
      success: false,
      error: 'Pipeline ID is required'
    });
  }

  if (!files || !Array.isArray(files)) {
    return res.status(400).json({
      success: false,
      error: 'Files array is required'
    });
  }

  try {
    const fs = require('fs');
    const path = require('path');

    // Create directory for this pipeline
    const pipelineDir = path.join(PROJECT_ROOT, 'engine-ref', pipelineId);

    if (!fs.existsSync(pipelineDir)) {
      fs.mkdirSync(pipelineDir, { recursive: true });
    }

    // Write each file
    files.forEach(file => {
      // Validate file path
      if (!file.path || typeof file.path !== 'string') {
        console.warn('Skipping invalid file: missing path');
        return;
      }
      if (!file.content || typeof file.content !== 'string') {
        console.warn(`Skipping invalid file: missing content for ${file.path}`);
        return;
      }

      const filePath = path.join(pipelineDir, file.path);
      const resolvedPath = path.resolve(filePath);
      const resolvedPipelineDir = path.resolve(pipelineDir);

      // Prevent path traversal attacks
      if (!resolvedPath.startsWith(resolvedPipelineDir)) {
        console.error(`Path traversal detected: ${file.path}`);
        throw new Error(`Invalid file path: path traversal detected`);
      }

      const dir = path.dirname(filePath);

      // Create directory if it doesn't exist
      if (!fs.existsSync(dir)) {
        fs.mkdirSync(dir, { recursive: true });
      }

      fs.writeFileSync(filePath, file.content, 'utf8');
      console.log(`Saved file: ${filePath}`);
    });

    res.json({
      success: true,
      path: pipelineDir,
      fileCount: files.length
    });
  } catch (error) {
    console.error('Save files error:', error);
    res.status(500).json({
      success: false,
      error: error.message
    });
  }
});

// Pipeline compile endpoint
app.post('/api/pipeline/compile', async (req, res) => {
  const { pipelineId, files, platform } = req.body;

  if (!pipelineId) {
    return res.status(400).json({
      success: false,
      error: 'Pipeline ID is required'
    });
  }

  if (!files || !Array.isArray(files)) {
    return res.status(400).json({
      success: false,
      error: 'Files array is required'
    });
  }

  const targetPlatform = platform || 'windows';
  console.log('Compiling for platform:', targetPlatform);

  try {
    const pipelineDir = path.join(PROJECT_ROOT, 'engine-ref', pipelineId);

    // Save files to pipeline directory
    files.forEach(file => {
      if (!file.path || typeof file.path !== 'string') {
        console.warn('Skipping invalid file: missing path');
        return;
      }
      if (!file.content || typeof file.content !== 'string') {
        console.warn(`Skipping invalid file: missing content for ${file.path}`);
        return;
      }

      const filePath = path.join(pipelineDir, file.path);
      const resolvedPath = path.resolve(filePath);
      const resolvedPipelineDir = path.resolve(pipelineDir);

      if (!resolvedPath.startsWith(resolvedPipelineDir)) {
        console.error(`Path traversal detected: ${file.path}`);
        throw new Error(`Invalid file path: path traversal detected`);
      }

      const dir = path.dirname(filePath);

      if (!fs.existsSync(dir)) {
        fs.mkdirSync(dir, { recursive: true });
      }

      let content = file.content;

      // Auto-inject SDL_MAIN_HANDLED for Windows main.c files
      if (file.path === 'main.c' && process.platform === 'win32') {
        if (!content.includes('SDL_MAIN_HANDLED')) {
          content = '#define SDL_MAIN_HANDLED\n' + content;
          console.log('Injected SDL_MAIN_HANDLED into main.c');
        }
        if (!content.includes('SDL_SetMainReady')) {
          content = content.replace(
            /int\s+main\s*\(.*?\)\s*\{/,
            match => match + '\n    SDL_SetMainReady();'
          );
          console.log('Injected SDL_SetMainReady() into main()');
        }
      }

      // Auto-fix Linux font paths to Windows font paths
      if (process.platform === 'win32') {
        content = content.replace(
          /\/usr\/share\/fonts\/truetype\/dejavu\/DejaVuSans-Bold\.ttf/g,
          'C:\\\\Windows\\\\Fonts\\\\arial.ttf'
        );
        content = content.replace(
          /\/usr\/share\/fonts\/[^"']+/g,
          'C:\\\\Windows\\\\Fonts\\\\arial.ttf'
        );
        if (content !== file.content) {
          console.log(`Fixed Linux font paths in ${file.path}`);
        }
      }

      fs.writeFileSync(filePath, content, 'utf8');
      console.log(`Saved file: ${filePath}`);
    });

    // Check for build script based on platform
    let buildCommand = '';
    let buildDir = pipelineDir;

    if (targetPlatform === 'web') {
      // Web platform - use Emscripten
      const isWindows = process.platform === 'win32';
      const emscriptenScript = path.join(pipelineDir, isWindows ? 'build-emscripten.bat' : 'build-emscripten.sh');

      if (fs.existsSync(emscriptenScript)) {
        // Set Emscripten environment variables before running build
        const emsdkPath = 'C:\\emsdk';
        const emscriptenPath = path.join(emsdkPath, 'upstream', 'emscripten');
        buildCommand = isWindows
          ? `set EMSDK=${emsdkPath} && set PATH=${emsdkPath};${emscriptenPath};%PATH% && cd "${pipelineDir}" && build-emscripten.bat`
          : `cd "${pipelineDir}" && bash build-emscripten.sh`;
      } else {
        // Copy shell.html template to pipeline directory
        const shellTemplatePath = path.join(PROJECT_ROOT, 'web', 'html_template', 'shell.html');
        const shellDestPath = path.join(pipelineDir, 'shell.html');
        if (fs.existsSync(shellTemplatePath) && !fs.existsSync(shellDestPath)) {
          fs.copyFileSync(shellTemplatePath, shellDestPath);
          console.log(`Copied shell.html template to ${pipelineDir}`);
        }
        // Generate default Emscripten build script
        if (isWindows) {
          const emscriptenScriptContent = `@echo off
REM Emscripten build script for Windows
REM Based on src/makefile configuration
echo Building for Web with Emscripten...

REM Set Emscripten environment
set EMSDK=C:\\emsdk
set PATH=C:\\emsdk;C:\\emsdk\\upstream\\emscripten;%PATH%

emcc game.c main.c ^
  -Wno-int-conversion ^
  -Wno-implicit-function-declaration ^
  -Wno-incompatible-function-pointer-types ^
  -s USE_SDL=2 ^
  -s USE_SDL_TTF=2 ^
  -s WASM=1 ^
  -s ALLOW_MEMORY_GROWTH=1 ^
  -I../../include ^
  -o game.html ^
  -std=c99

echo Build complete! Open game.html in a browser.
`;
          fs.writeFileSync(emscriptenScript, emscriptenScriptContent, 'utf8');
          buildCommand = `cd "${pipelineDir}" && build-emscripten.bat`;
        } else {
          const emscriptenScriptContent = `#!/bin/bash
# Emscripten build script
# Based on src/makefile configuration
echo "Building for Web with Emscripten..."

emcc game.c main.c \\
  -Wno-int-conversion \\
  -Wno-implicit-function-declaration \\
  -Wno-incompatible-function-pointer-types \\
  -s USE_SDL=2 \\
  -s USE_SDL_TTF=2 \\
  -s WASM=1 \\
  -s ALLOW_MEMORY_GROWTH=1 \\
  -I../../include \\
  -o game.html \\
  -std=c99

echo "Build complete! Open game.html in a browser."
`;
          fs.writeFileSync(emscriptenScript, emscriptenScriptContent, 'utf8');
          buildCommand = `cd "${pipelineDir}" && bash build-emscripten.sh`;
        }
      }
      buildDir = pipelineDir;
    } else {
      // Windows platform
      const buildScript = path.join(pipelineDir, 'build.bat');
      const cmakeFile = path.join(pipelineDir, 'CMakeLists.txt');
      const makefile = path.join(pipelineDir, 'Makefile');

      if (fs.existsSync(buildScript)) {
        // Use build.bat for Windows
        buildCommand = `cd "${pipelineDir}" && build.bat`;
      } else if (fs.existsSync(cmakeFile)) {
        // Use CMake
        const cmakeBuildDir = path.join(pipelineDir, 'build');
        if (!fs.existsSync(cmakeBuildDir)) {
          fs.mkdirSync(cmakeBuildDir);
        }
        buildDir = cmakeBuildDir;
        buildCommand = `cd "${cmakeBuildDir}" && cmake .. && cmake --build .`;
      } else if (fs.existsSync(makefile)) {
        // Use Makefile
        buildCommand = `cd "${pipelineDir}" && make`;
      } else {
        return res.status(400).json({
          success: false,
          error: 'No build script found (build.bat, CMakeLists.txt, or Makefile)'
        });
      }
    }

    console.log('Build command:', buildCommand);

    // Execute build
    exec(buildCommand, (error, stdout, stderr) => {
      const fullOutput = (stdout || '') + '\n' + (stderr || '');
      console.log('Build output:', fullOutput);

      if (error) {
        console.error('Build error:', error);
        return res.json({
          success: false,
          status: 'error',
          output: fullOutput
        });
      }

      // Find the executable based on platform
      let executable = '';
      if (targetPlatform === 'web') {
        // Web platform - look for HTML file
        const htmlFiles = fs.readdirSync(buildDir).filter(f => f.endsWith('.html'));
        console.log('Found HTML files in build dir:', htmlFiles);
        if (htmlFiles.length > 0) {
          executable = path.join(buildDir, htmlFiles[0]);
        }
      } else {
        // Native platform
        if (process.platform === 'win32') {
          const exeFiles = fs.readdirSync(buildDir).filter(f => f.endsWith('.exe'));
          console.log('Found exe files in build dir:', exeFiles);
          if (exeFiles.length > 0) {
            executable = path.join(buildDir, exeFiles[0]);
          }
        } else {
          // Linux/macOS
          const exeFiles = fs.readdirSync(buildDir).filter(f => {
            const filePath = path.join(buildDir, f);
            try {
              fs.accessSync(filePath, fs.constants.X_OK);
              return true;
            } catch {
              return false;
            }
          });
          if (exeFiles.length > 0) {
            executable = path.join(buildDir, exeFiles[0]);
          }
        }

        // Copy SDL2 DLL files to pipeline directory for runtime (native only)
        const dllSourceDir = path.join(PROJECT_ROOT, 'bin', 'UhandEngine');
        const dllFiles = ['SDL2.dll', 'SDL2_ttf.dll'];
        dllFiles.forEach(dllFile => {
          const src = path.join(dllSourceDir, dllFile);
          const dest = path.join(pipelineDir, dllFile);
          if (fs.existsSync(src)) {
            fs.copyFileSync(src, dest);
            console.log(`Copied ${dllFile} to ${pipelineDir}`);
          } else {
            console.warn(`DLL file not found: ${src}`);
          }
        });
      }

      console.log('Executable found:', executable);

      res.json({
        success: true,
        status: 'completed',
        output: fullOutput,
        executable: executable
      });
    });
  } catch (error) {
    console.error('Compile error:', error);
    res.status(500).json({
      success: false,
      error: error.message
    });
  }
});

// Pipeline run endpoint
app.post('/api/pipeline/run', async (req, res) => {
  const { pipelineId, executable, platform } = req.body;

  console.log('Run request received:', { pipelineId, executable, platform });

  if (!pipelineId) {
    console.error('Pipeline ID is required');
    return res.status(400).json({
      success: false,
      error: 'Pipeline ID is required'
    });
  }

  const targetPlatform = platform || 'windows';

  try {
    const pipelineDir = path.join(PROJECT_ROOT, 'engine-ref', pipelineId);
    let targetExecutable = executable;

    console.log('Pipeline directory:', pipelineDir);
    console.log('Target platform:', targetPlatform);

    // If executable not provided, try to find it
    if (!targetExecutable) {
      if (targetPlatform === 'web') {
        // Web platform - look for HTML file
        const htmlFiles = fs.readdirSync(pipelineDir).filter(f => f.endsWith('.html'));
        console.log('Found HTML files:', htmlFiles);
        if (htmlFiles.length > 0) {
          targetExecutable = path.join(pipelineDir, htmlFiles[0]);
        }
      } else {
        // Native platform
        const buildDir = path.join(pipelineDir, 'build');
        console.log('Checking build directory:', buildDir, 'exists:', fs.existsSync(buildDir));

        if (fs.existsSync(buildDir)) {
          if (process.platform === 'win32') {
            const exeFiles = fs.readdirSync(buildDir).filter(f => f.endsWith('.exe'));
            console.log('Found exe files in build:', exeFiles);
            if (exeFiles.length > 0) {
              targetExecutable = path.join(buildDir, exeFiles[0]);
            }
          } else {
            const exeFiles = fs.readdirSync(buildDir).filter(f => {
              const filePath = path.join(buildDir, f);
              try {
                fs.accessSync(filePath, fs.constants.X_OK);
                return true;
              } catch {
                return false;
              }
            });
            if (exeFiles.length > 0) {
              targetExecutable = path.join(buildDir, exeFiles[0]);
            }
          }
        }

        // Check in pipeline directory
        if (!targetExecutable) {
          console.log('Checking pipeline directory:', pipelineDir, 'exists:', fs.existsSync(pipelineDir));
          if (fs.existsSync(pipelineDir)) {
            if (process.platform === 'win32') {
              const exeFiles = fs.readdirSync(pipelineDir).filter(f => f.endsWith('.exe'));
              console.log('Found exe files in pipeline dir:', exeFiles);
              if (exeFiles.length > 0) {
                targetExecutable = path.join(pipelineDir, exeFiles[0]);
              }
            } else {
              const exeFiles = fs.readdirSync(pipelineDir).filter(f => {
                const filePath = path.join(pipelineDir, f);
                try {
                  fs.accessSync(filePath, fs.constants.X_OK);
                  return true;
                } catch {
                  return false;
                }
              });
              if (exeFiles.length > 0) {
                targetExecutable = path.join(pipelineDir, exeFiles[0]);
              }
            }
          }
        }
      }
    }

    if (!targetExecutable) {
      console.error('No executable found');
      return res.status(400).json({
        success: false,
        error: 'No executable found. Please compile first.'
      });
    }

    console.log('Running executable:', targetExecutable);
    console.log('Executable exists:', fs.existsSync(targetExecutable));

    if (targetPlatform === 'web') {
      // Web platform - start HTTP server
      const http = require('http');
      const fs = require('fs');
      const path = require('path');

      const port = 18082;
      const logs = [];

      // Stop existing server for this pipeline if any
      if (webServers.has(pipelineId)) {
        const existing = webServers.get(pipelineId);
        existing.server.close();
        webServers.delete(pipelineId);
        console.log(`Stopped existing Web server for pipeline ${pipelineId}`);
      }

      const server = http.createServer((req, res) => {
        const timestamp = new Date().toISOString();
        const logEntry = `[${timestamp}] ${req.method} ${req.url}`;
        logs.push(logEntry);
        console.log(logEntry);

        let filePath = path.join(pipelineDir, req.url === '/' ? 'test.html' : req.url);
        if (!fs.existsSync(filePath)) {
          filePath = targetExecutable;
        }

        const extname = path.extname(filePath);
        const isBinary = ['.wasm', '.data'].includes(extname);
        const contentType = {
          '.html': 'text/html',
          '.js': 'text/javascript',
          '.wasm': 'application/wasm',
          '.data': 'application/octet-stream',
          '.css': 'text/css'
        }[extname] || 'application/octet-stream';

        fs.readFile(filePath, (err, content) => {
          if (err) {
            const errorLog = `[${timestamp}] Error serving ${req.url}: ${err.message}`;
            logs.push(errorLog);
            console.error(errorLog);
            res.writeHead(500);
            res.end('Server Error');
            return;
          }
          res.writeHead(200, { 'Content-Type': contentType });
          if (isBinary) {
            res.end(content);
          } else {
            res.end(content, 'utf-8');
          }
        });
      });

      server.listen(port, () => {
        const startLog = `[${new Date().toISOString()}] Web server started on port ${port}`;
        logs.push(startLog);
        console.log(startLog);

        webServers.set(pipelineId, { server, port, logs });

        res.json({
          success: true,
          status: 'completed',
          output: `Web server started at http://localhost:${port}`,
          url: `http://localhost:${port}`
        });
      });
    } else {
      // Native platform - run executable
      exec(`"${targetExecutable}"`, (error, stdout, stderr) => {
        console.log('Run completed:', { error, stdoutLength: stdout?.length, stderrLength: stderr?.length });
        res.json({
          success: true,
          status: 'completed',
          output: stdout || stderr || error?.message || 'Program executed successfully'
        });
      });
    }
  } catch (error) {
    console.error('Run error:', error);
    res.status(500).json({
      success: false,
      error: error.message
    });
  }
});

// Stop Web server
app.post('/api/pipeline/stop-web-server', (req, res) => {
  const { pipelineId } = req.body;

  if (!pipelineId) {
    return res.status(400).json({ success: false, error: 'Pipeline ID is required' });
  }

  if (webServers.has(pipelineId)) {
    const webServer = webServers.get(pipelineId);
    webServer.server.close();
    webServers.delete(pipelineId);
    console.log(`Stopped Web server for pipeline ${pipelineId}`);
    res.json({ success: true, message: 'Web server stopped' });
  } else {
    res.status(404).json({ success: false, error: 'No active Web server for this pipeline' });
  }
});

// Get Web server logs
app.get('/api/pipeline/web-server-logs', (req, res) => {
  const { pipelineId } = req.query;

  if (!pipelineId) {
    return res.status(400).json({ success: false, error: 'Pipeline ID is required' });
  }

  if (webServers.has(pipelineId)) {
    const webServer = webServers.get(pipelineId);
    res.json({ success: true, logs: webServer.logs });
  } else {
    res.status(404).json({ success: false, error: 'No active Web server for this pipeline' });
  }
});

app.post('/api/pipeline/generate-code', async (req, res) => {
  const { design, requirements, platform } = req.body;

  if (!design) {
    return res.status(400).json({ success: false, error: 'Design is required' });
  }

  // Check if AI is configured
  if (!aiService.isConfigured()) {
    return res.status(400).json({
      success: false,
      error: 'AI not configured. Please configure AI in Settings first.'
    });
  }

  try {
    const targetPlatform = platform || 'windows';
    const isWindows = process.platform === 'win32';
    const isWeb = targetPlatform === 'web';

    // Generate code using AI
    const response = await aiService.chat([
      {
        role: 'system',
        content: `You are a JSON formatter. Your ONLY job is to format C code into a specific JSON structure.

You will receive C code and must return it in this exact JSON format:
{"files":[{"path":"file.c","content":"C_CODE_HERE","type":"main"}]}

RULES:
- Return ONLY the JSON, no other text
- No markdown code blocks (\`\`\`)
- No explanations
- Start with {
- End with }
- Escape newlines as \\n
- Escape quotes as \\"
- Escape backslashes as \\\\
- The content field must be a valid JSON string containing the C code

CRITICAL Windows main.c requirements:
- The FIRST line of main.c MUST be: #define SDL_MAIN_HANDLED
- The first line inside main() MUST be: SDL_SetMainReady();
- This is REQUIRED to prevent "undefined reference to WinMain" linker error on Windows

CRITICAL Windows font requirements:
- Use Windows system fonts, NOT Linux paths
- For TTF_OpenFont, use: "C:\\Windows\\Fonts\\arial.ttf" or "C:\\Windows\\Fonts\\tahoma.ttf"
- Or use NULL to let SDL2 choose a default font
- NEVER use Linux paths like /usr/share/fonts/

${isWeb ? `
CRITICAL Web platform requirements:
- Generate build-emscripten.sh for Emscripten compilation
- build-emscripten.sh must use emcc to compile to WebAssembly
- Use SDL2 flags: -s USE_SDL=2 -s USE_SDL_TTF=2 -s WASM=1
- Output should be game.html
- Include shell.html template for web canvas
` : `
For Windows platform:
- Generate a build.bat script (not Makefile or CMakeLists.txt)
- build.bat must use these EXACT settings:
  set SDL2_INC=..\\..\\include
  set SDL2_LIB=..\\..\\lib
  set SDL2_VER=SDL2_2.28.1
  set SDL2_TTF_VER=SDL2_ttf_2.20.2
- Compile each .c file separately with: gcc -c file.c -I%SDL2_INC% -o file.o
- Link with: gcc *.o -o game.exe -L%SDL2_LIB%\\%SDL2_VER% -lSDL2 -L%SDL2_LIB%\\%SDL2_TTF_VER% -lSDL2_ttf -mconsole
- Do NOT include -lSDL2main in the link command
`}`
      },
      {
        role: 'user',
        content: `Format this C code into JSON: Generate a simple ${design.game_type || 'game'} with SDL2. Create main.c with game loop, game.c with logic, and a header file. Requirements: ${requirements}

Target platform: ${isWeb ? 'Web (Emscripten)' : 'Windows'}

${isWeb ? `
IMPORTANT: Generate build-emscripten.sh script with emcc compilation flags.
The script should compile to game.html using -s USE_SDL=2 -s USE_SDL_TTF=2 -s WASM=1.
Include a shell.html template for web canvas rendering.
` : `
IMPORTANT: main.c must start with #define SDL_MAIN_HANDLED before any includes.
The first line inside main() must call SDL_SetMainReady().
Generate build.bat for compilation.
`}

Return the result as JSON with files array containing path, content (as escaped string), and type fields.`
      }
    ]);

    console.log('AI code generation response:', response.content);

    // Try to parse as JSON first
    let codeData;
    let jsonStr = response.content;

    // Remove markdown code blocks if present
    jsonStr = jsonStr.replace(/```json\s*/g, '').replace(/```\s*/g, '');

    // Try to find JSON object in the content
    const jsonMatch = jsonStr.match(/\{[\s\S]*\}/);
    if (jsonMatch) {
      jsonStr = jsonMatch[0];
    }

    console.log('Extracted JSON string length:', jsonStr.length);
    console.log('First 500 chars of extracted JSON:', jsonStr.substring(0, 500));

    try {
      codeData = JSON.parse(jsonStr);
      console.log('Parsed codeData keys:', Object.keys(codeData));
      console.log('codeData.files:', codeData.files);
    } catch (parseError) {
      console.error('JSON parse error:', parseError);
      console.error('Attempted to parse:', jsonStr);

      // If JSON parsing fails, try to extract markdown code blocks
      console.log('Attempting to extract markdown code blocks...');
      const codeBlockRegex = /```(?:c|h|makefile)?\s*([\s\S]*?)```/gi;
      const matches = [...response.content.matchAll(codeBlockRegex)];

      if (matches.length > 0) {
        console.log(`Found ${matches.length} code blocks`);

        // Try to extract file names from the content
        const files = [];
        const fileNameRegex = /\*\*([a-zA-Z0-9_]+\.[a-zA-Z]+)\*\*/g;
        const fileNames = [...response.content.matchAll(fileNameRegex)].map(m => m[1]);

        console.log('Extracted file names:', fileNames);

        // If we have file names, use them; otherwise use defaults
        const defaultNames = ['main.c', 'game.h', 'game.c', 'ui.h', 'ui.c', 'storage.h', 'storage.c', 'Makefile'];
        const namesToUse = fileNames.length > 0 ? fileNames : defaultNames.slice(0, matches.length);

        matches.forEach((match, index) => {
          const code = match[1].trim();
          const fileName = namesToUse[index] || `file_${index}.c`;
          const ext = fileName.split('.').pop();
          let type = 'code';
          if (ext === 'h') type = 'header';
          else if (ext === 'c') type = 'source';
          else if (ext === 'makefile' || fileName === 'Makefile') type = 'build';

          files.push({
            path: fileName,
            content: code,
            type: type
          });
        });

        codeData = { files };
        console.log('Constructed codeData from markdown blocks:', codeData);
      } else {
        throw new Error(`Failed to parse AI response as JSON: ${parseError.message}`);
      }
    }

    // Validate code structure and provide defaults
    if (!codeData.files || !Array.isArray(codeData.files)) {
      console.warn('No files array in response, using empty array');
      codeData.files = [];
    }

    console.log('Generated code files:', codeData.files.length);
    if (codeData.files.length > 0) {
      console.log('First file path:', codeData.files[0].path);
      console.log('First file content length:', codeData.files[0].content?.length);
    }

    res.json({ 
      success: true, 
      files: codeData.files 
    });
  } catch (error) {
    console.error('Code generation error:', error);
    
    // Handle 429 rate limit errors
    if (error.message.includes('429')) {
      return res.status(429).json({ 
        success: false, 
        error: 'API rate limit exceeded. Please wait a moment and try again.' 
      });
    }
    
    res.status(500).json({ success: false, error: error.message });
  }
});

// Start server
app.listen(PORT, () => {
  console.log(`UhandEngine WebUI Backend running on port ${PORT}`);
  console.log(`Project root: ${PROJECT_ROOT}`);
});
