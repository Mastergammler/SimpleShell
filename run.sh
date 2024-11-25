#!/bin/bash

. win32_build.sh
#. build.sh

if [ $COMPILE_STATUS -eq 0 ]; then
    if [ "$WIN32" -eq 1 ]; then
        wine $OUTPUT_FILE
    else
        $OUTPUT_FILE
    fi
fi
