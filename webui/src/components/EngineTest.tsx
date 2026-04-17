import { useState } from 'react'
import { Play, Square, RefreshCw, Terminal, CheckCircle, XCircle } from 'lucide-react'

interface TestResult {
  platform: 'native' | 'web'
  status: 'pending' | 'running' | 'success' | 'failed'
  output: string
  timestamp: string
}

function EngineTest() {
  const [testResults, setTestResults] = useState<TestResult[]>([])
  const [isRunning, setIsRunning] = useState(false)
  const [selectedPlatform, setSelectedPlatform] = useState<'native' | 'web' | 'both'>('both')
  const [webUrl, setWebUrl] = useState<string>('')

  const runTest = async (platform: 'native' | 'web') => {
    const newResult: TestResult = {
      platform,
      status: 'running',
      output: `Starting ${platform} test...`,
      timestamp: new Date().toISOString()
    }
    setTestResults(prev => [...prev, newResult])

    try {
      // 这里需要调用后端 API 来触发编译和测试
      const response = await fetch('/api/engine/test', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ platform })
      })

      const data = await response.json()

      setTestResults(prev => prev.map(r => 
        r.platform === platform && r.timestamp === newResult.timestamp
          ? { ...r, status: data.success ? 'success' : 'failed', output: data.output }
          : r
      ))

      if (platform === 'web' && data.success) {
        setWebUrl(`http://localhost:8080/index.html`)
      }
    } catch (error) {
      setTestResults(prev => prev.map(r => 
        r.platform === platform && r.timestamp === newResult.timestamp
          ? { ...r, status: 'failed', output: `Error: ${error}` }
          : r
      ))
    }
  }

  const runAllTests = async () => {
    setIsRunning(true)
    if (selectedPlatform === 'native' || selectedPlatform === 'both') {
      await runTest('native')
    }
    if (selectedPlatform === 'web' || selectedPlatform === 'both') {
      await runTest('web')
    }
    setIsRunning(false)
  }

  const clearResults = () => {
    setTestResults([])
    setWebUrl('')
  }

  return (
    <div className="flex-1 flex flex-col p-6 bg-gray-900 text-white">
      <div className="mb-6">
        <h1 className="text-2xl font-bold mb-2">引擎测试</h1>
        <p className="text-gray-400">测试 UhandEngine 的编译和运行状态</p>
      </div>

      <div className="flex gap-4 mb-6">
        <select
          value={selectedPlatform}
          onChange={(e) => setSelectedPlatform(e.target.value as 'native' | 'web' | 'both')}
          className="bg-gray-800 border border-gray-700 rounded px-4 py-2"
        >
          <option value="native">Native 平台</option>
          <option value="web">Web 平台</option>
          <option value="both">全部平台</option>
        </select>

        <button
          onClick={runAllTests}
          disabled={isRunning}
          className="flex items-center gap-2 bg-primary-600 hover:bg-primary-700 disabled:bg-gray-700 px-4 py-2 rounded"
        >
          {isRunning ? <RefreshCw className="w-4 h-4 animate-spin" /> : <Play className="w-4 h-4" />}
          {isRunning ? '运行中...' : '运行测试'}
        </button>

        <button
          onClick={clearResults}
          className="flex items-center gap-2 bg-gray-700 hover:bg-gray-600 px-4 py-2 rounded"
        >
          <Square className="w-4 h-4" />
          清除结果
        </button>
      </div>

      {webUrl && (
        <div className="mb-6">
          <h2 className="text-lg font-semibold mb-2 flex items-center gap-2">
            <Terminal className="w-5 h-5" />
            Web 预览
          </h2>
          <a
            href={webUrl}
            target="_blank"
            rel="noopener noreferrer"
            className="inline-flex items-center gap-2 bg-primary-600 hover:bg-primary-700 px-4 py-2 rounded"
          >
            在新标签页中打开 Web 预览
          </a>
        </div>
      )}

      <div className="flex-1">
        <h2 className="text-lg font-semibold mb-2 flex items-center gap-2">
          <Terminal className="w-5 h-5" />
          测试结果
        </h2>
        <div className="bg-gray-800 rounded-lg p-4 h-96 overflow-y-auto">
          {testResults.length === 0 ? (
            <p className="text-gray-500">暂无测试结果</p>
          ) : (
            <div className="space-y-4">
              {testResults.map((result, index) => (
                <div key={index} className="border border-gray-700 rounded p-4">
                  <div className="flex items-center justify-between mb-2">
                    <span className="font-semibold">
                      {result.platform === 'native' ? 'Native' : 'Web'} 平台
                    </span>
                    <div className="flex items-center gap-2">
                      {result.status === 'success' && <CheckCircle className="w-5 h-5 text-green-500" />}
                      {result.status === 'failed' && <XCircle className="w-5 h-5 text-red-500" />}
                      {result.status === 'running' && <RefreshCw className="w-5 h-5 text-yellow-500 animate-spin" />}
                      <span className="text-sm text-gray-400">
                        {new Date(result.timestamp).toLocaleTimeString()}
                      </span>
                    </div>
                  </div>
                  <pre className="text-sm bg-gray-900 p-2 rounded overflow-x-auto">
                    {result.output}
                  </pre>
                </div>
              ))}
            </div>
          )}
        </div>
      </div>
    </div>
  )
}

export default EngineTest
