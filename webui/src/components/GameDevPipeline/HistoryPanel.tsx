import { History, FileText, Cpu, Code, Eye, Play, MessageSquare } from 'lucide-react'
import { usePipelineStore } from '../../store/pipelineStore'

const stageIcons: { [key: string]: any } = {
  requirements: FileText,
  design: Cpu,
  code: Code,
  preview: Eye,
  build: Play,
  feedback: MessageSquare,
}

const stageNames: { [key: string]: string } = {
  requirements: '需求描述',
  design: 'AI 设计',
  code: '代码生成',
  preview: '预览编辑',
  build: '编译运行',
  feedback: '测试反馈',
}

function HistoryPanel() {
  const history = usePipelineStore(state => state.history)
  const pipelineId = usePipelineStore(state => state.pipelineId)

  return (
    <div className="flex-1 flex flex-col">
      {/* Header */}
      <div className="bg-gray-800 border-b border-gray-700 px-6 py-4">
        <h2 className="text-lg font-semibold flex items-center gap-2">
          <History className="w-5 h-5 text-primary-400" />
          历史记录
        </h2>
        <p className="text-sm text-gray-400 mt-1">
          查看开发历史和版本
        </p>
        {pipelineId && (
          <p className="text-xs text-gray-500 mt-1">
            Pipeline ID: {pipelineId}
          </p>
        )}
      </div>

      {/* Content */}
      <div className="flex-1 p-6 overflow-y-auto">
        <div className="max-w-4xl mx-auto">
          {history.length === 0 ? (
            <div className="text-center py-12">
              <History className="w-12 h-12 text-gray-600 mx-auto mb-4" />
              <p className="text-gray-400">暂无历史记录</p>
              <p className="text-sm text-gray-500 mt-2">
                完成各个阶段后将自动记录操作历史
              </p>
            </div>
          ) : (
            <div className="space-y-4">
              {history.map((item: any, index: number) => {
                const Icon = stageIcons[item.stage] || History
                return (
                  <div key={index} className="bg-gray-800 rounded-lg p-4 border-l-4 border-primary-500">
                    <div className="flex items-start gap-4">
                      <div className="flex-shrink-0">
                        <div className="w-10 h-10 rounded-full bg-primary-600/20 flex items-center justify-center">
                          <Icon className="w-5 h-5 text-primary-400" />
                        </div>
                      </div>
                      <div className="flex-1">
                        <div className="flex items-center justify-between mb-2">
                          <div className="font-medium text-white">
                            {stageNames[item.stage] || item.stage}
                          </div>
                          <div className="text-sm text-gray-500">
                            {new Date(item.timestamp).toLocaleString('zh-CN')}
                          </div>
                        </div>
                        <div className="text-sm text-gray-400">{item.description}</div>
                        {item.data && (
                          <div className="mt-2 text-xs text-gray-500">
                            {typeof item.data === 'object' ? JSON.stringify(item.data) : item.data}
                          </div>
                        )}
                      </div>
                    </div>
                  </div>
                )
              })}
            </div>
          )}
        </div>
      </div>
    </div>
  )
}

export default HistoryPanel
