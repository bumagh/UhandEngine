import { History, FileText, Cpu, Code, Eye, Play, MessageSquare, ChevronLeft, ChevronRight } from 'lucide-react'
import { usePipelineStore } from '../../store/pipelineStore'
import { useState } from 'react'

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

const ITEMS_PER_PAGE = 10

function HistoryPanel() {
  const history = usePipelineStore(state => state.history)
  const pipelineId = usePipelineStore(state => state.pipelineId)
  const [currentPage, setCurrentPage] = useState(1)

  const totalPages = Math.ceil(history.length / ITEMS_PER_PAGE)
  const startIndex = (currentPage - 1) * ITEMS_PER_PAGE
  const endIndex = startIndex + ITEMS_PER_PAGE
  const currentHistory = history.slice(startIndex, endIndex)

  const handlePageChange = (page: number) => {
    setCurrentPage(page)
  }

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
            <>
              <div className="space-y-4">
                {currentHistory.map((item: any, index: number) => {
                  const Icon = stageIcons[item.stage] || History
                  return (
                    <div key={startIndex + index} className="bg-gray-800 rounded-lg p-4 border-l-4 border-primary-500">
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

              {/* Pagination */}
              {totalPages > 1 && (
                <div className="flex items-center justify-center gap-2 mt-6 pt-4 border-t border-gray-700">
                  <button
                    onClick={() => handlePageChange(currentPage - 1)}
                    disabled={currentPage === 1}
                    className="p-2 rounded-lg bg-gray-700 hover:bg-gray-600 disabled:opacity-50 disabled:cursor-not-allowed transition-colors"
                  >
                    <ChevronLeft className="w-4 h-4" />
                  </button>

                  {Array.from({ length: totalPages }, (_, i) => i + 1).map((page) => (
                    <button
                      key={page}
                      onClick={() => handlePageChange(page)}
                      className={`px-3 py-1 rounded-lg transition-colors ${
                        currentPage === page
                          ? 'bg-primary-600 text-white'
                          : 'bg-gray-700 hover:bg-gray-600'
                      }`}
                    >
                      {page}
                    </button>
                  ))}

                  <button
                    onClick={() => handlePageChange(currentPage + 1)}
                    disabled={currentPage === totalPages}
                    className="p-2 rounded-lg bg-gray-700 hover:bg-gray-600 disabled:opacity-50 disabled:cursor-not-allowed transition-colors"
                  >
                    <ChevronRight className="w-4 h-4" />
                  </button>
                </div>
              )}

              {/* Page info */}
              <div className="text-center text-sm text-gray-500 mt-2">
                第 {currentPage} 页 / 共 {totalPages} 页 ({history.length} 条记录)
              </div>
            </>
          )}
        </div>
      </div>
    </div>
  )
}

export default HistoryPanel
