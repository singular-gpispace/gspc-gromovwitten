#!/bin/bash

# Check if workflow type is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <workflow_type>"
    echo "Available workflow types:"
    echo "  base (uses feynman_base.xpnet)"
    echo "  composition (uses feynman_composition.xpnet)"
    echo "  doubles (uses feynman_doubles_edges.xpnet)"
    echo "  quasi_psi (uses feynman_quasi_psi.xpnet)"
    echo "  quasi (uses feynman_quasi.xpnet)"
    echo "  sum_psi (uses feynman_sum_psi.xpnet)"
    echo "  sum (uses feynman_sum.xpnet)"
    exit 1
fi

WORKFLOW_TYPE=$1

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
WORKSPACE_DIR="$SCRIPT_DIR"

# Set base paths
BUILD_DIR="$WORKSPACE_DIR/build"
WORKFLOW_DIR="$WORKSPACE_DIR/workflow"

# Create directories if they don't exist
mkdir -p "$BUILD_DIR"
mkdir -p "$WORKFLOW_DIR"

# Clean build and install directories
rm -rf "$BUILD_DIR"/* 

# Load GPI-Space
echo "Loading GPI-Space..."
spack load gpi-space@24.12

# Get GPI-Space root from spack
GPISPACE_ROOT=$(spack location -i gpi-space@24.12)
export GSPC_HOME="$GPISPACE_ROOT"

# Map workflow type to actual file name
case $WORKFLOW_TYPE in
    "base")
        WORKFLOW_FILE="feynman_base.xpnet"
        ;;
    "composition")
        WORKFLOW_FILE="feynman_composition.xpnet"
        ;;
    "doubles")
        WORKFLOW_FILE="feynman_doubles_edges.xpnet"
        ;;
    "quasi_psi")
        WORKFLOW_FILE="feynman_quasi_psi.xpnet"
        ;;
    "quasi")
        WORKFLOW_FILE="feynman_quasi.xpnet"
        ;;
    "sum_psi")
        WORKFLOW_FILE="feynman_sum_psi.xpnet"
        ;;
    "sum")
        WORKFLOW_FILE="feynman_sum.xpnet"
        ;;
    *)
        echo "Invalid workflow type: $WORKFLOW_TYPE"
        exit 1
        ;;
esac

# Check if workflow file exists
if [ ! -f "$WORKFLOW_DIR/$WORKFLOW_FILE" ]; then
    echo "Error: Workflow file $WORKFLOW_FILE not found in $WORKFLOW_DIR"
    exit 1
fi

# Generate SVG workflow diagram
pnetc "$WORKFLOW_DIR/$WORKFLOW_FILE" | pnet2dot | dot -T svg > "$WORKFLOW_DIR/feynman_${WORKFLOW_TYPE}.svg"

# Build project
echo "Building project..."
cmake \
  -D GPISpace_ROOT=$GPISPACE_ROOT \
  -D GSPC_WITH_MONITOR_APP=OFF \
  -D Boost_ROOT=$GPISPACE_ROOT \
  -D Boost_NO_BOOST_CMAKE=TRUE \
  -D WORKFLOW_TYPE="$WORKFLOW_TYPE" \
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
FEYNMAN_BIN="$BUILD_DIR/install/bin/feynman"
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


# Run the selected workflow
case $WORKFLOW_TYPE in
    "base")
        # Base Feynman (K4)
        "$FEYNMAN_BIN" $COMMON_PARAMS --type "base" \
            --N 6 \
            --degree 4 \
            --graph "{{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}}"
        ;;
    "composition")
        # Feynman Composition
        "$FEYNMAN_BIN" $COMMON_PARAMS --type "composition" \
            --m 1 \
            --N 6 \
            --degree 4 \
            --graph "{{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}}"
        ;;
    "doubles")
        # Base Feynman (9 edges)
        "$FEYNMAN_BIN" $COMMON_PARAMS --type "doubles" \
            --N 9 \
            --degree 4 \
            --graph "{{1, 2}, {1, 2}, {1, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 6}, {5, 6}, {5, 6}}"
        ;;
    "quasi_psi")
        # Quasi Psi Class
        "$FEYNMAN_BIN" $COMMON_PARAMS --type "quasi_psi" \
            --N 3 \
            --degree 10 \
            --graph "{{1, 2}, {2, 3}, {3, 1}}" \
            --g 1 \
            --genus "1 0 0" \
            --loop "0 0 0"
        ;;
    "quasi")
        # Quasi Feynman
        "$FEYNMAN_BIN" $COMMON_PARAMS --type "quasi" \
            --N 3 \
            --degree 6 \
            --genus 2 \
            --graph "{{1, 2}, {1, 2}, {1, 2}}"
        ;;
    "sum_psi")
        # Feynman Sum Psi Class
        "$FEYNMAN_BIN" $COMMON_PARAMS --type "sum_psi" \
            --N 4 \
            --degree 10 \
            --graph "{{1,1},{1, 2}, {2, 3}, {3, 1}}" \
            --genus "0 0 0" \
            --loop "0 0 0"
        ;;
    "sum")
        # Feynman Sum
        "$FEYNMAN_BIN" $COMMON_PARAMS --type "sum" \
            --N 6 \
            --degree 4 \
            --graph "{{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}}"
        ;;
    *)
        echo "Invalid workflow type: $WORKFLOW_TYPE"
        exit 1
        ;;
esac