$ErrorActionPreference = "Stop"

$rootPath = Join-Path $PSScriptRoot ".."
$prepareVenvPath = Join-Path $PSScriptRoot "prepare_venv.py"
$venvName = "venv"

Write-Host "Preparing the virtual environment..."
try {
    python $prepareVenvPath $venvName
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to prepare virtual environment"
    }
} catch {
    Write-Error "Error preparing virtual environment: $_"
    exit 1
}

Write-Host "Activating the virtual environment..."
$activateScript = Join-Path $rootPath "$venvName\Scripts\Activate.ps1"
if (Test-Path $activateScript) {
    try {
        & $activateScript
        Write-Host "Virtual environment activated."
    } catch {
        Write-Error "Failed to activate virtual environment: $_"
        exit 1
    }
} else {
    Write-Error "Could not find the virtual environment activation script at: $activateScript"
    exit 1
}

Write-Host "Installing pre-commit hooks..."
try {
    pre-commit install -f
    if ($LASTEXITCODE -ne 0) {
        throw "Pre-commit installation failed"
    }
    Write-Host "Pre-commit installation completed successfully."
} catch {
    Write-Error "Error installing pre-commit hooks: $_"
    exit 1
}

Write-Host "Updating pre-commit hooks..."
try {
    pre-commit autoupdate
    if ($LASTEXITCODE -ne 0) {
        throw "Pre-commit autoupdate failed"
    }
    Write-Host "Pre-commit hooks updated successfully."
} catch {
    Write-Error "Error updating pre-commit hooks: $_"
    exit 1
}

Write-Host "Environment setup completed successfully."
