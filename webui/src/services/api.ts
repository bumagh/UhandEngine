const API_BASE_URL = '/api'

export interface FileNode {
  name: string
  type: 'file' | 'folder'
  path: string
  children?: FileNode[]
}

export interface EngineInfo {
  name: string
  version: string
  description: string
  systems: string[]
  examples: string[]
}

export interface ApiResponse<T = any> {
  success: boolean
  error?: string
  // Data is directly on the response, not in a data property
  [key: string]: any
}

class ApiService {
  private async request<T>(
    endpoint: string,
    options: RequestInit = {}
  ): Promise<ApiResponse<T>> {
    try {
      const response = await fetch(`${API_BASE_URL}${endpoint}`, {
        headers: {
          'Content-Type': 'application/json',
          ...options.headers,
        },
        ...options,
      })

      const data = await response.json()
      return data
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : 'Unknown error',
      }
    }
  }

  async getProjectStructure(): Promise<ApiResponse<{ structure: FileNode[] }>> {
    return this.request('/project/structure')
  }

  async getFileContent(path: string): Promise<ApiResponse<{ content: string }>> {
    return this.request(`/file/content?path=${encodeURIComponent(path)}`)
  }

  async saveFile(path: string, content: string): Promise<ApiResponse> {
    return this.request('/file/save', {
      method: 'POST',
      body: JSON.stringify({ path, content }),
    })
  }

  async compileAndRun(exampleName: string): Promise<ApiResponse<{ output: string }>> {
    return this.request('/compile/run', {
      method: 'POST',
      body: JSON.stringify({ exampleName }),
    })
  }

  async getEngineInfo(): Promise<ApiResponse<{ info: EngineInfo }>> {
    return this.request('/engine/info')
  }

  async healthCheck(): Promise<ApiResponse<{ status: string; timestamp: string }>> {
    return this.request('/health')
  }

  // AI API methods
  async getAIStatus(): Promise<ApiResponse<{ configured: boolean; provider: string }>> {
    return this.request('/ai/status')
  }

  async chatWithAI(messages: any[], options?: any): Promise<ApiResponse> {
    return this.request('/ai/chat', {
      method: 'POST',
      body: JSON.stringify({ messages, options }),
    })
  }

  async analyzeCode(code: string, context?: any): Promise<ApiResponse<{ analysis: string }>> {
    return this.request('/ai/analyze', {
      method: 'POST',
      body: JSON.stringify({ code, context }),
    })
  }

  async saveAIConfig(config: any): Promise<ApiResponse> {
    return this.request('/ai/config', {
      method: 'POST',
      body: JSON.stringify(config),
    })
  }

  async testAIConfig(config: any): Promise<ApiResponse> {
    return this.request('/ai/config/test', {
      method: 'POST',
      body: JSON.stringify(config),
    })
  }

  async analyzeRequirements(requirements: string): Promise<ApiResponse> {
    return this.request('/pipeline/analyze', {
      method: 'POST',
      body: JSON.stringify({ requirements }),
    })
  }

  async generateDesign(requirements: string): Promise<ApiResponse> {
    return this.request('/pipeline/design', {
      method: 'POST',
      body: JSON.stringify({ requirements }),
    })
  }

  async generatePipelineCode(design: any, requirements: string): Promise<ApiResponse> {
    return this.request('/pipeline/generate-code', {
      method: 'POST',
      body: JSON.stringify({ design, requirements }),
    })
  }

  async savePipelineFiles(pipelineId: string, files: any[]): Promise<ApiResponse> {
    return this.request('/pipeline/save-files', {
      method: 'POST',
      body: JSON.stringify({ pipelineId, files }),
    })
  }
}

export const apiService = new ApiService()
