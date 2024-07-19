#!/bin/bash

# Check if CODEGEN_PATH is set
if [ -z "$CODEGEN_PATH" ]; then
    echo "Error: CODEGEN_PATH is not set. Please set it before running this script."
    exit 1
fi

# Run the Java code generation tool
java -cp "$CODEGEN_PATH/*" codegen.Main -E -p ssc -o sm.xml sm.xml
