# Quick Start: Upload to GitHub

## Step 1: Install Git
Download and install Git for Windows: https://git-scm.com/download/win
- During installation, make sure to select "Add Git to PATH"

## Step 2: Create GitHub Repository
1. Go to https://github.com and sign in
2. Click the "+" icon → "New repository"
3. Name it: `ImageTreatmentSystem` (or any name you prefer)
4. **DO NOT** check "Initialize with README"
5. Click "Create repository"

## Step 3: Run These Commands
Open PowerShell in this folder and run:

```powershell
# Initialize git
git init

# Add all files (respecting .gitignore)
git add .

# Create commit
git commit -m "Initial commit: Image Treatment System"

# Add your GitHub repository (replace YOUR_USERNAME and REPO_NAME)
git remote add origin https://github.com/YOUR_USERNAME/REPO_NAME.git

# Set branch to main
git branch -M main

# Push to GitHub
git push -u origin main
```

## Step 4: Authentication
When pushing, GitHub will ask for:
- **Username**: Your GitHub username
- **Password**: Use a Personal Access Token (NOT your GitHub password)

### Create Personal Access Token:
1. Go to: https://github.com/settings/tokens
2. Click "Generate new token" → "Generate new token (classic)"
3. Name it: "ImageTreatmentSystem"
4. Select scope: `repo` (check the box)
5. Click "Generate token"
6. **Copy the token immediately** (you won't see it again)
7. Use this token as your password when pushing

## Alternative: Use GitHub Desktop
1. Download: https://desktop.github.com
2. Sign in with GitHub
3. File → Add Local Repository
4. Select: `C:\Users\trabe\ImageTreatmentSystem`
5. Click "Publish repository"

---

**Your project is ready! All files are properly configured with .gitignore**


