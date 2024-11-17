$rootPath = Join-Path $PSScriptRoot ".."
$prepareVenvPath = Join-Path $PSScriptRoot "prepare_venv.py"
$venvName = "venv"

Write-Host "Preparing the virtual environment..."
python $prepareVenvPath $venvName
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Activating the virtual environment..."
$activateScript = Join-Path $rootPath "venv\Scripts\Activate.ps1"
if (Test-Path $activateScript) {
    & $activateScript
    Write-Host "Virtual environment activated."
} else {
    Write-Host "Could not find the virtual environment activation script."
    exit 1
}

Write-Host "Running pre-commit..."
pre-commit install -f
if ($LASTEXITCODE -ne 0) {
    Write-Host "Pre-commit installation failed."
    exit $LASTEXITCODE
} else {
    Write-Host "Pre-commit installation completed successfully."
}

pre-commit autoupdate
