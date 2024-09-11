#!/bin/bash

OUTPUT_DIR=".build"
OUTPUT_FILE="$OUTPUT_DIR/shell"
SOURCE_FILE="src/main.cpp"

OPTIONS="-std=c++03 -Wall"

T_START=$(date +%s%N)

clang++ $SOURCE_FILE -o $OUTPUT_FILE $OPTIONS

T_END=$(date +%s%N)

CST_NANO_TO_MS=1000000
T_ELAPSED=$(((T_END - T_START)/ $CST_NANO_TO_MS))

echo "************************"
echo "   Compile :: $T_ELAPSED ms "
echo "************************"

$OUTPUT_FILE
