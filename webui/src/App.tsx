import { useState } from 'react'
import ProjectBrowser from './components/ProjectBrowser'
import AIAssistant from './components/AIAssistant'
import CodeEditor from './components/CodeEditor'
import { Layout, Menu, Code } from 'lucide-react'

function App() {
  const [activePanel, setActivePanel] = useState<'project' | 'ai' | 'editor'>('project')

  return (
    <div className="flex h-screen bg-gray-900 text-white">
      {/* Sidebar */}
      <div className="w-16 bg-gray-800 flex flex-col items-center py-4 space-y-4">
        <button
          onClick={() => setActivePanel('project')}
          className={`p-3 rounded-lg transition-colors ${
            activePanel === 'project' ? 'bg-primary-600' : 'hover:bg-gray-700'
          }`}
          title="Project Browser"
        >
          <Layout className="w-6 h-6" />
        </button>
        <button
          onClick={() => setActivePanel('ai')}
          className={`p-3 rounded-lg transition-colors ${
            activePanel === 'ai' ? 'bg-primary-600' : 'hover:bg-gray-700'
          }`}
          title="AI Assistant"
        >
          <Menu className="w-6 h-6" />
        </button>
        <button
          onClick={() => setActivePanel('editor')}
          className={`p-3 rounded-lg transition-colors ${
            activePanel === 'editor' ? 'bg-primary-600' : 'hover:bg-gray-700'
          }`}
          title="Code Editor"
        >
          <Code className="w-6 h-6" />
        </button>
      </div>

      {/* Main Content */}
      <div className="flex-1 flex">
        {activePanel === 'project' && <ProjectBrowser />}
        {activePanel === 'ai' && <AIAssistant />}
        {activePanel === 'editor' && <CodeEditor />}
      </div>
    </div>
  )
}

export default App
