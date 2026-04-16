import { MessageSquare } from 'lucide-react'

function FeedbackPanel() {
  return (
    <div className="flex-1 flex items-center justify-center">
      <div className="text-center">
        <MessageSquare className="w-12 h-12 text-gray-600 mx-auto mb-4" />
        <p className="text-gray-400">测试反馈面板</p>
        <p className="text-sm text-gray-500 mt-2">即将实现</p>
      </div>
    </div>
  )
}

export default FeedbackPanel
