import { useState, useEffect } from 'react'
import { Code, Play, RefreshCw } from 'lucide-react'
import { usePipelineStore } from '../../store/pipelineStore'
import { apiService } from '../../services/api'

function CodePanel() {
  const [loading, setLoading] = useState(false)
  const generatedFiles = usePipelineStore(state => state.generatedFiles)
  const design = usePipelineStore(state => state.design)
  const requirements = usePipelineStore(state => state.requirements)
  const addGeneratedFile = usePipelineStore(state => state.addGeneratedFile)
  const clearGeneratedFiles = usePipelineStore(state => state.clearGeneratedFiles)
  const setStage = usePipelineStore(state => state.setStage)

  useEffect(() => {
    if (generatedFiles.length === 0 && design) {
      generateCode()
    }
  }, [design])

  const generateCode = async () => {
    if (!design) return

    setLoading(true)

    try {
      // Clear existing files before generating new ones
      clearGeneratedFiles()

      const response = await apiService.generatePipelineCode(design, requirements)

      if (response.success && (response as any).files) {
        const files = (response as any).files
        files.forEach((file: any) => {
          addGeneratedFile(file)
        })
      } else {
        throw new Error(response.error || 'Failed to generate code')
      }
    } catch (error) {
      console.error('Code generation error:', error)
      const errorMessage = error instanceof Error ? error.message : '未知错误'

      if (errorMessage.includes('429') || errorMessage.includes('rate limit')) {
        alert('API 请求过于频繁，请稍等片刻后重试')
      } else if (errorMessage.includes('JSON') || errorMessage.includes('parse')) {
        alert('AI 返回格式错误，请重试或联系管理员')
      } else {
        alert('代码生成失败：' + errorMessage)
      }
    } finally {
      setLoading(false)
    }
  }

  const handleRegenerate = () => {
    generateCode()
  }

  if (loading) {
    return (
      <div className="flex-1 flex items-center justify-center">
        <div className="text-center">
          <RefreshCw className="w-12 h-12 text-primary-400 animate-spin mx-auto mb-4" />
          <p className="text-gray-400">AI 正在生成游戏代码...</p>
        </div>
      </div>
    )
  }

  return (
    <div className="flex-1 flex flex-col">
      {/* Header */}
      <div className="bg-gray-800 border-b border-gray-700 px-6 py-4">
        <h2 className="text-lg font-semibold flex items-center gap-2">
          <Code className="w-5 h-5 text-primary-400" />
          代码生成
        </h2>
        <p className="text-sm text-gray-400 mt-1">
          查看 AI 生成的游戏代码
        </p>
      </div>

      {/* Content */}
      <div className="flex-1 p-6 overflow-y-auto">
        <div className="max-w-4xl mx-auto">
          {generatedFiles.length === 0 ? (
            <div className="text-center py-12">
              <Code className="w-12 h-12 text-gray-600 mx-auto mb-4" />
              <p className="text-gray-400 mb-4">暂无生成的代码</p>
              <button
                onClick={generateCode}
                className="px-4 py-2 bg-primary-600 text-white rounded-lg hover:bg-primary-700"
              >
                生成代码
              </button>
            </div>
          ) : (
            <div className="space-y-4">
              {generatedFiles.map((file, index) => (
                <div key={index} className="bg-gray-800 rounded-lg p-4">
                  <div className="flex items-center justify-between mb-2">
                    <div className="font-medium text-white">{file.path}</div>
                    <span className="px-2 py-1 bg-gray-700 text-gray-300 rounded text-xs">
                      {file.type}
                    </span>
                  </div>
                  <pre className="bg-gray-900 rounded p-4 text-sm text-gray-300 overflow-x-auto max-h-64">
                    {file.content}
                  </pre>
                </div>
              ))}

              <div className="flex gap-4">
                <button
                  onClick={handleRegenerate}
                  className="flex-1 flex items-center justify-center gap-2 px-6 py-3 bg-gray-700 text-white rounded-lg hover:bg-gray-600 transition-colors"
                >
                  <RefreshCw className="w-5 h-5" />
                  重新生成
                </button>
                <button
                  onClick={() => setStage('preview')}
                  className="flex-1 flex items-center justify-center gap-2 px-6 py-3 bg-primary-600 text-white rounded-lg hover:bg-primary-700 transition-colors"
                >
                  <Play className="w-5 h-5" />
                  继续到预览编辑
                </button>
              </div>
            </div>
          )}
        </div>
      </div>
    </div>
  )
}

export default CodePanel
