import { useState, useEffect } from 'react'
import { Folder, File, ChevronRight, ChevronDown, Loader2 } from 'lucide-react'
import { useStore } from '../store/useStore'
import { FileNode } from '../services/api'

function ProjectBrowser() {
  const [expanded, setExpanded] = useState<Set<string>>(new Set())
  const { projectStructure, loading, loadProjectStructure, loadFileContent, currentFile, error } = useStore()

  useEffect(() => {
    loadProjectStructure()
  }, [loadProjectStructure])

  const toggleExpand = (path: string) => {
    setExpanded(prev => {
      const next = new Set(prev)
      if (next.has(path)) {
        next.delete(path)
      } else {
        next.add(path)
      }
      return next
    })
  }

  const renderNode = (node: FileNode, path: string = ''): JSX.Element => {
    const currentPath = path ? `${path}/${node.name}` : node.name
    const isExpanded = expanded.has(currentPath)

    if (node.type === 'folder' && node.children) {
      return (
        <div key={currentPath}>
          <div
            className="flex items-center gap-2 px-4 py-2 hover:bg-gray-800 cursor-pointer"
            onClick={() => toggleExpand(currentPath)}
          >
            {isExpanded ? (
              <ChevronDown className="w-4 h-4 text-gray-400" />
            ) : (
              <ChevronRight className="w-4 h-4 text-gray-400" />
            )}
            <Folder className="w-4 h-4 text-blue-400" />
            <span className="text-sm">{node.name}</span>
          </div>
          {isExpanded && (
            <div className="ml-4">
              {node.children.map(child => renderNode(child, currentPath))}
            </div>
          )}
        </div>
      )
    }

    return (
      <div
        key={currentPath}
        className="flex items-center gap-2 px-4 py-2 hover:bg-gray-800 cursor-pointer"
        onClick={() => loadFileContent(currentPath)}
      >
        <div className="w-4" />
        <File className="w-4 h-4 text-gray-400" />
        <span className="text-sm">{node.name}</span>
      </div>
    )
  }

  return (
    <div className="flex-1 flex">
      {/* File Tree */}
      <div className="w-64 bg-gray-800 border-r border-gray-700 overflow-y-auto">
        <div className="px-4 py-3 border-b border-gray-700">
          <h2 className="text-sm font-semibold text-gray-300">Project Explorer</h2>
        </div>
        {loading ? (
          <div className="flex items-center justify-center py-8">
            <Loader2 className="w-6 h-6 animate-spin text-gray-400" />
          </div>
        ) : error ? (
          <div className="px-4 py-8 text-red-400 text-sm">{error}</div>
        ) : projectStructure ? (
          projectStructure.map(node => renderNode(node))
        ) : (
          <div className="px-4 py-8 text-gray-400 text-sm">No project structure loaded</div>
        )}
      </div>

      {/* File Preview */}
      <div className="flex-1 p-8">
        <div className="bg-gray-800 rounded-lg p-6">
          {currentFile ? (
            <div>
              <h1 className="text-2xl font-bold mb-4">{currentFile.path}</h1>
              <pre className="text-sm text-gray-300 overflow-auto max-h-96">
                {currentFile.content}
              </pre>
            </div>
          ) : (
            <div>
              <h1 className="text-2xl font-bold mb-4">UhandEngine Project</h1>
              <p className="text-gray-400 mb-4">
                Select a file from the project explorer to view its contents.
              </p>
              <div className="text-sm text-gray-500">
                <p>Total Files: 20</p>
                <p>Engine Modules: 10</p>
                <p>Examples: 21</p>
              </div>
            </div>
          )}
        </div>
      </div>
    </div>
  )
}

export default ProjectBrowser
