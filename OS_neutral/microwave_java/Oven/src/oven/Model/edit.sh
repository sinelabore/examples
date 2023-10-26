#!/bin/bash

if [ -z "$CODEGEN_PATH" ]; then
  echo "CODEGEN_PATH is not set. Please set it before running this script."
  exit 1
fi

# Get the script's directory
script_dir=$(dirname "$0")

java -cp "$CODEGEN_PATH/*" codegen.Main -p ssc -E -l java -U "$script_dir/codegen.cfg" -o Reactor Reactor.xml
 
