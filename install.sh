#!/bin/bash
rm -rf ~/Dokumente/gpispace/gspc-gromovwitten/build_dir/* ~/Dokumente/gpispace/gspc-gromovwitten/install_dir/*


# Activate the Spack environment
spack load gpi-space@24.12

# Generate SVG workflow diagram
pnetc ~/Dokumente/gpispace/gspc-gromovwitten/template/workflow/template.xpnet | pnet2dot | dot -T svg > ~/Dokumente/gpispace/gspc-gromovwitten/template/workflow/fey.svg

# Define variables
BUILD_TYPE="Release"
BOOST_NO_CMAKE="on"
GPISPACE_ROOT="/net/nascip131/users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-fjs7sy3f6o4dhm4i44ogigr7kszvw6oc"
INSTALL_PREFIX="/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/install_dir/"
BUILD_DIR="/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/build_dir/"
FLINT_HOME="/users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/flint-3.1.2-nkrmyxtfk72g74hlkhu7pfnpjmihm2ti/"
GMP_HOME="/users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gmp-6.3.0-oxer5kfejyvurympzmcsng7v6dmkybg3"
SINGULAR_INSTALL_DIR="/users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/singular-4.4.0p2-ycxrx2pdawxkvpglgirh4s77ag24vomy"

export GSPC_HOME="$GPISPACE_ROOT"

SOURCE_DIR="~/Dokumente/gpispace/gspc-gromovwitten/template"

# Set FLINT home directory

# Set the library path (corrected syntax)
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$FLINT_HOME/lib:$GMP_HOME/lib:$SINGULAR_INSTALL_DIR/lib

# Set the DEP_LIBS variable (corrected syntax)
DEP_LIBS=$FLINT_HOME/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$GMP_HOME/lib

# # Sanity checks
# if [ ! -d "$SINGULAR_INSTALL_DIR" ]; then
#   echo "Error: Singular installation directory not found: $SINGULAR_INSTALL_DIR"
#   exit 1
# fi

# # Check for FLINT library existence
# if [ ! -f "$FLINT_HOME/lib/libflint.so" ]; then
#   echo "Error: FLINT library not found in $FLINT_HOME/lib"
#   exit 1
# fi

# if [ ! -d "$GPISPACE_ROOT" ]; then
#   echo "Error: GPI-Space root directory not found: $GPISPACE_ROOT"
#   exit 1
# fi

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
