import { create } from 'zustand'
import { apiService, FileNode, EngineInfo } from '../services/api'

interface AppState {
  // Project structure
  projectStructure: FileNode[] | null
  loading: boolean
  error: string | null
  
  // Current file
  currentFile: { path: string; content: string } | null
  
  // Engine info
  engineInfo: EngineInfo | null
  
  // Actions
  loadProjectStructure: () => Promise<void>
  loadFileContent: (path: string) => Promise<void>
  saveFileContent: (path: string, content: string) => Promise<void>
  loadEngineInfo: () => Promise<void>
  setLoading: (loading: boolean) => void
  setError: (error: string | null) => void
}

export const useStore = create<AppState>((set, get) => ({
  projectStructure: null,
  loading: false,
  error: null,
  currentFile: null,
  engineInfo: null,

  loadProjectStructure: async () => {
    set({ loading: true, error: null })
    try {
      console.log('Loading project structure...')
      const response = await apiService.getProjectStructure()
      console.log('Response:', response)
      if (response.success && response.data) {
        set({ projectStructure: response.data.structure, loading: false })
      } else {
        console.error('Failed to load project structure:', response.error)
        set({ error: response.error || 'Failed to load project structure', loading: false })
      }
    } catch (error) {
      console.error('Error loading project structure:', error)
      set({ error: error instanceof Error ? error.message : 'Unknown error', loading: false })
    }
  },

  loadFileContent: async (path: string) => {
    set({ loading: true, error: null })
    try {
      const response = await apiService.getFileContent(path)
      if (response.success && response.data) {
        set({ currentFile: { path, content: response.data.content }, loading: false })
      } else {
        set({ error: response.error || 'Failed to load file content', loading: false })
      }
    } catch (error) {
      set({ error: error instanceof Error ? error.message : 'Unknown error', loading: false })
    }
  },

  saveFileContent: async (path: string, content: string) => {
    set({ loading: true, error: null })
    try {
      const response = await apiService.saveFile(path, content)
      if (response.success) {
        set({ loading: false })
      } else {
        set({ error: response.error || 'Failed to save file', loading: false })
      }
    } catch (error) {
      set({ error: error instanceof Error ? error.message : 'Unknown error', loading: false })
    }
  },

  loadEngineInfo: async () => {
    set({ loading: true, error: null })
    try {
      const response = await apiService.getEngineInfo()
      if (response.success && response.data) {
        set({ engineInfo: response.data.info, loading: false })
      } else {
        set({ error: response.error || 'Failed to load engine info', loading: false })
      }
    } catch (error) {
      set({ error: error instanceof Error ? error.message : 'Unknown error', loading: false })
    }
  },

  setLoading: (loading: boolean) => set({ loading }),
  setError: (error: string | null) => set({ error }),
}))
