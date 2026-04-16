import { FileText, Cpu, Code, Eye, Play, MessageSquare, History, RotateCcw } from 'lucide-react'
import { usePipelineStore } from '../../store/pipelineStore'

const stages = [
  { id: 'requirements', label: '需求描述', icon: FileText },
  { id: 'design', label: 'AI 设计', icon: Cpu },
  { id: 'code', label: '代码生成', icon: Code },
  { id: 'preview', label: '预览编辑', icon: Eye },
  { id: 'build', label: '编译运行', icon: Play },
  { id: 'feedback', label: '测试反馈', icon: MessageSquare },
  { id: 'history', label: '历史记录', icon: History },
]

function PipelineSidebar() {
  const currentStage = usePipelineStore(state => state.currentStage)
  const setStage = usePipelineStore(state => state.setStage)
  const reset = usePipelineStore(state => state.reset)

  const handleReset = () => {
    if (confirm('确定要重置所有进度吗？这将清除所有已保存的数据。')) {
      reset()
      localStorage.removeItem('pipeline-storage')
    }
  }

  return (
    <div className="w-64 bg-gray-800 border-r border-gray-700 flex flex-col">
      <div className="p-4 border-b border-gray-700">
        <h2 className="text-lg font-semibold text-white">游戏开发流水线</h2>
        <p className="text-sm text-gray-400 mt-1">AI 驱动的游戏开发</p>
      </div>

      <div className="flex-1 p-4 space-y-2 overflow-y-auto">
        {stages.map((stage) => {
          const Icon = stage.icon
          const isActive = currentStage === stage.id

          return (
            <button
              key={stage.id}
              onClick={() => setStage(stage.id as any)}
              className={`w-full flex items-center gap-3 px-4 py-3 rounded-lg transition-colors ${
                isActive
                  ? 'bg-primary-600 text-white'
                  : 'text-gray-300 hover:bg-gray-700'
              }`}
            >
              <Icon className="w-5 h-5" />
              <span className="font-medium">{stage.label}</span>
            </button>
          )
        })}
      </div>

      <div className="p-4 border-t border-gray-700 space-y-3">
        <div className="text-xs text-gray-500">
          <div className="flex items-center justify-between mb-2">
            <span>当前阶段</span>
            <span className="text-primary-400">
              {stages.find(s => s.id === currentStage)?.label}
            </span>
          </div>
        </div>

        <button
          onClick={handleReset}
          className="w-full flex items-center justify-center gap-2 px-4 py-2 bg-gray-700 text-gray-300 rounded-lg hover:bg-gray-600 transition-colors text-sm"
        >
          <RotateCcw className="w-4 h-4" />
          重置进度
        </button>
      </div>
    </div>
  )
}

export default PipelineSidebar
