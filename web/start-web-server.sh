#!/bin/bash
echo "Starting UhandEngine Web Server..."
echo ""
echo "Web server will start at: http://localhost:8000"
echo "Press Ctrl+C to stop the server"
echo ""
python3 -m http.server 8000
