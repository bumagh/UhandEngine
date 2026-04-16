import { useState } from 'react'
import { MessageSquare, ArrowLeft, Send, CheckCircle, XCircle, AlertCircle } from 'lucide-react'
import { usePipelineStore } from '../../store/pipelineStore'

function FeedbackPanel() {
  const feedback = usePipelineStore(state => state.feedback)
  const addFeedback = usePipelineStore(state => state.addFeedback)
  const addHistory = usePipelineStore(state => state.addHistory)
  const setStage = usePipelineStore(state => state.setStage)
  const [feedbackText, setFeedbackText] = useState('')
  const [selectedRating, setSelectedRating] = useState<'good' | 'bad' | 'issue' | null>(null)

  const handleSubmitFeedback = () => {
    if (!feedbackText.trim()) {
      alert('请输入反馈内容')
      return
    }

    if (!selectedRating) {
      alert('请选择反馈类型')
      return
    }

    const newFeedback = {
      id: Date.now().toString(),
      timestamp: new Date(),
      rating: selectedRating,
      content: feedbackText
    }

    addFeedback(newFeedback)
    addHistory({
      id: Date.now().toString(),
      timestamp: new Date(),
      stage: 'feedback',
      description: `提交反馈: ${selectedRating}`,
      data: { rating: selectedRating, content: feedbackText }
    })
    setFeedbackText('')
    setSelectedRating(null)
    alert('反馈已提交')
  }

  const handleBack = () => {
    setStage('build')
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
              <MessageSquare className="w-5 h-5 text-primary-400" />
              测试反馈
            </h2>
            <p className="text-sm text-gray-400 mt-1">
              提供测试结果和反馈
            </p>
          </div>
        </div>
      </div>

      {/* Content */}
      <div className="flex-1 flex overflow-hidden">
        {/* Feedback Form */}
        <div className="w-1/2 p-6 overflow-y-auto">
          <div className="bg-gray-800 rounded-lg p-6">
            <h3 className="text-lg font-medium text-white mb-4">提交反馈</h3>

            {/* Rating Selection */}
            <div className="mb-4">
              <label className="block text-sm text-gray-400 mb-2">反馈类型</label>
              <div className="flex gap-3">
                <button
                  onClick={() => setSelectedRating('good')}
                  className={`flex-1 flex items-center justify-center gap-2 px-4 py-3 rounded-lg transition-colors ${
                    selectedRating === 'good'
                      ? 'bg-green-600 text-white'
                      : 'bg-gray-700 text-gray-300 hover:bg-gray-600'
                  }`}
                >
                  <CheckCircle className="w-5 h-5" />
                  正常运行
                </button>
                <button
                  onClick={() => setSelectedRating('bad')}
                  className={`flex-1 flex items-center justify-center gap-2 px-4 py-3 rounded-lg transition-colors ${
                    selectedRating === 'bad'
                      ? 'bg-red-600 text-white'
                      : 'bg-gray-700 text-gray-300 hover:bg-gray-600'
                  }`}
                >
                  <XCircle className="w-5 h-5" />
                  运行错误
                </button>
                <button
                  onClick={() => setSelectedRating('issue')}
                  className={`flex-1 flex items-center justify-center gap-2 px-4 py-3 rounded-lg transition-colors ${
                    selectedRating === 'issue'
                      ? 'bg-yellow-600 text-white'
                      : 'bg-gray-700 text-gray-300 hover:bg-gray-600'
                  }`}
                >
                  <AlertCircle className="w-5 h-5" />
                  其他问题
                </button>
              </div>
            </div>

            {/* Feedback Text */}
            <div className="mb-4">
              <label className="block text-sm text-gray-400 mb-2">反馈内容</label>
              <textarea
                value={feedbackText}
                onChange={(e) => setFeedbackText(e.target.value)}
                placeholder="描述测试结果、遇到的问题或建议..."
                className="w-full h-40 bg-gray-900 text-gray-300 p-4 rounded-lg resize-none focus:outline-none focus:ring-2 focus:ring-primary-500"
              />
            </div>

            {/* Submit Button */}
            <button
              onClick={handleSubmitFeedback}
              className="w-full flex items-center justify-center gap-2 px-4 py-3 bg-primary-600 text-white rounded-lg hover:bg-primary-700 transition-colors"
            >
              <Send className="w-4 h-4" />
              提交反馈
            </button>
          </div>
        </div>

        {/* Feedback History */}
        <div className="w-1/2 p-6 overflow-y-auto border-l border-gray-700">
          <h3 className="text-lg font-medium text-white mb-4">反馈历史</h3>
          <div className="space-y-3">
            {feedback.length === 0 ? (
              <div className="text-center py-12">
                <MessageSquare className="w-12 h-12 text-gray-600 mx-auto mb-4" />
                <p className="text-gray-400">暂无反馈记录</p>
              </div>
            ) : (
              feedback.map((item: any) => (
                <div key={item.id} className="bg-gray-800 rounded-lg p-4">
                  <div className="flex items-center justify-between mb-2">
                    <div className="flex items-center gap-2">
                      {item.rating === 'good' && <CheckCircle className="w-4 h-4 text-green-500" />}
                      {item.rating === 'bad' && <XCircle className="w-4 h-4 text-red-500" />}
                      {item.rating === 'issue' && <AlertCircle className="w-4 h-4 text-yellow-500" />}
                      <span className="text-sm text-gray-400">
                        {new Date(item.timestamp).toLocaleString('zh-CN')}
                      </span>
                    </div>
                  </div>
                  <p className="text-gray-300 text-sm">{item.content}</p>
                </div>
              ))
            )}
          </div>
        </div>
      </div>
    </div>
  )
}

export default FeedbackPanel
