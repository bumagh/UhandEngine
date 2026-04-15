import { useState, useEffect } from 'react'
import { Settings as SettingsIcon, Save, Key, Globe, Server } from 'lucide-react'
import { apiService } from '../services/api'

interface AIConfig {
  provider: 'openai' | 'anthropic' | 'custom'
  apiKey: string
  model: string
  baseUrl: string
}

function Settings() {
  const [config, setConfig] = useState<AIConfig>({
    provider: 'openai',
    apiKey: '',
    model: 'gpt-3.5-turbo',
    baseUrl: 'https://api.openai.com/v1'
  })
  const [loading, setLoading] = useState(false)
  const [saving, setSaving] = useState(false)
  const [message, setMessage] = useState<{ type: 'success' | 'error'; text: string } | null>(null)

  useEffect(() => {
    loadConfig()
  }, [])

  const loadConfig = () => {
    const saved = localStorage.getItem('ai-config')
    if (saved) {
      try {
        setConfig(JSON.parse(saved))
      } catch (e) {
        console.error('Failed to load config:', e)
      }
    }
  }

  const handleSave = async () => {
    setSaving(true)
    setMessage(null)

    try {
      // Save to localStorage
      localStorage.setItem('ai-config', JSON.stringify(config))

      // Save to backend
      const response = await apiService.saveAIConfig(config)
      
      if (response.success) {
        setMessage({ type: 'success', text: 'Configuration saved successfully' })
      } else {
        setMessage({ type: 'error', text: response.error || 'Failed to save configuration' })
      }
    } catch (error) {
      setMessage({ type: 'error', text: error instanceof Error ? error.message : 'Failed to save configuration' })
    } finally {
      setSaving(false)
    }
  }

  const handleTest = async () => {
    setLoading(true)
    setMessage(null)

    try {
      const response = await apiService.testAIConfig(config)
      
      if (response.success) {
        setMessage({ type: 'success', text: 'Configuration test successful' })
      } else {
        setMessage({ type: 'error', text: response.error || 'Configuration test failed' })
      }
    } catch (error) {
      setMessage({ type: 'error', text: error instanceof Error ? error.message : 'Configuration test failed' })
    } finally {
      setLoading(false)
    }
  }

  const handleProviderChange = (provider: 'openai' | 'anthropic' | 'custom') => {
    const defaults = {
      openai: { model: 'gpt-3.5-turbo', baseUrl: 'https://api.openai.com/v1' },
      anthropic: { model: 'claude-3-opus-20240229', baseUrl: 'https://api.anthropic.com' },
      custom: { model: '', baseUrl: '' }
    }
    setConfig(prev => ({
      ...prev,
      provider,
      model: defaults[provider].model,
      baseUrl: defaults[provider].baseUrl
    }))
  }

  return (
    <div className="flex-1 flex flex-col">
      {/* Header */}
      <div className="bg-gray-800 border-b border-gray-700 px-6 py-4">
        <h2 className="text-lg font-semibold flex items-center gap-2">
          <SettingsIcon className="w-5 h-5 text-primary-400" />
          AI Configuration
        </h2>
      </div>

      {/* Content */}
      <div className="flex-1 p-6 overflow-y-auto">
        <div className="max-w-2xl mx-auto space-y-6">
          {/* Provider Selection */}
          <div className="bg-gray-800 rounded-lg p-6">
            <h3 className="text-lg font-medium mb-4 flex items-center gap-2">
              <Server className="w-5 h-5 text-primary-400" />
              API Provider
            </h3>
            <div className="flex gap-4">
              <button
                onClick={() => handleProviderChange('openai')}
                className={`flex-1 p-4 rounded-lg border-2 transition-colors ${
                  config.provider === 'openai'
                    ? 'border-primary-600 bg-primary-600/20'
                    : 'border-gray-700 hover:border-gray-600'
                }`}
              >
                <div className="font-medium">OpenAI</div>
                <div className="text-sm text-gray-400 mt-1">GPT models</div>
              </button>
              <button
                onClick={() => handleProviderChange('anthropic')}
                className={`flex-1 p-4 rounded-lg border-2 transition-colors ${
                  config.provider === 'anthropic'
                    ? 'border-primary-600 bg-primary-600/20'
                    : 'border-gray-700 hover:border-gray-600'
                }`}
              >
                <div className="font-medium">Anthropic</div>
                <div className="text-sm text-gray-400 mt-1">Claude models</div>
              </button>
              <button
                onClick={() => handleProviderChange('custom')}
                className={`flex-1 p-4 rounded-lg border-2 transition-colors ${
                  config.provider === 'custom'
                    ? 'border-primary-600 bg-primary-600/20'
                    : 'border-gray-700 hover:border-gray-600'
                }`}
              >
                <div className="font-medium">Custom</div>
                <div className="text-sm text-gray-400 mt-1">OpenAI-compatible</div>
              </button>
            </div>
          </div>

          {/* API Key */}
          <div className="bg-gray-800 rounded-lg p-6">
            <h3 className="text-lg font-medium mb-4 flex items-center gap-2">
              <Key className="w-5 h-5 text-primary-400" />
              API Key
            </h3>
            <input
              type="password"
              value={config.apiKey}
              onChange={(e) => setConfig(prev => ({ ...prev, apiKey: e.target.value }))}
              placeholder="Enter your API key"
              className="w-full bg-gray-700 text-white rounded-lg px-4 py-3 focus:outline-none focus:ring-2 focus:ring-primary-500"
            />
            <p className="text-sm text-gray-400 mt-2">
              Your API key is stored locally and never sent to any server except the AI provider.
            </p>
          </div>

          {/* Model */}
          <div className="bg-gray-800 rounded-lg p-6">
            <h3 className="text-lg font-medium mb-4">Model</h3>
            <input
              type="text"
              value={config.model}
              onChange={(e) => setConfig(prev => ({ ...prev, model: e.target.value }))}
              placeholder="e.g., gpt-3.5-turbo, gpt-4, claude-3-opus-20240229"
              className="w-full bg-gray-700 text-white rounded-lg px-4 py-3 focus:outline-none focus:ring-2 focus:ring-primary-500"
            />
            <p className="text-sm text-gray-400 mt-2">
              The model to use for AI requests. Common models: gpt-3.5-turbo, gpt-4, claude-3-opus-20240229
            </p>
          </div>

          {/* Base URL */}
          <div className="bg-gray-800 rounded-lg p-6">
            <h3 className="text-lg font-medium mb-4 flex items-center gap-2">
              <Globe className="w-5 h-5 text-primary-400" />
              Base URL
            </h3>
            <input
              type="text"
              value={config.baseUrl}
              onChange={(e) => setConfig(prev => ({ ...prev, baseUrl: e.target.value }))}
              placeholder="https://api.openai.com/v1"
              className="w-full bg-gray-700 text-white rounded-lg px-4 py-3 focus:outline-none focus:ring-2 focus:ring-primary-500"
            />
            <p className="text-sm text-gray-400 mt-2">
              The base URL for the API. Use this for custom endpoints or proxies.
            </p>
          </div>

          {/* Message */}
          {message && (
            <div className={`rounded-lg p-4 ${
              message.type === 'success' 
                ? 'bg-green-900 border border-green-700 text-green-200'
                : 'bg-red-900 border border-red-700 text-red-200'
            }`}>
              {message.text}
            </div>
          )}

          {/* Actions */}
          <div className="flex gap-4">
            <button
              onClick={handleTest}
              disabled={loading || !config.apiKey}
              className="flex-1 bg-gray-700 hover:bg-gray-600 text-white px-6 py-3 rounded-lg transition-colors disabled:opacity-50 disabled:cursor-not-allowed flex items-center justify-center gap-2"
            >
              {loading ? 'Testing...' : 'Test Configuration'}
            </button>
            <button
              onClick={handleSave}
              disabled={saving}
              className="flex-1 bg-primary-600 hover:bg-primary-700 text-white px-6 py-3 rounded-lg transition-colors disabled:opacity-50 disabled:cursor-not-allowed flex items-center justify-center gap-2"
            >
              {saving ? (
                <>Saving...</>
              ) : (
                <>
                  <Save className="w-5 h-5" />
                  Save Configuration
                </>
              )}
            </button>
          </div>
        </div>
      </div>
    </div>
  )
}

export default Settings
