# PowerShell script to upload project to GitHub
# Make sure Git is installed and you have a GitHub repository created

Write-Host "=== Image Treatment System - GitHub Upload ===" -ForegroundColor Cyan
Write-Host ""

# Check if git is available
try {
    $gitVersion = git --version
    Write-Host "Git found: $gitVersion" -ForegroundColor Green
} catch {
    Write-Host "ERROR: Git is not installed or not in PATH!" -ForegroundColor Red
    Write-Host "Please install Git from: https://git-scm.com/download/win" -ForegroundColor Yellow
    exit 1
}

# Get repository URL from user
Write-Host "Enter your GitHub repository URL (e.g., https://github.com/username/repo-name.git):" -ForegroundColor Yellow
$repoUrl = Read-Host

if ([string]::IsNullOrWhiteSpace($repoUrl)) {
    Write-Host "ERROR: Repository URL cannot be empty!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Initializing git repository..." -ForegroundColor Cyan
git init

Write-Host "Adding files..." -ForegroundColor Cyan
git add .

Write-Host "Creating initial commit..." -ForegroundColor Cyan
git commit -m "Initial commit: Image Treatment System"

Write-Host "Adding remote repository..." -ForegroundColor Cyan
git remote add origin $repoUrl

Write-Host "Setting branch to main..." -ForegroundColor Cyan
git branch -M main

Write-Host ""
Write-Host "Ready to push! Run the following command:" -ForegroundColor Green
Write-Host "  git push -u origin main" -ForegroundColor Yellow
Write-Host ""
Write-Host "Note: You may need to authenticate with your GitHub username and Personal Access Token" -ForegroundColor Cyan


