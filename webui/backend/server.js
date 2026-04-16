const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const path = require('path');
const fs = require('fs');
const { exec } = require('child_process');
require('dotenv').config();

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
      const filePath = path.join(pipelineDir, file.path);
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

app.post('/api/pipeline/generate-code', async (req, res) => {
  const { design, requirements } = req.body;
  
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
    // Detect platform
    const isWindows = process.platform === 'win32';

    // Generate code using AI
    const response = await aiService.chat([
      {
        role: 'system',
        content: `You are a JSON formatter. Your ONLY job is to format C code into a specific JSON structure.

You will receive C code and must return it in this exact JSON format:
{"files":[{"path":"file.c","content":"C_CODE_HERE","type":"main"}]]

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

For Windows platform:
- Generate a CMakeLists.txt instead of Makefile
- Use SDL2 find_package in CMake
- Ensure paths use forward slashes for CMake

For Linux/macOS platform:
- Generate a Makefile using sdl2-config
- Use standard SDL2 compilation flags` 
      },
      {
        role: 'user',
        content: `Format this C code into JSON: Generate a simple ${design.game_type || 'game'} with SDL2. Create main.c with game loop, game.c with logic, and a header file. Requirements: ${requirements}

Target platform: ${isWindows ? 'Windows (use CMakeLists.txt)' : 'Linux/macOS (use Makefile with sdl2-config)'}

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
