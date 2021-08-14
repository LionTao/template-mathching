# Simple template matching

## Prerequisite

- conan package manager
- cmake
- C++ compiler(VS for windows, clang for macos, gcc for linux)

## Quick start

### Powershell
```bash
./build.ps1
```

### Bash
```bash
mkdir -p build
cd build
conan install ..
cmake .. -G "Unix Makefiles"
cmake --build . --config Release
cd bin
stitcher.exe --source "../../target.png" --template "../../mark.png" --output "result.tif"
```