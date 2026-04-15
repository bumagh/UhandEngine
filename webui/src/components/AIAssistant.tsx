import { useState, useEffect } from 'react'
import { MessageSquare, Send, Loader2, AlertCircle, User, Bot } from 'lucide-react'
import { apiService } from '../services/api'
import { useStore } from '../store/useStore'

interface Message {
  id: string
  role: 'user' | 'assistant'
  content: string
  timestamp: Date
}

function AIAssistant() {
  const [messages, setMessages] = useState<Message[]>([
    {
      id: '1',
      role: 'assistant',
      content: 'Hello! I am the UhandEngine AI Assistant. I can help you with:\n\n• Creating game scenes\n• Writing game logic\n• Debugging issues\n• Explaining engine concepts\n\nHow can I help you today?',
      timestamp: new Date()
    }
  ])
  const [input, setInput] = useState('')
  const [loading, setLoading] = useState(false)
  const [aiConfigured, setAiConfigured] = useState(false)
  const [error, setError] = useState<string | null>(null)
  const { currentFile, projectStructure, engineInfo } = useStore()

  useEffect(() => {
    checkAIStatus()
  }, [])

  const checkAIStatus = async () => {
    try {
      const response = await apiService.getAIStatus()
      if (response.success && (response as any).configured !== undefined) {
        setAiConfigured((response as any).configured)
        if (!(response as any).configured) {
          setError('AI not configured. Please set OPENAI_API_KEY in backend/.env')
        }
      }
    } catch (err) {
      console.error('Failed to check AI status:', err)
      setError('Failed to connect to AI service')
    }
  }

  const handleSend = async () => {
    if (!input.trim()) return

    const userMessage: Message = {
      id: Date.now().toString(),
      role: 'user',
      content: input,
      timestamp: new Date()
    }

    setMessages(prev => [...prev, userMessage])
    setInput('')
    setLoading(true)
    setError(null)

    try {
      const context: any = {}
      if (currentFile) {
        context.currentFile = currentFile
      }
      if (projectStructure) {
        context.projectStructure = projectStructure
      }
      if (engineInfo) {
        context.engineInfo = engineInfo
      }

      const messagesForAI = [
        { role: 'system', content: 'You are a helpful AI assistant for UhandEngine game engine.' },
        ...messages.map(m => ({ role: m.role, content: m.content })),
        { role: 'user', content: input }
      ]

      const response = await apiService.chatWithAI(messagesForAI)
      
      if (response.success && (response as any).data) {
        const aiResponse: Message = {
          id: (Date.now() + 1).toString(),
          role: 'assistant',
          content: (response as any).data.content || 'No response from AI',
          timestamp: new Date()
        }
        setMessages(prev => [...prev, aiResponse])
      } else {
        throw new Error(response.error || 'AI response failed')
      }
    } catch (err) {
      console.error('AI chat error:', err)
      setError(err instanceof Error ? err.message : 'Failed to get AI response')
      const errorMessage: Message = {
        id: (Date.now() + 1).toString(),
        role: 'assistant',
        content: `Error: ${err instanceof Error ? err.message : 'Failed to get AI response'}`,
        timestamp: new Date()
      }
      setMessages(prev => [...prev, errorMessage])
    } finally {
      setLoading(false)
    }
  }

  return (
    <div className="flex-1 flex flex-col">
      {/* Header */}
      <div className="bg-gray-800 border-b border-gray-700 px-6 py-4">
        <h2 className="text-lg font-semibold flex items-center gap-2">
          <MessageSquare className="w-5 h-5 text-primary-400" />
          AI Assistant
          {!aiConfigured && <AlertCircle className="w-4 h-4 text-yellow-400" />}
        </h2>
      </div>

      {/* Error Banner */}
      {error && (
        <div className="bg-yellow-900 border-b border-yellow-700 px-6 py-3">
          <p className="text-sm text-yellow-200 flex items-center gap-2">
            <AlertCircle className="w-4 h-4" />
            {error}
          </p>
        </div>
      )}

      {/* Messages */}
      <div className="flex-1 overflow-y-auto p-6 space-y-4">
        {messages.map(message => (
          <div
            key={message.id}
            className={`flex gap-3 ${
              message.role === 'user' ? 'flex-row-reverse' : ''
            }`}
          >
            <div className={`flex-shrink-0 ${
              message.role === 'user' ? 'bg-primary-600' : 'bg-gray-700'
            } rounded-full p-2`}>
              {message.role === 'user' ? (
                <User className="w-4 h-4" />
              ) : (
                <Bot className="w-4 h-4" />
              )}
            </div>
            <div className={`flex-1 ${
              message.role === 'user' ? 'text-right' : ''
            }`}>
              <div className={`inline-block max-w-2xl rounded-lg px-4 py-3 ${
                message.role === 'user'
                  ? 'bg-primary-600 text-white'
                  : 'bg-gray-700 text-gray-200'
              }`}>
                <p className="whitespace-pre-wrap text-sm">{message.content}</p>
              </div>
              <p className="text-xs text-gray-500 mt-1">
                {message.timestamp.toLocaleTimeString()}
              </p>
            </div>
          </div>
        ))}
        {loading && (
          <div className="flex gap-3">
            <div className="flex-shrink-0 bg-gray-700 rounded-full p-2">
              <Loader2 className="w-4 h-4 animate-spin" />
            </div>
            <div className="flex-1">
              <div className="inline-block rounded-lg px-4 py-3 bg-gray-700 text-gray-200">
                <p className="text-sm">Thinking...</p>
              </div>
            </div>
          </div>
        )}
      </div>

      {/* Input */}
      <div className="bg-gray-800 border-t border-gray-700 p-4">
        <div className="flex gap-3">
          <input
            type="text"
            value={input}
            onChange={(e) => setInput(e.target.value)}
            onKeyPress={(e) => e.key === 'Enter' && handleSend()}
            placeholder={aiConfigured ? "Ask me anything about UhandEngine..." : "AI not configured"}
            disabled={!aiConfigured || loading}
            className={`flex-1 rounded-lg px-4 py-3 focus:outline-none focus:ring-2 focus:ring-primary-500 ${
              !aiConfigured || loading 
                ? 'bg-gray-600 text-gray-400 cursor-not-allowed' 
                : 'bg-gray-700 text-white'
            }`}
          />
          <button
            onClick={handleSend}
            disabled={!aiConfigured || loading || !input.trim()}
            className={`px-6 py-3 rounded-lg transition-colors ${
              !aiConfigured || loading || !input.trim()
                ? 'bg-gray-600 text-gray-400 cursor-not-allowed'
                : 'bg-primary-600 hover:bg-primary-700 text-white'
            }`}
          >
            {loading ? <Loader2 className="w-5 h-5 animate-spin" /> : <Send className="w-5 h-5" />}
          </button>
        </div>
      </div>
    </div>
  )
}

export default AIAssistant
