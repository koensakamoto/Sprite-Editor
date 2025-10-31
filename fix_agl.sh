#!/bin/bash
# Script to fix AGL framework issue in Makefile after qmake

MAKEFILE="./build/Qt_6_9_0_for_macOS-Debug/Makefile"

if [ -f "$MAKEFILE" ]; then
    echo "Patching Makefile to remove AGL framework..."
    sed -i.bak 's/-framework AGL//g' "$MAKEFILE"
    echo "Makefile patched successfully!"
else
    echo "Makefile not found at: $MAKEFILE"
    echo "Please run qmake first."
    exit 1
fi
