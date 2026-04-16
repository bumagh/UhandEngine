import { useState } from 'react'
import { Play, ArrowLeft, RefreshCw, Terminal } from 'lucide-react'
import { usePipelineStore } from '../../store/pipelineStore'
import { apiService } from '../../services/api'

function BuildPanel() {
  const generatedFiles = usePipelineStore(state => state.generatedFiles)
  const pipelineId = usePipelineStore(state => state.pipelineId)
  const setStage = usePipelineStore(state => state.setStage)
  const addHistory = usePipelineStore(state => state.addHistory)
  const [building, setBuilding] = useState(false)
  const [buildOutput, setBuildOutput] = useState('')
  const [runOutput, setRunOutput] = useState('')
  const [status, setStatus] = useState<'idle' | 'building' | 'running' | 'success' | 'error'>('idle')

  const handleBuildAndRun = async () => {
    if (generatedFiles.length === 0) {
      alert('没有可编译的代码文件')
      return
    }

    if (!pipelineId) {
      alert('Pipeline ID 不存在，请刷新页面重试')
      return
    }

    setBuilding(true)
    setStatus('building')
    setBuildOutput('')
    setRunOutput('')

    try {
      const response = await apiService.compileAndRunPipeline(pipelineId, generatedFiles)

      if (response.success) {
        setBuildOutput(response.buildOutput || 'Build completed')
        setRunOutput(response.runOutput || 'Run completed')
        setStatus('success')
        addHistory({
          id: Date.now().toString(),
          timestamp: new Date(),
          stage: 'build',
          description: '编译运行成功',
          data: { status: response.status }
        })
      } else {
        setBuildOutput(response.output || 'Build failed')
        setStatus('error')
      }
    } catch (error) {
      console.error('Build and run error:', error)
      const errorMessage = error instanceof Error ? error.message : '未知错误'
      setBuildOutput('编译运行失败：' + errorMessage)
      setStatus('error')
    } finally {
      setBuilding(false)
    }
  }

  const handleBack = () => {
    setStage('preview')
  }

  if (generatedFiles.length === 0) {
    return (
      <div className="flex-1 flex items-center justify-center">
        <div className="text-center">
          <Play className="w-12 h-12 text-gray-600 mx-auto mb-4" />
          <p className="text-gray-400">没有可编译的代码文件</p>
          <button
            onClick={handleBack}
            className="mt-4 px-4 py-2 bg-primary-600 text-white rounded-lg hover:bg-primary-700"
          >
            返回预览编辑
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
              <Play className="w-5 h-5 text-primary-400" />
              编译运行
            </h2>
            <p className="text-sm text-gray-400 mt-1">
              编译并运行生成的游戏代码
            </p>
          </div>
        </div>
        <button
          onClick={handleBuildAndRun}
          disabled={building}
          className="flex items-center gap-2 px-4 py-2 bg-green-600 text-white rounded-lg hover:bg-green-700 transition-colors disabled:opacity-50 disabled:cursor-not-allowed"
        >
          {building ? (
            <>
              <RefreshCw className="w-4 h-4 animate-spin" />
              编译中...
            </>
          ) : (
            <>
              <Play className="w-4 h-4" />
              编译并运行
            </>
          )}
        </button>
      </div>

      {/* Content */}
      <div className="flex-1 flex overflow-hidden">
        {/* Build Output */}
        <div className="flex-1 flex flex-col border-r border-gray-700">
          <div className="bg-gray-800 border-b border-gray-700 px-4 py-2 flex items-center gap-2">
            <Terminal className="w-4 h-4 text-gray-400" />
            <span className="text-sm font-medium text-gray-400">编译输出</span>
            <span className={`ml-auto text-xs px-2 py-1 rounded ${
              status === 'success' ? 'bg-green-600/20 text-green-400' :
              status === 'error' ? 'bg-red-600/20 text-red-400' :
              status === 'building' ? 'bg-blue-600/20 text-blue-400' :
              'bg-gray-700 text-gray-400'
            }`}>
              {status === 'idle' ? '未编译' :
               status === 'building' ? '编译中' :
               status === 'success' ? '成功' :
               status === 'error' ? '失败' : '运行中'}
            </span>
          </div>
          <pre className="flex-1 bg-gray-900 text-gray-300 p-4 text-sm overflow-auto font-mono">
            {buildOutput || '点击"编译并运行"开始编译'}
          </pre>
        </div>

        {/* Run Output */}
        <div className="flex-1 flex flex-col">
          <div className="bg-gray-800 border-b border-gray-700 px-4 py-2 flex items-center gap-2">
            <Terminal className="w-4 h-4 text-gray-400" />
            <span className="text-sm font-medium text-gray-400">运行输出</span>
          </div>
          <pre className="flex-1 bg-gray-900 text-gray-300 p-4 text-sm overflow-auto font-mono">
            {runOutput || '运行结果将显示在这里'}
          </pre>
        </div>
      </div>
    </div>
  )
}

export default BuildPanel
