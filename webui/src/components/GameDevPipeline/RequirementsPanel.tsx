import { useState } from 'react'
import { Sparkles, FileText, Lightbulb } from 'lucide-react'
import { apiService } from '../../services/api'
import { usePipelineStore } from '../../store/pipelineStore'

const templates = [
  {
    name: '平台跳跃游戏',
    description: '创建一个简单的平台跳跃游戏',
    content: `创建一个简单的平台跳跃游戏：
- 玩家可以左右移动和跳跃
- 有平台可以站立
- 简单的碰撞检测
- 计分系统`
  },
  {
    name: '射击游戏',
    description: '创建一个简单的射击游戏',
    content: `创建一个简单的射击游戏：
- 玩家可以移动和射击
- 敌人会从屏幕上方出现
- 简单的碰撞检测
- 生命值和得分系统`
  },
  {
    name: '益智游戏',
    description: '创建一个简单的益智游戏',
    content: `创建一个简单的益智游戏：
- 玩家需要移动方块到目标位置
- 有障碍物需要避开
- 计步数和计时
- 多个关卡`
  }
]

function RequirementsPanel() {
  const [input, setInput] = useState('')
  const [loading, setLoading] = useState(false)
  const [showTemplates, setShowTemplates] = useState(false)
  
  const requirements = usePipelineStore(state => state.requirements)
  const setRequirements = usePipelineStore(state => state.setRequirements)
  const setStage = usePipelineStore(state => state.setStage)

  const handleUseTemplate = (content: string) => {
    setInput(content)
    setShowTemplates(false)
  }

  const handleAnalyze = async () => {
    if (!input.trim()) return
    
    setLoading(true)
    setRequirements(input)
    
    try {
      const response = await apiService.analyzeRequirements(input)
      
      if (response.success) {
        setStage('design')
      } else {
        throw new Error(response.error || 'Failed to analyze requirements')
      }
    } catch (error) {
      console.error('Analysis error:', error)
      const errorMessage = error instanceof Error ? error.message : '未知错误'
      if (errorMessage.includes('AI not configured')) {
        alert('请先在 Settings 中配置 AI')
      } else {
        alert('分析失败：' + errorMessage)
      }
    } finally {
      setLoading(false)
    }
  }

  return (
    <div className="flex-1 flex flex-col">
      {/* Header */}
      <div className="bg-gray-800 border-b border-gray-700 px-6 py-4">
        <h2 className="text-lg font-semibold flex items-center gap-2">
          <FileText className="w-5 h-5 text-primary-400" />
          需求描述
        </h2>
        <p className="text-sm text-gray-400 mt-1">
          用自然语言描述你想要的游戏，AI 将为你生成设计和代码
        </p>
      </div>

      {/* Content */}
      <div className="flex-1 p-6 overflow-y-auto">
        <div className="max-w-3xl mx-auto space-y-6">
          {/* Templates */}
          <div>
            <button
              onClick={() => setShowTemplates(!showTemplates)}
              className="flex items-center gap-2 text-sm text-primary-400 hover:text-primary-300 mb-3"
            >
              <Lightbulb className="w-4 h-4" />
              使用示例模板
            </button>
            
            {showTemplates && (
              <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                {templates.map((template) => (
                  <button
                    key={template.name}
                    onClick={() => handleUseTemplate(template.content)}
                    className="bg-gray-800 border border-gray-700 rounded-lg p-4 text-left hover:border-primary-600 transition-colors"
                  >
                    <div className="font-medium text-white mb-1">{template.name}</div>
                    <div className="text-sm text-gray-400">{template.description}</div>
                  </button>
                ))}
              </div>
            )}
          </div>

          {/* Input */}
          <div>
            <label className="block text-sm font-medium text-gray-300 mb-2">
              游戏需求描述
            </label>
            <textarea
              value={input}
              onChange={(e) => setInput(e.target.value)}
              placeholder="描述你想要创建的游戏..."
              className="w-full h-64 bg-gray-800 text-white rounded-lg px-4 py-3 focus:outline-none focus:ring-2 focus:ring-primary-500 resize-none"
            />
          </div>

          {/* Action */}
          <button
            onClick={handleAnalyze}
            disabled={loading || !input.trim()}
            className={`w-full flex items-center justify-center gap-2 px-6 py-3 rounded-lg transition-colors ${
              loading || !input.trim()
                ? 'bg-gray-700 text-gray-400 cursor-not-allowed'
                : 'bg-primary-600 hover:bg-primary-700 text-white'
            }`}
          >
            {loading ? (
              <>分析中...</>
            ) : (
              <>
                <Sparkles className="w-5 h-5" />
                开始 AI 分析
              </>
            )}
          </button>

          {/* Saved Requirements */}
          {requirements && (
            <div className="bg-gray-800 border border-gray-700 rounded-lg p-4">
              <div className="text-sm font-medium text-gray-300 mb-2">已保存的需求</div>
              <div className="text-sm text-gray-400 whitespace-pre-wrap">{requirements}</div>
            </div>
          )}
        </div>
      </div>
    </div>
  )
}

export default RequirementsPanel
