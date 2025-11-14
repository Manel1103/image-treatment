# PowerShell script to check if all required tools are installed
# Run this script to verify your setup: .\check_setup.ps1

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Image Treatment System - Setup Checker" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$allGood = $true

# Check C++ Compiler
Write-Host "Checking C++ Compiler..." -ForegroundColor Yellow
try {
    $clOutput = & cl 2>&1
    if ($LASTEXITCODE -eq 0 -or $clOutput -match "Microsoft") {
        Write-Host "  ✓ C++ Compiler (MSVC) found" -ForegroundColor Green
    } else {
        throw "Not found"
    }
} catch {
    try {
        $gccOutput = & gcc --version 2>&1
        if ($LASTEXITCODE -eq 0) {
            Write-Host "  ✓ C++ Compiler (GCC/MinGW) found" -ForegroundColor Green
        } else {
            throw "Not found"
        }
    } catch {
        Write-Host "  ✗ C++ Compiler NOT found" -ForegroundColor Red
        Write-Host "    Install Visual Studio with C++ tools or MinGW" -ForegroundColor Yellow
        $allGood = $false
    }
}

# Check CMake
Write-Host "Checking CMake..." -ForegroundColor Yellow
try {
    $cmakeVersion = & cmake --version 2>&1
    if ($LASTEXITCODE -eq 0) {
        $version = ($cmakeVersion | Select-String -Pattern "version (\d+\.\d+)" | ForEach-Object { $_.Matches.Groups[1].Value })
        Write-Host "  ✓ CMake found (version $version)" -ForegroundColor Green
    } else {
        throw "Not found"
    }
} catch {
    Write-Host "  ✗ CMake NOT found" -ForegroundColor Red
    Write-Host "    Download from: https://cmake.org/download/" -ForegroundColor Yellow
    $allGood = $false
}

# Check OpenCV
Write-Host "Checking OpenCV..." -ForegroundColor Yellow
$opencvFound = $false

# Check environment variable
if ($env:OpenCV_DIR) {
    Write-Host "  ✓ OpenCV_DIR environment variable is set: $env:OpenCV_DIR" -ForegroundColor Green
    if (Test-Path $env:OpenCV_DIR) {
        Write-Host "  ✓ OpenCV directory exists" -ForegroundColor Green
        $opencvFound = $true
    } else {
        Write-Host "  ✗ OpenCV directory does not exist at: $env:OpenCV_DIR" -ForegroundColor Red
    }
} else {
    Write-Host "  ⚠ OpenCV_DIR environment variable not set" -ForegroundColor Yellow
}

# Try to find OpenCV in common locations
$commonPaths = @(
    "C:\opencv\build",
    "C:\opencv\build\include",
    "C:\Program Files\opencv",
    "C:\OpenCV"
)

foreach ($path in $commonPaths) {
    if (Test-Path $path) {
        Write-Host "  ℹ Found possible OpenCV installation at: $path" -ForegroundColor Cyan
        if (-not $opencvFound) {
            Write-Host "    Consider setting OpenCV_DIR to: $path" -ForegroundColor Yellow
        }
    }
}

if (-not $opencvFound) {
    Write-Host "  ✗ OpenCV not found" -ForegroundColor Red
    Write-Host "    Download from: https://opencv.org/releases/" -ForegroundColor Yellow
    Write-Host "    Extract and set OpenCV_DIR environment variable" -ForegroundColor Yellow
    $allGood = $false
}

# Check if we're in the project directory
Write-Host "Checking project structure..." -ForegroundColor Yellow
if (Test-Path "CMakeLists.txt") {
    Write-Host "  ✓ CMakeLists.txt found" -ForegroundColor Green
} else {
    Write-Host "  ✗ CMakeLists.txt not found" -ForegroundColor Red
    Write-Host "    Make sure you're in the project root directory" -ForegroundColor Yellow
    $allGood = $false
}

if (Test-Path "include\Treatment.h") {
    Write-Host "  ✓ Project headers found" -ForegroundColor Green
} else {
    Write-Host "  ✗ Project headers not found" -ForegroundColor Red
    $allGood = $false
}

if (Test-Path "src\main_example.cpp") {
    Write-Host "  ✓ Source files found" -ForegroundColor Green
} else {
    Write-Host "  ✗ Source files not found" -ForegroundColor Red
    $allGood = $false
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
if ($allGood) {
    Write-Host "✓ All checks passed! You're ready to build." -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Cyan
    Write-Host "  1. mkdir build" -ForegroundColor White
    Write-Host "  2. cd build" -ForegroundColor White
    Write-Host "  3. cmake .. -G \"Visual Studio 17 2022\" -A x64" -ForegroundColor White
    Write-Host "  4. cmake --build . --config Release" -ForegroundColor White
} else {
    Write-Host "✗ Some checks failed. Please install missing components." -ForegroundColor Red
    Write-Host ""
    Write-Host "See INSTALLATION_GUIDE.md for detailed instructions." -ForegroundColor Yellow
}
Write-Host "========================================" -ForegroundColor Cyan








