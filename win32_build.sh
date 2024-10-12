#!/bin/bash

SOURCE_FILE="src/main.cpp"
OUTPUT_DIR=".build"
export OUTPUT_FILE="$OUTPUT_DIR/mgsh.exe"
export WIN32=1

# WINDOWS HEADERS

WIN_SDK="$HOME/03-resources/libs/WindowsKits/10/Include/10.0.26100.0"
WIN_LIB="$HOME/03-resources/libs/WindowsKits/10/Lib/10.0.26100.0"
MSVC="$HOME/03-resources/libs/MSVC/14.40.33807"
SDK_INCLUDES="-isystem $WIN_SDK/um -isystem $WIN_SDK/shared -isystem $WIN_SDK/ucrt -isystem $WIN_SDK/winrt -isystem $WIN_SDK/cppwinrt"
MSVC_INCLUDES="-isystem $MSVC/include -isystem $MSVC/atlmfc/include"
SDK_LIBS="-L$WIN_LIB/crt/x64 -L$WIN_LIB/um/x64 -L$WIN_LIB/ucrt/x64 -L$MSVC/lib/x64 -L$MSVC/atlmfc/lib/x64"
INCLUDES="$MSVC_INCLUDES $SDK_INCLUDES"
LIB_PATHS="$SDK_LIBS"

TARGET="-target x86_64-pc-win32 -fuse-ld=lld"

# MSVC WIN 10 needs at least std14
CPP_STD="-std=c++14"

# -D_AMD64_ or -DX86_ = 1 is required for some windows headers
MSVC_DEFAULTS="$TARGET $INCLUDES $CPP_STD -D_AMD64_=1"
OPTIONS="-Wall -Wno-c++11-extensions"


T_START=$(date +%s%N)

clang++ $SOURCE_FILE -o $OUTPUT_FILE $OPTIONS $SDK_LIBS $MSVC_DEFAULTS
export COMPILE_STATUS=$?

T_END=$(date +%s%N)

CST_NANO_TO_MS=1000000
T_ELAPSED=$(((T_END - T_START)/ $CST_NANO_TO_MS))


if [ $COMPILE_STATUS -eq 0 ]; then

    echo "************************"
    echo "   Compile :: $T_ELAPSED ms "
    echo "************************"

fi
