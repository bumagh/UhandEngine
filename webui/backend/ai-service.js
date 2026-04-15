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
          baseURL: config.baseUrl || 'https://api.openai.com/v1'
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
    
    const response = await this.openai.chat.completions.create({
      model,
      messages,
      temperature: options.temperature || 0.7,
      max_tokens: options.maxTokens || 1000,
      stream: options.stream || false,
    });

    if (options.stream) {
      return response; // Stream object
    }

    // Validate response
    if (!response.choices || response.choices.length === 0) {
      throw new Error('No choices in response from OpenAI');
    }

    if (!response.choices[0].message || !response.choices[0].message.content) {
      throw new Error('No message content in response from OpenAI');
    }

    return {
      content: response.choices[0].message.content,
      usage: response.usage,
      model: response.model,
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
    if (this.provider === 'openai') {
      return !!this.openai;
    }
    return false;
  }
}

module.exports = AIService;
