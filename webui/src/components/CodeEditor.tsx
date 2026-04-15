import { useState } from 'react'
import Editor from '@monaco-editor/react'
import { FileCode, Play, Save } from 'lucide-react'

function CodeEditor() {
  const [code, setCode] = useState(`#include "../src/engine/Scene.h"
#include "../src/engine/gameObject.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow(
        "UhandEngine Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    // Main loop
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        // Game logic here
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}`)

  const handleSave = () => {
    console.log('Saving code...')
    // TODO: Implement save functionality
  }

  const handleRun = () => {
    console.log('Running code...')
    // TODO: Implement run functionality
  }

  return (
    <div className="flex-1 flex flex-col">
      {/* Header */}
      <div className="bg-gray-800 border-b border-gray-700 px-6 py-4 flex items-center justify-between">
        <h2 className="text-lg font-semibold flex items-center gap-2">
          <FileCode className="w-5 h-5 text-primary-400" />
          Code Editor
        </h2>
        <div className="flex gap-2">
          <button
            onClick={handleSave}
            className="flex items-center gap-2 bg-gray-700 hover:bg-gray-600 text-white px-4 py-2 rounded-lg transition-colors"
          >
            <Save className="w-4 h-4" />
            Save
          </button>
          <button
            onClick={handleRun}
            className="flex items-center gap-2 bg-primary-600 hover:bg-primary-700 text-white px-4 py-2 rounded-lg transition-colors"
          >
            <Play className="w-4 h-4" />
            Run
          </button>
        </div>
      </div>

      {/* Editor */}
      <div className="flex-1">
        <Editor
          height="100%"
          defaultLanguage="c"
          value={code}
          onChange={(value) => setCode(value || '')}
          theme="vs-dark"
          options={{
            minimap: { enabled: false },
            fontSize: 14,
            lineNumbers: 'on',
            roundedSelection: false,
            scrollBeyondLastLine: false,
            automaticLayout: true,
          }}
        />
      </div>
    </div>
  )
}

export default CodeEditor
