import { Code, Download, Play } from 'lucide-react'
import { usePipelineStore } from '../../store/pipelineStore'

function CodePanel() {
  const generatedFiles = usePipelineStore(state => state.generatedFiles)
  const setStage = usePipelineStore(state => state.setStage)

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
              <p className="text-sm text-gray-500">请先确认设计，然后 AI 将生成代码</p>
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
              
              <button
                onClick={() => setStage('preview')}
                className="w-full flex items-center justify-center gap-2 px-6 py-3 bg-primary-600 text-white rounded-lg hover:bg-primary-700 transition-colors"
              >
                <Play className="w-5 h-5" />
                继续到预览编辑
              </button>
            </div>
          )}
        </div>
      </div>
    </div>
  )
}

export default CodePanel
