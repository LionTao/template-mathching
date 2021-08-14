mkdir -force build
Set-Location build
conan install ..
cmake .. -G "Visual Studio 16"
cmake --build . --config Release
Set-Location bin
stitcher.exe --source "../../target.png" --template "../../mark.png" --output "result.tif"