const OpenAI = require('openai');

class AIService {
  constructor() {
    this.provider = process.env.AI_PROVIDER || 'openai';
    this.openai = null;
    this.config = null; // Dynamic config from WebUI
    
    this.initialize();
  }

  initialize() {
    if (this.provider === 'openai') {
      const apiKey = process.env.OPENAI_API_KEY;
      if (!apiKey) {
        console.warn('OPENAI_API_KEY not found in environment variables');
        return;
      }
      this.openai = new OpenAI({ apiKey });
      console.log('OpenAI client initialized');
    }
  }

  setConfig(config) {
    this.config = config;
    
    // Reinitialize with new config
    if (config.apiKey) {
      try {
        // Use OpenAI client for both OpenAI and OpenAI-compatible APIs
        this.openai = new OpenAI({ 
          apiKey: config.apiKey,
          baseURL: config.baseUrl || 'https://api.openai.com/v1',
          defaultHeaders: {
            'anthropic-version': '2023-06-01'
          }
        });
        console.log('AI client initialized with dynamic config');
      } catch (error) {
        console.error('Failed to initialize AI client with dynamic config:', error);
      }
    }
  }

  async chat(messages, options = {}) {
    try {
      if (this.openai) {
        return await this.chatWithOpenAI(messages, options);
      } else {
        throw new Error('AI provider not configured');
      }
    } catch (error) {
      console.error('AI chat error:', error);
      throw error;
    }
  }

  async chatWithOpenAI(messages, options = {}) {
    const model = options.model || (this.config?.model) || process.env.OPENAI_MODEL || 'gpt-3.5-turbo';
    
    console.log('Sending request to:', this.openai.baseURL);
    console.log('Model:', model);
    console.log('Messages:', messages);
    
    const response = await this.openai.chat.completions.create({
      model,
      messages,
      temperature: options.temperature || 0.7,
      max_tokens: options.maxTokens || 1000,
      stream: options.stream || false,
    });

    console.log('Raw response:', JSON.stringify(response, null, 2));

    if (options.stream) {
      return response; // Stream object
    }

    // Try to extract content from different response formats
    let content = null;
    let usage = response.usage;
    let model_name = response.model;

    // OpenAI format
    if (response.choices && response.choices.length > 0 && response.choices[0].message) {
      content = response.choices[0].message.content;
      console.log('Detected OpenAI format');
    }
    // Anthropic format
    else if (response.content && Array.isArray(response.content)) {
      const textBlock = response.content.find(block => block.type === 'text');
      if (textBlock && textBlock.text) {
        content = textBlock.text;
        console.log('Detected Anthropic format');
      }
    }
    // Direct content field
    else if (response.content && typeof response.content === 'string') {
      content = response.content;
      console.log('Detected direct content format');
    }

    if (!content) {
      console.error('Response has no extractable content:', response);
      throw new Error('No content in response from API. Response format not recognized.');
    }

    return {
      content,
      usage,
      model: model_name,
    };
  }

  async generateCode(prompt, context = {}) {
    const systemPrompt = this.buildSystemPrompt(context);
    const messages = [
      { role: 'system', content: systemPrompt },
      { role: 'user', content: prompt },
    ];

    const response = await this.chat(messages, { temperature: 0.3 });
    return response.content;
  }

  async analyzeCode(code, context = {}) {
    const systemPrompt = this.buildSystemPrompt(context);
    const prompt = `Analyze this code and provide feedback:\n\n${code}\n\nFocus on:
- Potential bugs or issues
- Code quality improvements
- Performance considerations
- Best practices for UhandEngine`;

    const messages = [
      { role: 'system', content: systemPrompt },
      { role: 'user', content: prompt },
    ];

    const response = await this.chat(messages, { temperature: 0.5 });
    return response.content;
  }

  buildSystemPrompt(context) {
    let prompt = `You are an AI assistant for UhandEngine, a C-based 2D game engine built with SDL2.
You help users write game code, debug issues, and understand engine systems.

Engine Systems:
- Scene, GameObject, Transform, Component
- AssetManager, RenderQueue, InputManager
- Camera, Animation, Tween, Physics
- ComponentType reflection system

Code Style:
- C language with SDL2
- Prefix: UhandEngine_ or Scene_, GameObject_, etc.
- Memory management: explicit allocation/deallocation
- Error handling: check return values

`;

    if (context.projectStructure) {
      prompt += `\nProject Structure:\n${JSON.stringify(context.projectStructure, null, 2)}\n`;
    }

    if (context.engineInfo) {
      prompt += `\nEngine Info:\n${JSON.stringify(context.engineInfo, null, 2)}\n`;
    }

    if (context.currentFile) {
      prompt += `\nCurrent File: ${context.currentFile.path}\n`;
      prompt += `Content:\n${context.currentFile.content}\n`;
    }

    return prompt;
  }

  isConfigured() {
    // Check if AI client is initialized (either from env or dynamic config)
    return !!this.openai;
  }
}

module.exports = AIService;
