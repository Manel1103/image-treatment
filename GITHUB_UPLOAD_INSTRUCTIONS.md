# Instructions to Upload to GitHub

## Prerequisites
1. Install Git if not already installed: https://git-scm.com/download/win
2. Create a GitHub account if you don't have one: https://github.com
3. Create a new repository on GitHub (don't initialize with README, .gitignore, or license)

## Steps to Upload

### 1. Open PowerShell or Command Prompt in the project directory
```powershell
cd C:\Users\trabe\ImageTreatmentSystem
```

### 2. Initialize Git repository (if not already done)
```powershell
git init
```

### 3. Add all files (respecting .gitignore)
```powershell
git add .
```

### 4. Create initial commit
```powershell
git commit -m "Initial commit: Image Treatment System"
```

### 5. Add your GitHub repository as remote
Replace `YOUR_USERNAME` and `YOUR_REPO_NAME` with your actual GitHub username and repository name:
```powershell
git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
```

### 6. Rename branch to main (if needed)
```powershell
git branch -M main
```

### 7. Push to GitHub
```powershell
git push -u origin main
```

## If you need to authenticate:
- GitHub may ask for your username and password
- For password, use a Personal Access Token (not your GitHub password)
- Create token at: https://github.com/settings/tokens
- Select scope: `repo` (full control of private repositories)

## Alternative: Using GitHub Desktop
1. Download GitHub Desktop: https://desktop.github.com
2. Sign in with your GitHub account
3. File → Add Local Repository
4. Select: `C:\Users\trabe\ImageTreatmentSystem`
5. Click "Publish repository" button

## Files that will be uploaded:
✅ Source code (src/, include/)
✅ CMakeLists.txt
✅ README.md
✅ .gitignore
✅ CMakeSettings.json (optional)
✅ check_setup.ps1 (optional)

## Files that will be ignored (not uploaded):
❌ Build directories (out/, build/)
❌ Compiled executables (*.exe, *.dll)
❌ Object files (*.obj, *.o)
❌ Generated images (*.jpg, *.png)
❌ IDE files (.vs/, .vscode/)
❌ CMake cache files


