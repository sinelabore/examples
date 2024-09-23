#!/bin/sh

# Check if the CLASSPATH environment variable is set
# E.g. export CLASSPATH=~/sinelabore/bin/*
if [ -z "$CLASSPATH" ]; then
    echo "CLASSPATH environment variable is not set."
    exit 1
else
    echo "Classpath set to: $CLASSPATH"
fi

# Run the Java command using the CLASSPATH environment variable
java -cp "$CLASSPATH" codegen.Main -s -p CADIFRA -o learning learning.cdd
