import { History } from 'lucide-react'
import { usePipelineStore } from '../../store/pipelineStore'

function HistoryPanel() {
  const history = usePipelineStore(state => state.history)

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
      </div>

      {/* Content */}
      <div className="flex-1 p-6 overflow-y-auto">
        <div className="max-w-4xl mx-auto">
          {history.length === 0 ? (
            <div className="text-center py-12">
              <History className="w-12 h-12 text-gray-600 mx-auto mb-4" />
              <p className="text-gray-400">暂无历史记录</p>
            </div>
          ) : (
            <div className="space-y-4">
              {history.map((item, index) => (
                <div key={index} className="bg-gray-800 rounded-lg p-4">
                  <div className="flex items-center justify-between mb-2">
                    <div className="font-medium text-white">{item.stage}</div>
                    <div className="text-sm text-gray-500">
                      {item.timestamp.toLocaleString()}
                    </div>
                  </div>
                  <div className="text-sm text-gray-400">{item.description}</div>
                </div>
              ))}
            </div>
          )}
        </div>
      </div>
    </div>
  )
}

export default HistoryPanel
