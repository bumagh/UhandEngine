import { useState, useEffect } from 'react'
import { Cpu, CheckCircle, RefreshCw, ArrowRight } from 'lucide-react'
import { usePipelineStore } from '../../store/pipelineStore'
import { apiService } from '../../services/api'

function DesignPanel() {
  const [loading, setLoading] = useState(false)
  const design = usePipelineStore(state => state.design)
  const requirements = usePipelineStore(state => state.requirements)
  const setDesign = usePipelineStore(state => state.setDesign)
  const setStage = usePipelineStore(state => state.setStage)

  useEffect(() => {
    if (!design && requirements) {
      generateDesign()
    }
  }, [])

  const generateDesign = async () => {
    setLoading(true)
    
    try {
      const response = await apiService.generateDesign(requirements)
      
      if (response.success && (response as any).design) {
        setDesign((response as any).design)
      } else {
        throw new Error(response.error || 'Failed to generate design')
      }
    } catch (error) {
      console.error('Design generation error:', error)
      const errorMessage = error instanceof Error ? error.message : '未知错误'
      
      if (errorMessage.includes('429') || errorMessage.includes('rate limit')) {
        alert('API 请求过于频繁，请稍等片刻后重试')
      } else if (errorMessage.includes('JSON') || errorMessage.includes('parse')) {
        alert('AI 返回格式错误，请重试或联系管理员')
      } else {
        alert('设计生成失败：' + errorMessage)
      }
    } finally {
      setLoading(false)
    }
  }

  const handleConfirm = () => {
    setStage('code')
  }

  const handleRegenerate = () => {
    generateDesign()
  }

  if (loading) {
    return (
      <div className="flex-1 flex items-center justify-center">
        <div className="text-center">
          <RefreshCw className="w-12 h-12 text-primary-400 animate-spin mx-auto mb-4" />
          <p className="text-gray-400">AI 正在生成游戏设计...</p>
        </div>
      </div>
    )
  }

  if (!design) {
    return (
      <div className="flex-1 flex items-center justify-center">
        <div className="text-center">
          <Cpu className="w-12 h-12 text-gray-600 mx-auto mb-4" />
          <p className="text-gray-400 mb-4">暂无设计</p>
          <button
            onClick={generateDesign}
            className="px-4 py-2 bg-primary-600 text-white rounded-lg hover:bg-primary-700"
          >
            生成设计
          </button>
        </div>
      </div>
    )
  }

  return (
    <div className="flex-1 flex flex-col">
      {/* Header */}
      <div className="bg-gray-800 border-b border-gray-700 px-6 py-4">
        <h2 className="text-lg font-semibold flex items-center gap-2">
          <Cpu className="w-5 h-5 text-primary-400" />
          AI 设计
        </h2>
        <p className="text-sm text-gray-400 mt-1">
          查看并确认 AI 生成的游戏设计
        </p>
      </div>

      {/* Content */}
      <div className="flex-1 p-6 overflow-y-auto">
        <div className="max-w-4xl mx-auto space-y-6">
          {/* Game Type */}
          <div className="bg-gray-800 rounded-lg p-6">
            <h3 className="text-lg font-medium text-white mb-3">游戏类型</h3>
            <p className="text-gray-300">{design.game_type}</p>
          </div>

          {/* Scenes */}
          <div className="bg-gray-800 rounded-lg p-6">
            <h3 className="text-lg font-medium text-white mb-3">场景设计</h3>
            <div className="space-y-3">
              {design.scenes.map((scene, index) => (
                <div key={index} className="bg-gray-700 rounded-lg p-4">
                  <div className="font-medium text-white mb-2">{scene.name}</div>
                  <div className="text-sm text-gray-400 mb-2">{scene.description}</div>
                  <div className="text-sm text-gray-500">
                    对象: {scene.objects.join(', ')}
                  </div>
                </div>
              ))}
            </div>
          </div>

          {/* Components */}
          <div className="bg-gray-800 rounded-lg p-6">
            <h3 className="text-lg font-medium text-white mb-3">组件系统</h3>
            <div className="flex flex-wrap gap-2">
              {design.components.map((component, index) => (
                <span
                  key={index}
                  className="px-3 py-1 bg-primary-600/20 text-primary-400 rounded-full text-sm"
                >
                  {component}
                </span>
              ))}
            </div>
          </div>

          {/* Features */}
          <div className="bg-gray-800 rounded-lg p-6">
            <h3 className="text-lg font-medium text-white mb-3">功能特性</h3>
            <ul className="space-y-2">
              {design.features.map((feature, index) => (
                <li key={index} className="flex items-start gap-2 text-gray-300">
                  <CheckCircle className="w-5 h-5 text-green-500 flex-shrink-0 mt-0.5" />
                  {feature}
                </li>
              ))}
            </ul>
          </div>

          {/* Actions */}
          <div className="flex gap-4">
            <button
              onClick={handleRegenerate}
              className="flex-1 flex items-center justify-center gap-2 px-6 py-3 bg-gray-700 text-white rounded-lg hover:bg-gray-600 transition-colors"
            >
              <RefreshCw className="w-5 h-5" />
              重新生成
            </button>
            <button
              onClick={handleConfirm}
              className="flex-1 flex items-center justify-center gap-2 px-6 py-3 bg-primary-600 text-white rounded-lg hover:bg-primary-700 transition-colors"
            >
              确认设计
              <ArrowRight className="w-5 h-5" />
            </button>
          </div>
        </div>
      </div>
    </div>
  )
}

export default DesignPanel
