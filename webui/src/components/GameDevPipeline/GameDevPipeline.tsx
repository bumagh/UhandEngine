import { usePipelineStore } from '../../store/pipelineStore'
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
