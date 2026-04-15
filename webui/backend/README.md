# UhandEngine WebUI Backend

Backend API server for UhandEngine WebUI, providing file system access and engine control.

## Features

- **Project Structure API**: Get the complete project file tree
- **File Operations**: Read and write file contents
- **Compilation & Execution**: Compile and run game examples
- **Engine Info**: Query engine capabilities and available examples
- **Health Check**: Server status monitoring

## Installation

```bash
cd webui/backend
npm install
```

## Running

```bash
npm start
```

For development with auto-reload:
```bash
npm run dev
```

The server will start on http://localhost:8080

## API Endpoints

### GET /api/health
Health check endpoint.

**Response:**
```json
{
  "success": true,
  "status": "ok",
  "timestamp": "2024-01-15T10:00:00.000Z"
}
```

### GET /api/project/structure
Get the complete project file structure.

**Response:**
```json
{
  "success": true,
  "structure": [
    {
      "name": "src",
      "type": "folder",
      "path": "src",
      "children": [...]
    }
  ]
}
```

### GET /api/file/content?path=<file_path>
Get the content of a specific file.

**Parameters:**
- `path` (required): Relative path to the file from project root

**Response:**
```json
{
  "success": true,
  "content": "file content here"
}
```

### POST /api/file/save
Save content to a file.

**Body:**
```json
{
  "path": "src/engine/Scene.c",
  "content": "file content here"
}
```

**Response:**
```json
{
  "success": true
}
```

### POST /api/compile/run
Compile and run a game example.

**Body:**
```json
{
  "exampleName": "game_example"
}
```

**Response:**
```json
{
  "success": true,
  "output": "Game output here"
}
```

### GET /api/engine/info
Get engine information and available systems/examples.

**Response:**
```json
{
  "success": true,
  "info": {
    "name": "UhandEngine",
    "version": "0.1.0",
    "systems": [...],
    "examples": [...]
  }
}
```

## Security Notes

- This backend server provides direct file system access
- Should only be run in a trusted development environment
- Consider adding authentication for production use
- File operations should be validated and restricted

## Development

The backend is built with:
- **Express**: Web framework
- **CORS**: Cross-origin resource sharing
- **Body-parser**: Request body parsing
- **Node.js fs**: File system operations
- **Node.js child_process**: Process execution

## Future Enhancements

- [ ] Authentication and authorization
- [ ] WebSocket support for real-time updates
- [ ] Build system integration
- [ ] Debugging interface
- [ ] Performance monitoring
- [ ] File change watching
