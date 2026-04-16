@echo off
echo Restarting Frontend and Backend Development Servers...

:: Kill backend server on port 18081
echo Killing backend server on port 18081...
for /f "tokens=5" %%a in ('netstat -ano ^| findstr :18081') do (
    taskkill /F /PID %%a 2>nul
)

:: Kill frontend server on port 13000
echo Killing frontend server on port 13000...
for /f "tokens=5" %%a in ('netstat -ano ^| findstr :13000') do (
    taskkill /F /PID %%a 2>nul
)

:: Kill frontend server on port 13001
echo Killing frontend server on port 13001...
for /f "tokens=5" %%a in ('netstat -ano ^| findstr :13001') do (
    taskkill /F /PID %%a 2>nul
)

:: Wait a moment
timeout /t 2 /nobreak >nul

:: Start backend server
echo Starting backend server...
cd /d "%~dp0webui\backend"
start "Backend Server" cmd /k "node server.js"

:: Wait for backend to start
timeout /t 3 /nobreak >nul

:: Start frontend server
echo Starting frontend server...
cd /d "%~dp0webui"
start "Frontend Server" cmd /k "npm run dev"

echo.
echo Development servers restarted successfully!
echo Backend: http://localhost:18081
echo Frontend: http://localhost:13001
echo.
