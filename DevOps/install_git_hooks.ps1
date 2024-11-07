Copy-Item -Path "GitHooks\*" -Destination "../.git/hooks" -Recurse -Force

$sourceFiles = Get-ChildItem -Path "GitHooks" -Recurse

Write-Host "Files copied from GitHooks:"
$sourceFiles | ForEach-Object { Write-Host $_.Name }

Read-Host -Prompt "Copiing of GitHooks has completed"