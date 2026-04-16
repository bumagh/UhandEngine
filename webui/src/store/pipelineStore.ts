import { create } from 'zustand'

export interface GameDesign {
  game_type: string
  scenes: SceneDesign[]
  components: string[]
  features: string[]
}

export interface SceneDesign {
  name: string
  objects: string[]
  description: string
}

export interface GeneratedFile {
  path: string
  content: string
  type: 'scene' | 'object' | 'component' | 'main'
}

export interface BuildStatus {
  status: 'idle' | 'compiling' | 'success' | 'error'
  logs: string[]
  error?: string
}

export interface RunStatus {
  status: 'idle' | 'running' | 'stopped' | 'error'
  pid?: number
  error?: string
}

export interface Feedback {
  id: string
  timestamp: Date
  testResults: string
  issues: string[]
  suggestions: string[]
}

export interface PipelineHistory {
  id: string
  timestamp: Date
  stage: string
  description: string
  data: any
}

export interface PipelineState {
  currentStage: 'requirements' | 'design' | 'code' | 'preview' | 'build' | 'feedback' | 'history'
  requirements: string
  design: GameDesign | null
  generatedFiles: GeneratedFile[]
  buildStatus: BuildStatus
  runStatus: RunStatus
  feedback: Feedback[]
  history: PipelineHistory[]
  
  // Actions
  setStage: (stage: PipelineState['currentStage']) => void
  setRequirements: (requirements: string) => void
  setDesign: (design: GameDesign) => void
  addGeneratedFile: (file: GeneratedFile) => void
  updateGeneratedFile: (path: string, content: string) => void
  setBuildStatus: (status: BuildStatus) => void
  setRunStatus: (status: RunStatus) => void
  addFeedback: (feedback: Feedback) => void
  addHistory: (history: PipelineHistory) => void
  reset: () => void
}

const initialState: PipelineState = {
  currentStage: 'requirements',
  requirements: '',
  design: null,
  generatedFiles: [],
  buildStatus: { status: 'idle', logs: [] },
  runStatus: { status: 'idle' },
  feedback: [],
  history: [],
  
  setStage: () => {},
  setRequirements: () => {},
  setDesign: () => {},
  addGeneratedFile: () => {},
  updateGeneratedFile: () => {},
  setBuildStatus: () => {},
  setRunStatus: () => {},
  addFeedback: () => {},
  addHistory: () => {},
  reset: () => {},
}

export const usePipelineStore = create<PipelineState>((set) => ({
  ...initialState,
  
  setStage: (stage) => set({ currentStage: stage }),
  
  setRequirements: (requirements) => set({ requirements }),
  
  setDesign: (design) => set({ design }),
  
  addGeneratedFile: (file) => set((state) => ({
    generatedFiles: [...state.generatedFiles, file]
  })),
  
  updateGeneratedFile: (path, content) => set((state) => ({
    generatedFiles: state.generatedFiles.map(f => 
      f.path === path ? { ...f, content } : f
    )
  })),
  
  setBuildStatus: (buildStatus) => set({ buildStatus }),
  
  setRunStatus: (runStatus) => set({ runStatus }),
  
  addFeedback: (feedback) => set((state) => ({
    feedback: [...state.feedback, feedback]
  })),
  
  addHistory: (history) => set((state) => ({
    history: [...state.history, history]
  })),
  
  reset: () => set(initialState),
}))
