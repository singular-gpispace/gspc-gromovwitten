#!/bin/bash

# Clean previous build and install directories
#rm -rf ~/gpi/try_gpi/gspc-gromovwitten/build_dir/* ~/gpi/try_gpi/gspc-gromovwitten/install_dir/*

# Create necessary directories
mkdir -p ~/gpi/try_gpi/gspc-gromovwitten/build_dir ~/gpi/try_gpi/gspc-gromovwitten/install_dir

# Activate the Spack environment
spack load gpi-space@24.12 || { echo "Failed to load GPI-Space"; exit 1; }

# Generate SVG workflow diagram
pnetc ~/gpi/try_gpi/gspc-gromovwitten/template/workflow/template.xpnet | pnet2dot | dot -T svg > ~/gpi/try_gpi/gspc-gromovwitten/template/workflow/fey.svg

# Define variables
INSTALL_PREFIX="/home/atraore/gpi/try_gpi/gspc-gromovwitten/install_dir/"
BUILD_TYPE="Release"
BOOST_NO_CMAKE="on"

# Set GPI-Space root
GPISPACE_ROOT="/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5"
export GSPC_HOME=$GPISPACE_ROOT

# Set FLINT home directory
FLINT_HOME="/usr/local"
FLINT_INCLUDE_DIR="/usr/local/include/flint"
FLINT_LIB_DIR="/usr/local/lib"

# Set GMP home directory
GMP_HOME="/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gmp-6.2.1-gjqp7e3m3fik4wsuqqcxv2brlj2wkyza"

# Set Singular install
SINGULAR_INSTALL_DIR="/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/singular-4.4.0p2-k7rgdkzo5prqsvxjckejvcdvxgjr64bk"

# Set the library path correctly
export LD_LIBRARY_PATH=$GPISPACE_ROOT/lib:$FLINT_LIB_DIR:$GMP_HOME/lib:$SINGULAR_INSTALL_DIR/lib:$LD_LIBRARY_PATH

# Verify paths before building
ls $GPISPACE_ROOT/gspc_version || { echo "GPI-Space version file missing!"; exit 1; }

# Run CMake
cmake -D CMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
      -D CMAKE_BUILD_TYPE=$BUILD_TYPE \
      -D SINGULAR_HOME=$SINGULAR_INSTALL_DIR \
      -D Boost_NO_BOOST_CMAKE=$BOOST_NO_CMAKE \
      -D GPISpace_ROOT=$GPISPACE_ROOT \
      -D GMP_HOME=$GMP_HOME \
      -D FLINT_HOME=$FLINT_HOME \
      -D FLINT_INCLUDE_DIR=$FLINT_INCLUDE_DIR \
      -D FLINT_LIB_DIR=$FLINT_LIB_DIR \
      -B ~/gpi/try_gpi/gspc-gromovwitten/build_dir \
      -S ~/gpi/try_gpi/gspc-gromovwitten/template

# Build and install
cmake --build ~/gpi/try_gpi/gspc-gromovwitten/build_dir --target install -- -j $(nproc)