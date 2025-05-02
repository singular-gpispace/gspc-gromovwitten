#!/bin/bash

# Direct values
SOURCE="cpp/minimal.cpp"
OUTPUT="cpp/minimal"
COMPILE_FLAGS="-I/usr/local/include -L/usr/local/lib -lflint -lgmp"
OUTPUT_FILE="cpp/results.txt"

# Create cpp directory if it doesn't exist
mkdir -p cpp

# Compile the program
echo "Compiling program..."
g++ $SOURCE -o $OUTPUT $COMPILE_FLAGS

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Run the program
echo "Running program..."
./$OUTPUT > $OUTPUT_FILE

# Check if execution was successful
if [ $? -ne 0 ]; then
    echo "Execution failed!"
    exit 1
fi

echo "Results saved to $OUTPUT_FILE"
echo "Contents of results:"
cat $OUTPUT_FILE