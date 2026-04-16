import { useState } from 'react'
import { Eye, Save, ArrowLeft } from 'lucide-react'
import { usePipelineStore } from '../../store/pipelineStore'

function PreviewPanel() {
  const generatedFiles = usePipelineStore(state => state.generatedFiles)
  const updateGeneratedFile = usePipelineStore(state => state.updateGeneratedFile)
  const setStage = usePipelineStore(state => state.setStage)
  const addHistory = usePipelineStore(state => state.addHistory)
  const [selectedFile, setSelectedFile] = useState<number | null>(null)
  const [editingContent, setEditingContent] = useState('')

  const handleFileSelect = (index: number) => {
    setSelectedFile(index)
    setEditingContent(generatedFiles[index].content)
  }

  const handleSaveEdit = () => {
    if (selectedFile !== null) {
      updateGeneratedFile(generatedFiles[selectedFile].path, editingContent)
      addHistory({
        id: Date.now().toString(),
        timestamp: new Date(),
        stage: 'preview',
        description: `编辑文件: ${generatedFiles[selectedFile].path}`,
        data: { file: generatedFiles[selectedFile].path }
      })
      alert('文件已更新')
    }
  }

  const handleBack = () => {
    setStage('code')
  }

  if (generatedFiles.length === 0) {
    return (
      <div className="flex-1 flex items-center justify-center">
        <div className="text-center">
          <Eye className="w-12 h-12 text-gray-600 mx-auto mb-4" />
          <p className="text-gray-400">没有可编辑的代码文件</p>
          <button
            onClick={handleBack}
            className="mt-4 px-4 py-2 bg-primary-600 text-white rounded-lg hover:bg-primary-700"
          >
            返回代码生成
          </button>
        </div>
      </div>
    )
  }

  return (
    <div className="flex-1 flex flex-col">
      {/* Header */}
      <div className="bg-gray-800 border-b border-gray-700 px-6 py-4 flex items-center justify-between">
        <div className="flex items-center gap-3">
          <button
            onClick={handleBack}
            className="p-2 hover:bg-gray-700 rounded-lg transition-colors"
          >
            <ArrowLeft className="w-5 h-5 text-gray-400" />
          </button>
          <div>
            <h2 className="text-lg font-semibold flex items-center gap-2">
              <Eye className="w-5 h-5 text-primary-400" />
              预览编辑
            </h2>
            <p className="text-sm text-gray-400 mt-1">
              查看和编辑生成的代码文件
            </p>
          </div>
        </div>
        {selectedFile !== null && (
          <button
            onClick={handleSaveEdit}
            className="flex items-center gap-2 px-4 py-2 bg-green-600 text-white rounded-lg hover:bg-green-700 transition-colors"
          >
            <Save className="w-4 h-4" />
            保存修改
          </button>
        )}
      </div>

      {/* Content */}
      <div className="flex-1 flex overflow-hidden">
        {/* File List */}
        <div className="w-64 bg-gray-800 border-r border-gray-700 p-4 overflow-y-auto">
          <h3 className="text-sm font-medium text-gray-400 mb-3">文件列表</h3>
          <div className="space-y-2">
            {generatedFiles.map((file, index) => (
              <button
                key={index}
                onClick={() => handleFileSelect(index)}
                className={`w-full text-left px-3 py-2 rounded-lg transition-colors ${
                  selectedFile === index
                    ? 'bg-primary-600 text-white'
                    : 'text-gray-300 hover:bg-gray-700'
                }`}
              >
                <div className="font-medium truncate">{file.path}</div>
                <div className="text-xs opacity-70">{file.type}</div>
              </button>
            ))}
          </div>
        </div>

        {/* Code Editor */}
        <div className="flex-1 flex flex-col">
          {selectedFile !== null ? (
            <>
              <div className="bg-gray-800 border-b border-gray-700 px-4 py-2">
                <span className="text-sm text-gray-400">
                  编辑: {generatedFiles[selectedFile].path}
                </span>
              </div>
              <textarea
                value={editingContent}
                onChange={(e) => setEditingContent(e.target.value)}
                className="flex-1 bg-gray-900 text-gray-300 p-4 font-mono text-sm resize-none focus:outline-none"
                spellCheck={false}
              />
            </>
          ) : (
            <div className="flex-1 flex items-center justify-center">
              <div className="text-center">
                <Eye className="w-12 h-12 text-gray-600 mx-auto mb-4" />
                <p className="text-gray-400">选择一个文件进行编辑</p>
              </div>
            </div>
          )}
        </div>
      </div>
    </div>
  )
}

export default PreviewPanel
