Write-Host "Building Zen C + LearnOpenGL..." -ForegroundColor Cyan

# Create build directory if not exists
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

# Copy shaders
Write-Host "Copying shaders..." -ForegroundColor Yellow
robocopy shaders build\shaders /E /NP /NJH /NJS /NFL

# Copy Textures
Write-Host "Copying textures..." -ForegroundColor Yellow
robocopy textures build\textures /E /NP /NJH /NJS /NFL

# Build normal
Write-Host "Compiling..." -ForegroundColor Yellow
zc build .\src\main.zc

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build completed sucessfully! Executable on: build/app.exe" -ForegroundColor Green
} else {
    Write-Host "Build failed!" -ForegroundColor Red
}