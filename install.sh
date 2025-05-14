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


time "$FEYNMAN_BIN" $COMMON_PARAMS \
  --N 3 \
  --degree 6 \
  --genus 2 \
  --graph "{{1, 2}, {1, 2}, {1, 2}}" \



# time ~/gpi/try_gpi/gspc-gromovwitten/bin/bin/feynman \
#   --gspc-home $GPISPACE_ROOT \
#   --nodefile ~/gpi/try_gpi/gspc-gromovwitten/nodefile \
#   --rif-strategy ssh \
#   --topology "worker:7" \
#   --N 6 \
#   --degree 4 \
#   --genus 3 \
#   --graph "{{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}}" \
#   --log-host localhost \
#   --log-port 9876



# time ~/gpi/try_gpi/gspc-gromovwitten/bin/bin/feynman \
#   --gspc-home $GPISPACE_ROOT \
#   --nodefile ~/gpi/try_gpi/gspc-gromovwitten/nodefile \
#   --rif-strategy ssh \
#   --topology "worker:7" \
#   --N 6 \
#   --degree 22 \
#   --genus 3 \
#   --graph "{{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}" \
#   --log-host localhost \
#   --log-port 9876


# time ~/gpi/try_gpi/gspc-gromovwitten/bin/bin/feynman \
#   --gspc-home $GPISPACE_ROOT \
#   --nodefile ~/gpi/try_gpi/gspc-gromovwitten/nodefile \
#   --rif-strategy ssh \
#   --topology "worker:7" \
#   --N 9 \
#   --degree 4 \
#   --graph "{ {1, 2}, {1, 2}, {1, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 6}, {5, 6}, {5, 6} }" \
#   --log-host localhost \
#   --log-port 9876


# cd $GPISPACE_ROOT/bin && ./gspc-logging-to-stdout.exe --port 9876 > ~/gpi/try_gpi/gspc-gromovwitten/monitor.txt 2>&1

# : > ~/gpi/try_gpi/gspc-gromovwitten/monitor.txt # erase the content of the file monitor.txt