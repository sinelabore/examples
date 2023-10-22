#!/bin/bash

if [ -z "$CODEGEN_PATH" ]; then
  echo "CODEGEN_PATH is not set. Please set it before running this script."
  exit 1
fi

java -cp "$CODEGEN_PATH/*" codegen.Main -p ssc -l lua -o oven oven.xml
