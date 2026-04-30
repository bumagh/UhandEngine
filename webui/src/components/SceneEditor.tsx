import { useState, useEffect } from 'react'
import { 
  Layers, Plus, Trash2, Edit3, Save, FolderOpen, 
  Eye, EyeOff, Play, Square, ChevronRight, ChevronDown,
  Box, Type, Container as ContainerIcon, ChevronLeft, ChevronRight as ChevronRightIcon
} from 'lucide-react'

interface GameObject {
  id: string
  name: string
  type: 'base' | 'sprite' | 'text' | 'container'
  visible: boolean
  active: boolean
  depth: number
  transform: {
    x: number
    y: number
    rotation: number
    scaleX: number
    scaleY: number
    anchorX: number
    anchorY: number
    width: number
    height: number
  }
  children?: GameObject[]
  components?: string[]
}

interface Scene {
  id: string
  name: string
  rootObject: GameObject
}

export default function SceneEditor() {
  const [scene, setScene] = useState<Scene | null>(null)
  const [selectedObject, setSelectedObject] = useState<GameObject | null>(null)
  const [expandedNodes, setExpandedNodes] = useState<Set<string>>(new Set())
  const [isPreviewing, setIsPreviewing] = useState(false)
  const [leftPanelCollapsed, setLeftPanelCollapsed] = useState(false)
  const [rightPanelCollapsed, setRightPanelCollapsed] = useState(false)

  // 初始化示例场景
  useEffect(() => {
    const exampleScene: Scene = {
      id: 'scene-1',
      name: 'Main Scene',
      rootObject: {
        id: 'root',
        name: 'Root',
        type: 'container',
        visible: true,
        active: true,
        depth: 0,
        transform: { x: 0, y: 0, rotation: 0, scaleX: 1, scaleY: 1, anchorX: 0.5, anchorY: 0.5, width: 0, height: 0 },
        children: [
          {
            id: 'child-1',
            name: 'Background',
            type: 'sprite',
            visible: true,
            active: true,
            depth: 0,
            transform: { x: 400, y: 300, rotation: 0, scaleX: 1, scaleY: 1, anchorX: 0.5, anchorY: 0.5, width: 800, height: 600 },
            children: [],
            components: ['SpriteComponent']
          },
          {
            id: 'child-2',
            name: 'Player',
            type: 'sprite',
            visible: true,
            active: true,
            depth: 1,
            transform: { x: 100, y: 100, rotation: 0, scaleX: 1, scaleY: 1, anchorX: 0.5, anchorY: 0.5, width: 64, height: 64 },
            children: [],
            components: ['SpriteComponent', 'Transform']
          }
        ]
      }
    }
    setScene(exampleScene)
  }, [])

  const toggleNode = (nodeId: string) => {
    const newExpanded = new Set(expandedNodes)
    if (newExpanded.has(nodeId)) {
      newExpanded.delete(nodeId)
    } else {
      newExpanded.add(nodeId)
    }
    setExpandedNodes(newExpanded)
  }

  const renderGameObjectTree = (node: GameObject, level: number = 0): JSX.Element => {
    const isExpanded = expandedNodes.has(node.id)
    const isSelected = selectedObject?.id === node.id
    const hasChildren = node.children && node.children.length > 0

    const getIcon = () => {
      switch (node.type) {
        case 'sprite': return <Box className="w-4 h-4" />
        case 'text': return <Type className="w-4 h-4" />
        case 'container': return <ContainerIcon className="w-4 h-4" />
        default: return <Layers className="w-4 h-4" />
      }
    }

    return (
      <div key={node.id}>
        <div
          className={`flex items-center gap-2 px-2 py-1 cursor-pointer hover:bg-gray-700 ${
            isSelected ? 'bg-blue-600' : ''
          }`}
          style={{ paddingLeft: `${level * 16 + 8}px` }}
          onClick={() => setSelectedObject(node)}
        >
          {hasChildren ? (
            <button onClick={(e) => { e.stopPropagation(); toggleNode(node.id) }}>
              {isExpanded ? <ChevronDown className="w-4 h-4" /> : <ChevronRight className="w-4 h-4" />}
            </button>
          ) : (
            <span className="w-4 h-4" />
          )}
          {getIcon()}
          <span className="flex-1">{node.name}</span>
          <button
            onClick={(e) => { e.stopPropagation(); toggleVisibility(node) }}
            className="p-1 hover:bg-gray-600 rounded"
          >
            {node.visible ? <Eye className="w-4 h-4" /> : <EyeOff className="w-4 h-4 text-gray-500" />}
          </button>
        </div>
        {isExpanded && hasChildren && node.children?.map(child => renderGameObjectTree(child, level + 1))}
      </div>
    )
  }

  const toggleVisibility = (node: GameObject) => {
    if (!scene) return

    const updateVisibility = (obj: GameObject): GameObject => {
      if (obj.id === node.id) {
        return { ...obj, visible: !obj.visible }
      }
      if (obj.children) {
        return { ...obj, children: obj.children.map(updateVisibility) }
      }
      return obj
    }

    setScene({ ...scene, rootObject: updateVisibility(scene.rootObject) })
    if (selectedObject?.id === node.id) {
      setSelectedObject({ ...selectedObject, visible: !selectedObject.visible })
    }
  }

  const addGameObject = (type: 'sprite' | 'text' | 'container') => {
    if (!scene || !selectedObject) return

    const newObject: GameObject = {
      id: `object-${Date.now()}`,
      name: `New ${type.charAt(0).toUpperCase() + type.slice(1)}`,
      type,
      visible: true,
      active: true,
      depth: 0,
      transform: { x: 0, y: 0, rotation: 0, scaleX: 1, scaleY: 1, anchorX: 0.5, anchorY: 0.5, width: 64, height: 64 },
      children: [],
      components: type === 'sprite' ? ['SpriteComponent'] : type === 'text' ? ['TextComponent'] : []
    }

    const addToTree = (obj: GameObject): GameObject => {
      if (obj.id === selectedObject.id) {
        return {
          ...obj,
          children: [...(obj.children || []), newObject]
        }
      }
      if (obj.children) {
        return { ...obj, children: obj.children.map(addToTree) }
      }
      return obj
    }

    setScene({ ...scene, rootObject: addToTree(scene.rootObject) })
    setExpandedNodes(new Set([...expandedNodes, selectedObject.id]))
  }

  const deleteGameObject = (nodeId: string) => {
    if (!scene) return

    const removeFromTree = (obj: GameObject): GameObject | null => {
      if (obj.id === nodeId) {
        return null
      }
      if (obj.children) {
        const filteredChildren = obj.children.map(removeFromTree).filter(Boolean) as GameObject[]
        return { ...obj, children: filteredChildren }
      }
      return obj
    }

    const newRoot = removeFromTree(scene.rootObject)
    if (newRoot) {
      setScene({ ...scene, rootObject: newRoot })
      if (selectedObject?.id === nodeId) {
        setSelectedObject(null)
      }
    }
  }

  const updateTransform = (property: keyof GameObject['transform'], value: number) => {
    if (!selectedObject) return

    const updateProperty = (obj: GameObject): GameObject => {
      if (obj.id === selectedObject.id) {
        return {
          ...obj,
          transform: { ...obj.transform, [property]: value }
        }
      }
      if (obj.children) {
        return { ...obj, children: obj.children.map(updateProperty) }
      }
      return obj
    }

    setScene(prev => prev ? { ...prev, rootObject: updateProperty(prev.rootObject) } : null)
    setSelectedObject({ ...selectedObject, transform: { ...selectedObject.transform, [property]: value } })
  }

  const saveScene = () => {
    if (!scene) return
    const json = JSON.stringify(scene, null, 2)
    const blob = new Blob([json], { type: 'application/json' })
    const url = URL.createObjectURL(blob)
    const a = document.createElement('a')
    a.href = url
    a.download = `${scene.name}.json`
    a.click()
    URL.revokeObjectURL(url)
  }

  const loadScene = (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0]
    if (!file) return

    const reader = new FileReader()
    reader.onload = (e) => {
      try {
        const loadedScene = JSON.parse(e.target?.result as string)
        setScene(loadedScene)
      } catch (error) {
        console.error('Failed to load scene:', error)
      }
    }
    reader.readAsText(file)
  }

  return (
    <div className="flex h-full w-full">
      {/* Left Panel - Hierarchy */}
      <div 
        className={`${leftPanelCollapsed ? 'w-0' : 'flex-1 min-w-0 max-w-md'} bg-gray-800 flex flex-col border-r border-gray-700 transition-all duration-300 overflow-hidden`}
        style={{ flexBasis: leftPanelCollapsed ? '0' : 'auto' }}
      >
        <div className="p-4 border-b border-gray-700 flex items-center justify-between">
          <h2 className="text-lg font-semibold flex items-center gap-2 whitespace-nowrap">
            <Layers className="w-5 h-5" />
            Scene Hierarchy
          </h2>
          <button
            onClick={() => setLeftPanelCollapsed(!leftPanelCollapsed)}
            className="p-1 hover:bg-gray-700 rounded flex-shrink-0"
          >
            <ChevronLeft className="w-4 h-4" />
          </button>
        </div>
        
        {!leftPanelCollapsed && (
          <>
            <div className="flex-1 overflow-y-auto p-2 min-w-0">
              {scene ? renderGameObjectTree(scene.rootObject) : <div className="text-gray-500 p-4">No scene loaded</div>}
            </div>

            <div className="p-4 border-t border-gray-700 flex-shrink-0">
              <div className="flex gap-2 mb-2">
                <button
                  onClick={() => addGameObject('sprite')}
                  className="flex-1 bg-blue-600 hover:bg-blue-700 px-3 py-2 rounded flex items-center justify-center gap-2 text-sm"
                >
                  <Plus className="w-4 h-4 flex-shrink-0" />
                  <span className="truncate">Sprite</span>
                </button>
                <button
                  onClick={() => addGameObject('text')}
                  className="flex-1 bg-green-600 hover:bg-green-700 px-3 py-2 rounded flex items-center justify-center gap-2 text-sm"
                >
                  <Plus className="w-4 h-4 flex-shrink-0" />
                  <span className="truncate">Text</span>
                </button>
                <button
                  onClick={() => addGameObject('container')}
                  className="flex-1 bg-purple-600 hover:bg-purple-700 px-3 py-2 rounded flex items-center justify-center gap-2 text-sm"
                >
                  <Plus className="w-4 h-4 flex-shrink-0" />
                  <span className="truncate">Container</span>
                </button>
              </div>
              <div className="flex gap-2">
                <button
                  onClick={saveScene}
                  className="flex-1 bg-gray-600 hover:bg-gray-700 px-3 py-2 rounded flex items-center justify-center gap-2 text-sm"
                >
                  <Save className="w-4 h-4 flex-shrink-0" />
                  <span className="truncate">Save</span>
                </button>
                <label className="flex-1 bg-gray-600 hover:bg-gray-700 px-3 py-2 rounded flex items-center justify-center gap-2 text-sm cursor-pointer">
                  <FolderOpen className="w-4 h-4 flex-shrink-0" />
                  <span className="truncate">Load</span>
                  <input type="file" accept=".json" onChange={loadScene} className="hidden" />
                </label>
              </div>
            </div>
          </>
        )}
      </div>

      {/* Middle Panel - Preview */}
      <div className="flex-1 flex flex-col bg-gray-900 min-w-0">
        <div className="p-4 border-b border-gray-700 flex items-center justify-between flex-shrink-0">
          <h2 className="text-lg font-semibold whitespace-nowrap">Scene Preview</h2>
          <button
            onClick={() => setIsPreviewing(!isPreviewing)}
            className={`${isPreviewing ? 'bg-red-600 hover:bg-red-700' : 'bg-green-600 hover:bg-green-700'} px-4 py-2 rounded flex items-center gap-2 flex-shrink-0`}
          >
            {isPreviewing ? <Square className="w-4 h-4" /> : <Play className="w-4 h-4" />}
            {isPreviewing ? 'Stop' : 'Preview'}
          </button>
        </div>
        
        <div className="flex-1 flex items-center justify-center bg-gray-950 relative overflow-hidden min-w-0">
          {isPreviewing ? (
            <div className="text-white text-center">
              <p>Preview Mode</p>
              <p className="text-sm text-gray-400">Game preview would be rendered here</p>
            </div>
          ) : (
            <div className="text-gray-500 text-center">
              <p>Click "Preview" to test the scene</p>
            </div>
          )}
        </div>
      </div>

      {/* Right Panel - Inspector */}
      <div 
        className={`${rightPanelCollapsed ? 'w-0' : 'flex-1 min-w-0 max-w-md'} bg-gray-800 flex flex-col border-l border-gray-700 transition-all duration-300 overflow-hidden`}
        style={{ flexBasis: rightPanelCollapsed ? '0' : 'auto' }}
      >
        <div className="p-4 border-b border-gray-700 flex items-center justify-between flex-shrink-0">
          <h2 className="text-lg font-semibold flex items-center gap-2 whitespace-nowrap">
            <Edit3 className="w-5 h-5" />
            Inspector
          </h2>
          <div className="flex items-center gap-2 flex-shrink-0">
            {selectedObject && (
              <button
                onClick={() => deleteGameObject(selectedObject.id)}
                className="p-2 bg-red-600 hover:bg-red-700 rounded"
                title="Delete"
              >
                <Trash2 className="w-4 h-4" />
              </button>
            )}
            <button
              onClick={() => setRightPanelCollapsed(!rightPanelCollapsed)}
              className="p-1 hover:bg-gray-700 rounded"
            >
              <ChevronRightIcon className="w-4 h-4" />
            </button>
          </div>
        </div>

        {!rightPanelCollapsed && (
          <div className="flex-1 overflow-y-auto p-4 min-w-0">
            {selectedObject ? (
              <div className="space-y-4">
                {/* Basic Properties */}
                <div>
                  <h3 className="text-sm font-semibold text-gray-400 mb-2">Basic</h3>
                  <div className="space-y-2">
                    <div>
                      <label className="text-xs text-gray-400">Name</label>
                      <input
                        type="text"
                        value={selectedObject.name}
                        onChange={(e) => {
                          const updateName = (obj: GameObject): GameObject => {
                            if (obj.id === selectedObject.id) return { ...obj, name: e.target.value }
                            if (obj.children) return { ...obj, children: obj.children.map(updateName) }
                            return obj
                          }
                          setScene(prev => prev ? { ...prev, rootObject: updateName(prev.rootObject) } : null)
                          setSelectedObject({ ...selectedObject, name: e.target.value })
                        }}
                        className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                      />
                    </div>
                    <div className="flex gap-2">
                      <label className="flex items-center gap-2 text-sm whitespace-nowrap">
                        <input
                          type="checkbox"
                          checked={selectedObject.visible}
                          onChange={() => toggleVisibility(selectedObject)}
                        />
                        Visible
                      </label>
                      <label className="flex items-center gap-2 text-sm whitespace-nowrap">
                        <input
                          type="checkbox"
                          checked={selectedObject.active}
                          onChange={(event) => {
                            const updateActive = (obj: GameObject): GameObject => {
                              if (obj.id === selectedObject.id) return { ...obj, active: event.target.checked }
                              if (obj.children) return { ...obj, children: obj.children.map(updateActive) }
                              return obj
                            }
                            setScene(prev => prev ? { ...prev, rootObject: updateActive(prev.rootObject) } : null)
                            setSelectedObject({ ...selectedObject, active: event.target.checked })
                          }}
                        />
                        Active
                      </label>
                    </div>
                    <div>
                      <label className="text-xs text-gray-400">Depth</label>
                      <input
                        type="number"
                        value={selectedObject.depth}
                        onChange={(e) => {
                          const updateDepth = (obj: GameObject): GameObject => {
                            if (obj.id === selectedObject.id) return { ...obj, depth: parseInt(e.target.value) }
                            if (obj.children) return { ...obj, children: obj.children.map(updateDepth) }
                            return obj
                          }
                          setScene(prev => prev ? { ...prev, rootObject: updateDepth(prev.rootObject) } : null)
                          setSelectedObject({ ...selectedObject, depth: parseInt(e.target.value) })
                        }}
                        className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                      />
                    </div>
                  </div>
                </div>

                {/* Transform */}
                <div>
                  <h3 className="text-sm font-semibold text-gray-400 mb-2">Transform</h3>
                  <div className="space-y-2">
                    <div className="grid grid-cols-2 gap-2">
                      <div>
                        <label className="text-xs text-gray-400">X</label>
                        <input
                          type="number"
                          value={selectedObject.transform.x}
                          onChange={(e) => updateTransform('x', parseFloat(e.target.value))}
                          className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                        />
                      </div>
                      <div>
                        <label className="text-xs text-gray-400">Y</label>
                        <input
                          type="number"
                          value={selectedObject.transform.y}
                          onChange={(e) => updateTransform('y', parseFloat(e.target.value))}
                          className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                        />
                      </div>
                    </div>
                    <div>
                      <label className="text-xs text-gray-400">Rotation (deg)</label>
                      <input
                        type="number"
                        value={(selectedObject.transform.rotation * 180 / Math.PI).toFixed(2)}
                        onChange={(e) => updateTransform('rotation', parseFloat(e.target.value) * Math.PI / 180)}
                        className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                      />
                    </div>
                    <div className="grid grid-cols-2 gap-2">
                      <div>
                        <label className="text-xs text-gray-400">Scale X</label>
                        <input
                          type="number"
                          step="0.1"
                          value={selectedObject.transform.scaleX}
                          onChange={(e) => updateTransform('scaleX', parseFloat(e.target.value))}
                          className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                        />
                      </div>
                      <div>
                        <label className="text-xs text-gray-400">Scale Y</label>
                        <input
                          type="number"
                          step="0.1"
                          value={selectedObject.transform.scaleY}
                          onChange={(e) => updateTransform('scaleY', parseFloat(e.target.value))}
                          className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                        />
                      </div>
                    </div>
                    <div className="grid grid-cols-2 gap-2">
                      <div>
                        <label className="text-xs text-gray-400">Anchor X</label>
                        <input
                          type="number"
                          step="0.1"
                          min="0"
                          max="1"
                          value={selectedObject.transform.anchorX}
                          onChange={(e) => updateTransform('anchorX', parseFloat(e.target.value))}
                          className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                        />
                      </div>
                      <div>
                        <label className="text-xs text-gray-400">Anchor Y</label>
                        <input
                          type="number"
                          step="0.1"
                          min="0"
                          max="1"
                          value={selectedObject.transform.anchorY}
                          onChange={(e) => updateTransform('anchorY', parseFloat(e.target.value))}
                          className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                        />
                      </div>
                    </div>
                    <div className="grid grid-cols-2 gap-2">
                      <div>
                        <label className="text-xs text-gray-400">Width</label>
                        <input
                          type="number"
                          value={selectedObject.transform.width}
                          onChange={(e) => updateTransform('width', parseFloat(e.target.value))}
                          className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                        />
                      </div>
                      <div>
                        <label className="text-xs text-gray-400">Height</label>
                        <input
                          type="number"
                          value={selectedObject.transform.height}
                          onChange={(e) => updateTransform('height', parseFloat(e.target.value))}
                          className="w-full bg-gray-700 px-2 py-1 rounded text-sm"
                        />
                      </div>
                    </div>
                  </div>
                </div>

                {/* Components */}
                {selectedObject.components && selectedObject.components.length > 0 && (
                  <div>
                    <h3 className="text-sm font-semibold text-gray-400 mb-2">Components</h3>
                    <div className="space-y-1">
                      {selectedObject.components.map((component, index) => (
                        <div key={index} className="bg-gray-700 px-2 py-1 rounded text-sm truncate">
                          {component}
                        </div>
                      ))}
                    </div>
                  </div>
                )}
              </div>
            ) : (
              <div className="text-gray-500 text-center py-8">
                Select a GameObject to edit its properties
              </div>
            )}
          </div>
        )}
      </div>
    </div>
  )
}
