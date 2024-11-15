#!/bin/bash
mkdir -p  ~/gpi/try_gpi/Reduce/build_dir  ~/gpi/try_gpi/Reduce/install_dir
# Activate the Spack environm
spack load gpi-space@23.06
pnetc ~/gpi/try_gpi/Reduce/template/workflow/template.xpnet | pnet2dot | dot -T svg > ~/gpi/try_gpi/Reduce/template/workflow/fey.svg
# Define variables
INSTALL_PREFIX="/home/atraore/gpi/try_gpi/Reduce/install_dir/"
BUILD_TYPE="Release"
SINGULAR_HOME="/home/atraore/Singular/Singular/"
BOOST_NO_CMAKE="on"
GPISPACE_ROOT="$HOME/spack/"
GPISPACE_ROOT="$HOME/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/"
BUILD_DIR="/home/atraore/gpi/try_gpi/Reduce/build_dir/"
SOURCE_DIR="/home/atraore/gpi/try_gpi/Reduce/template"

 SINGULAR_INSTALL_DIR=/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/singular-snapshot_22_03-5jvwtprazqirywu2triw6rprjazzi3so
 FLINT_HOME=$SINGULAR_INSTALL_DIR/lib
 LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SINGULAR_INSTALL_DIR/lib
 DEP_LIBS=$SINGULAR_INSTALL_DIR/lib

 cmake -D CMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
      -D CMAKE_BUILD_TYPE=$BUILD_TYPE \
      -D SINGULAR_HOME=$SINGULAR_INSTALL_DIR \
      -D Boost_NO_BOOST_CMAKE=$BOOST_NO_CMAKE \
      -D GPISpace_ROOT=$GPISPACE_ROOT \
      -D GMP_HOME=$GMP_HOME \
      -B $BUILD_DIR \
      -S $SOURCE_DIR

cmake --build $BUILD_DIR --target install -j $(nproc)


