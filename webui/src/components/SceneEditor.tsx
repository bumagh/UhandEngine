import { useState, useEffect, useRef } from 'react'
import { 
  Layers, Plus, Trash2, Edit3, Save, FolderOpen, 
  Eye, EyeOff, Play, Square, ChevronRight, ChevronDown,
  Box, Type, Container as ContainerIcon, ChevronLeft, ChevronRight as ChevronRightIcon, RefreshCw
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
  const [previewUrl, setPreviewUrl] = useState<string>('')
  const [previewLoading, setPreviewLoading] = useState(false)
  const [useEnginePreview, setUseEnginePreview] = useState(false)
  const canvasRef = useRef<HTMLCanvasElement>(null)
  const [isDragging, setIsDragging] = useState(false)
  const [dragStartPos, setDragStartPos] = useState<{ x: number; y: number } | null>(null)
  const [draggedObject, setDraggedObject] = useState<GameObject | null>(null)

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

  const startPreview = async () => {
    if (!scene) return

    setPreviewLoading(true)
    setIsPreviewing(true)

    try {
      const response = await fetch('/api/scene/preview', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ scene })
      })

      const data = await response.json()

      if (data.success) {
        setPreviewUrl(data.url)
      } else {
        console.error('Failed to start preview:', data.error)
        setIsPreviewing(false)
      }
    } catch (error) {
      console.error('Failed to start preview:', error)
      setIsPreviewing(false)
    } finally {
      setPreviewLoading(false)
    }
  }

  const stopPreview = async () => {
    try {
      await fetch('/api/scene/preview', {
        method: 'DELETE'
      })
    } catch (error) {
      console.error('Failed to stop preview:', error)
    }
    setIsPreviewing(false)
    setPreviewUrl('')
  }

  const togglePreview = () => {
    if (isPreviewing) {
      stopPreview()
    } else {
      startPreview()
    }
  }

  // Canvas 实时渲染
  useEffect(() => {
    if (!canvasRef.current || !scene) return

    const canvas = canvasRef.current
    const ctx = canvas.getContext('2d')
    if (!ctx) return

    // 设置 Canvas 尺寸
    const container = canvas.parentElement
    if (container) {
      canvas.width = container.clientWidth
      canvas.height = container.clientHeight
    }

    // 清空 Canvas
    ctx.fillStyle = '#0a0a0a'
    ctx.fillRect(0, 0, canvas.width, canvas.height)

    // 将原点移到 Canvas 中心
    ctx.save()
    ctx.translate(canvas.width / 2, canvas.height / 2)

    // 绘制网格（以中心为原点）
    ctx.strokeStyle = '#1a1a1a'
    ctx.lineWidth = 1
    const gridSize = 50
    const startX = -canvas.width / 2
    const startY = -canvas.height / 2
    for (let x = startX; x < canvas.width / 2; x += gridSize) {
      ctx.beginPath()
      ctx.moveTo(x, -canvas.height / 2)
      ctx.lineTo(x, canvas.height / 2)
      ctx.stroke()
    }
    for (let y = startY; y < canvas.height / 2; y += gridSize) {
      ctx.beginPath()
      ctx.moveTo(-canvas.width / 2, y)
      ctx.lineTo(canvas.width / 2, y)
      ctx.stroke()
    }

    // 绘制坐标轴
    ctx.strokeStyle = '#333333'
    ctx.lineWidth = 2
    ctx.beginPath()
    ctx.moveTo(-canvas.width / 2, 0)
    ctx.lineTo(canvas.width / 2, 0)
    ctx.stroke()
    ctx.beginPath()
    ctx.moveTo(0, -canvas.height / 2)
    ctx.lineTo(0, canvas.height / 2)
    ctx.stroke()

    // 递归渲染 GameObject
    const renderGameObject = (obj: GameObject, parentTransform?: any) => {
      if (!obj.visible) return

      const transform = obj.transform
      const x = transform.x + (parentTransform?.x || 0)
      const y = transform.y + (parentTransform?.y || 0)
      const width = transform.width || 64
      const height = transform.height || 64
      const rotation = transform.rotation || 0
      const scaleX = transform.scaleX || 1
      const scaleY = transform.scaleY || 1

      ctx.save()
      ctx.translate(x, y)
      ctx.rotate(rotation)
      ctx.scale(scaleX, scaleY)

      // 绘制选中框
      if (selectedObject?.id === obj.id) {
        ctx.strokeStyle = '#3b82f6'
        ctx.lineWidth = 2
        ctx.strokeRect(-width/2 - 4, -height/2 - 4, width + 8, height + 8)
      }

      // 根据类型绘制不同的形状
      switch (obj.type) {
        case 'sprite':
          ctx.fillStyle = '#3b82f6'
          ctx.fillRect(-width/2, -height/2, width, height)
          break
        case 'text':
          ctx.fillStyle = '#22c55e'
          ctx.font = '16px Arial'
          ctx.textAlign = 'center'
          ctx.textBaseline = 'middle'
          ctx.fillText('Text', 0, 0)
          break
        case 'container':
          ctx.strokeStyle = '#a855f7'
          ctx.lineWidth = 2
          ctx.strokeRect(-width/2, -height/2, width, height)
          break
        default:
          ctx.fillStyle = '#6b7280'
          ctx.fillRect(-width/2, -height/2, width, height)
      }

      ctx.restore()

      // 渲染子对象
      if (obj.children) {
        obj.children.forEach(child => renderGameObject(child, { x, y }))
      }
    }

    renderGameObject(scene.rootObject)

    ctx.restore()

    // 绘制坐标信息（恢复到左上角坐标系）
    ctx.fillStyle = '#ffffff'
    ctx.font = '12px Arial'
    ctx.fillText(`Scene: ${scene.name}`, 10, 20)
    ctx.fillText(`Objects: ${countGameObjects(scene.rootObject)}`, 10, 40)
  }, [scene, selectedObject, useEnginePreview])

  // 辅助函数：统计 GameObject 数量
  const countGameObjects = (obj: GameObject): number => {
    let count = 1
    if (obj.children) {
      obj.children.forEach(child => {
        count += countGameObjects(child)
      })
    }
    return count
  }

  // 辅助函数：查找点击的 GameObject
  const findGameObjectAtPosition = (obj: GameObject, x: number, y: number, parentTransform?: any): GameObject | null => {
    if (!obj.visible) return null

    const transform = obj.transform
    const objX = transform.x + (parentTransform?.x || 0)
    const objY = transform.y + (parentTransform?.y || 0)
    const width = transform.width || 64
    const height = transform.height || 64

    // 检查点是否在 GameObject 矩形内
    const halfWidth = width / 2
    const halfHeight = height / 2
    if (x >= objX - halfWidth && x <= objX + halfWidth &&
        y >= objY - halfHeight && y <= objY + halfHeight) {
      return obj
    }

    // 递归检查子对象
    if (obj.children) {
      for (const child of obj.children) {
        const found = findGameObjectAtPosition(child, x, y, { x: objX, y: objY })
        if (found) return found
      }
    }

    return null
  }

  // Canvas 坐标转换为场景坐标
  const canvasToSceneCoords = (canvasX: number, canvasY: number): { x: number; y: number } => {
    if (!canvasRef.current) return { x: 0, y: 0 }
    const canvas = canvasRef.current
    const centerX = canvas.width / 2
    const centerY = canvas.height / 2
    return {
      x: canvasX - centerX,
      y: canvasY - centerY
    }
  }

  // 鼠标按下事件
  const handleMouseDown = (e: React.MouseEvent<HTMLCanvasElement>) => {
    if (!canvasRef.current || !scene || useEnginePreview) return

    const canvas = canvasRef.current
    const rect = canvas.getBoundingClientRect()
    const canvasX = e.clientX - rect.left
    const canvasY = e.clientY - rect.top

    const sceneCoords = canvasToSceneCoords(canvasX, canvasY)
    const clickedObject = findGameObjectAtPosition(scene.rootObject, sceneCoords.x, sceneCoords.y)

    if (clickedObject && clickedObject.id !== 'root') {
      setSelectedObject(clickedObject)
      setIsDragging(true)
      setDragStartPos(sceneCoords)
      setDraggedObject(clickedObject)
    }
  }

  // 鼠标移动事件
  const handleMouseMove = (e: React.MouseEvent<HTMLCanvasElement>) => {
    if (!isDragging || !draggedObject || !canvasRef.current || !scene) return

    const canvas = canvasRef.current
    const rect = canvas.getBoundingClientRect()
    const canvasX = e.clientX - rect.left
    const canvasY = e.clientY - rect.top

    const sceneCoords = canvasToSceneCoords(canvasX, canvasY)
    const deltaX = sceneCoords.x - (dragStartPos?.x || 0)
    const deltaY = sceneCoords.y - (dragStartPos?.y || 0)

    // 更新 GameObject 位置
    const updateObjectPosition = (obj: GameObject): GameObject => {
      if (obj.id === draggedObject.id) {
        return {
          ...obj,
          transform: {
            ...obj.transform,
            x: obj.transform.x + deltaX,
            y: obj.transform.y + deltaY
          }
        }
      }
      if (obj.children) {
        return {
          ...obj,
          children: obj.children.map(updateObjectPosition)
        }
      }
      return obj
    }

    setScene(prev => prev ? { ...prev, rootObject: updateObjectPosition(prev.rootObject) } : null)
    setSelectedObject(prev => prev ? {
      ...prev,
      transform: {
        ...prev.transform,
        x: prev.transform.x + deltaX,
        y: prev.transform.y + deltaY
      }
    } : null)
    setDragStartPos(sceneCoords)
  }

  // 鼠标释放事件
  const handleMouseUp = () => {
    setIsDragging(false)
    setDragStartPos(null)
    setDraggedObject(null)
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
          <div className="flex items-center gap-2">
            <button
              onClick={() => setUseEnginePreview(!useEnginePreview)}
              className={`px-3 py-2 rounded flex items-center gap-2 text-sm ${
                useEnginePreview ? 'bg-gray-600 hover:bg-gray-700' : 'bg-blue-600 hover:bg-blue-700'
              }`}
              title="Toggle between real-time Canvas preview and compiled Engine preview"
            >
              {useEnginePreview ? 'Engine' : 'Real-time'}
            </button>
            {useEnginePreview && (
              <button
                onClick={togglePreview}
                disabled={previewLoading}
                className={`${isPreviewing ? 'bg-red-600 hover:bg-red-700' : 'bg-green-600 hover:bg-green-700'} px-4 py-2 rounded flex items-center gap-2 flex-shrink-0 disabled:opacity-50 disabled:cursor-not-allowed`}
              >
                {previewLoading ? (
                  <>
                    <RefreshCw className="w-4 h-4 animate-spin" />
                    Loading...
                  </>
                ) : isPreviewing ? (
                  <>
                    <Square className="w-4 h-4" />
                    Stop
                  </>
                ) : (
                  <>
                    <Play className="w-4 h-4" />
                    Preview
                  </>
                )}
              </button>
            )}
          </div>
        </div>
        
        <div className="flex-1 bg-gray-950 relative overflow-hidden min-w-0">
          {useEnginePreview ? (
            previewUrl ? (
              <iframe
                src={previewUrl}
                className="w-full h-full border-0"
                title="Scene Preview"
              />
            ) : (
              <div className="flex items-center justify-center h-full">
                <div className="text-gray-500 text-center">
                  <p>Click "Preview" to test the scene with compiled engine</p>
                </div>
              </div>
            )
          ) : (
            <canvas
              ref={canvasRef}
              className="w-full h-full cursor-crosshair"
              title="Real-time Scene Preview"
              onMouseDown={handleMouseDown}
              onMouseMove={handleMouseMove}
              onMouseUp={handleMouseUp}
              onMouseLeave={handleMouseUp}
            />
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
