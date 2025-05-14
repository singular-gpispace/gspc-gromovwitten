#!/bin/bash

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
WORKSPACE_DIR="$SCRIPT_DIR"

# Set base paths
BUILD_DIR="$WORKSPACE_DIR/build"
BIN_DIR="$BUILD_DIR/bin"
WORKFLOW_DIR="$WORKSPACE_DIR/workflow"

# Create directories if they don't exist
mkdir -p "$BUILD_DIR"
mkdir -p "$BIN_DIR"
mkdir -p "$WORKFLOW_DIR"

# Clean build and install directories
rm -rf "$BUILD_DIR"/* 

# Load GPI-Space
echo "Loading GPI-Space..."
spack load gpi-space@24.12

# Get GPI-Space root from spack
GPISPACE_ROOT=$(spack location -i gpi-space@24.12)
export GSPC_HOME="$GPISPACE_ROOT"

# Generate SVG workflow diagram
echo "Generating workflow diagram..."
pnetc "$WORKFLOW_DIR/feynman.xpnet" | pnet2dot | dot -T svg > "$WORKFLOW_DIR/feynman.svg"

# Build project
echo "Building project..."
cmake \
  -D GPISpace_ROOT=$GPISPACE_ROOT \
  -D GSPC_WITH_MONITOR_APP=OFF \
  -D Boost_ROOT=$GPISPACE_ROOT \
  -D CMAKE_INSTALL_PREFIX=$BIN_DIR \
  -D Boost_NO_BOOST_CMAKE=TRUE \
  -B "$BUILD_DIR" \
  -S "$WORKSPACE_DIR"

if [ $? -ne 0 ]; then
    echo "CMake configuration failed"
    exit 1
fi

cmake \
  --build "$BUILD_DIR" \
  --target install \
  -j $(nproc)

if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
fi

# Check if binary exists
FEYNMAN_BIN="$BIN_DIR/bin/feynman"
if [ ! -f "$FEYNMAN_BIN" ]; then
    echo "Error: Feynman binary not found at $FEYNMAN_BIN"
    exit 1
fi

# Common parameters for all workflows
COMMON_PARAMS="--gspc-home $GPISPACE_ROOT \
  --nodefile $WORKSPACE_DIR/nodefile \
  --rif-strategy ssh \
  --topology worker:7 \
  --log-host localhost \
  --log-port 9876"

# Run the first workflow
echo "Running first workflow..."
time "$FEYNMAN_BIN" $COMMON_PARAMS \
  --N 3 \
  --degree 10 \
  --graph "{{1, 2}, {2, 3}, {3, 1}}" \
  --genus "1 0 0" \
  --loop "0 0 0"

# Run the second workflow
echo "Running second workflow..."
time "$FEYNMAN_BIN" $COMMON_PARAMS \
  --N 4 \
  --degree 10 \
  --graph "{{1,1},{1, 2}, {2, 3}, {3, 1}}" \
  --genus "0 0 0" \
  --loop "0 0 0"