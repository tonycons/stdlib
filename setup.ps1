
if (Test-Path build) {
    Remove-Item -Path build -Recurse -Force
}

[string]$tgt = Read-Host "Select one of the targets (win64, win64-debug, x64-linux)"

Copy-Item -Path "res\bin\toolchain.$tgt" -Destination "build" -Recurse

& "cmake" @(
 "--no-warn-unused-cli",
 "-DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo",
 "-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE",
 "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=toolchain/toolchain.$tgt.cmake",
 "-DCMAKE_MAKE_PROGRAM=ninja",
 "-Bbuild",
 "-G Ninja"
)

