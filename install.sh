#!/bin/bash

# Remove old build and install directories
rm -rf ~/gpi/try_gpi/Reduce/build_dir/* ~/gpi/try_gpi/Reduce/install_dir/*

# Activate the Spack environment
spack load gpi-space@24.12

# Generate SVG workflow diagram
pnetc ~/gpi/try_gpi/Reduce/template/workflow/template.xpnet | pnet2dot | dot -T svg > ~/gpi/try_gpi/Reduce/template/workflow/fey.svg

# Define installation paths
INSTALL_PREFIX="/home/atraore/gpi/try_gpi/Reduce/install_dir/"
BUILD_TYPE="Release"
BOOST_NO_CMAKE="on"
GPISPACE_ROOT="/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5"
BUILD_DIR="/home/atraore/gpi/try_gpi/Reduce/build_dir/"
SOURCE_DIR="/home/atraore/gpi/try_gpi/Reduce/template"

# Set GSPC_HOME explicitly to GPI-Space 24.12
export GSPC_HOME=$GPISPACE_ROOT

# Set FLINT home directory
FLINT_HOME="/usr/local/"

# Set the library path (corrected syntax)
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$FLINT_HOME/lib:$GMP_HOME/lib:$SINGULAR_INSTALL_DIR/lib

# Set the DEP_LIBS variable (corrected syntax)
DEP_LIBS=$FLINT_HOME/lib

GMP_HOME="/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gmp-6.2.1-u44gipctyowzsbhpnkn2a6ffddkpfcyk"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$GMP_HOME/lib

SINGULAR_INSTALL_DIR="/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/singular-snapshot_22_03-5jvwtprazqirywu2triw6rprjazzi3so"

# Sanity checks
if [ ! -d "$SINGULAR_INSTALL_DIR" ]; then
  echo "Error: Singular installation directory not found: $SINGULAR_INSTALL_DIR"
  exit 1
fi

# Check for FLINT library existence
if [ ! -f "$FLINT_HOME/lib/libflint.so" ]; then
  echo "Error: FLINT library not found in $FLINT_HOME/lib"
  exit 1
fi

if [ ! -d "$GPISPACE_ROOT" ]; then
  echo "Error: GPI-Space root directory not found: $GPISPACE_ROOT"
  exit 1
fi

# Run CMake
cmake -D CMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
      -D CMAKE_BUILD_TYPE=$BUILD_TYPE \
      -D SINGULAR_HOME=$SINGULAR_INSTALL_DIR \
      -D Boost_NO_BOOST_CMAKE=$BOOST_NO_CMAKE \
      -D GPISpace_ROOT=$GPISPACE_ROOT \
      -D GMP_HOME=$GMP_HOME \
      -D FLINT_HOME=$FLINT_HOME \
      -B $BUILD_DIR \
      -S $SOURCE_DIR

# Build and install
cmake --build "$BUILD_DIR" --target install -- -j $(nproc)
