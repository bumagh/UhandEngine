import { useEffect } from 'react'
import { usePipelineStore } from '../../store/pipelineStore'
import { apiService } from '../../services/api'
import PipelineSidebar from './PipelineSidebar'
import RequirementsPanel from './RequirementsPanel'
import DesignPanel from './DesignPanel'
import CodePanel from './CodePanel'
import PreviewPanel from './PreviewPanel'
import BuildPanel from './BuildPanel'
import FeedbackPanel from './FeedbackPanel'
import HistoryPanel from './HistoryPanel'

function GameDevPipeline() {
  const currentStage = usePipelineStore(state => state.currentStage)

  useEffect(() => {
    const syncAIConfig = async () => {
      const saved = localStorage.getItem('ai-config')
      if (!saved) return

      try {
        const config = JSON.parse(saved)
        if (config.apiKey) {
          await apiService.saveAIConfig(config)
        }
      } catch (error) {
        console.error('Failed to sync AI config for pipeline:', error)
      }
    }

    syncAIConfig()
  }, [])

  const renderPanel = () => {
    switch (currentStage) {
      case 'requirements':
        return <RequirementsPanel />
      case 'design':
        return <DesignPanel />
      case 'code':
        return <CodePanel />
      case 'preview':
        return <PreviewPanel />
      case 'build':
        return <BuildPanel />
      case 'feedback':
        return <FeedbackPanel />
      case 'history':
        return <HistoryPanel />
      default:
        return <RequirementsPanel />
    }
  }

  return (
    <div className="flex h-screen bg-gray-900 text-white">
      <PipelineSidebar />
      <div className="flex-1 flex flex-col">
        {renderPanel()}
      </div>
    </div>
  )
}

export default GameDevPipeline
