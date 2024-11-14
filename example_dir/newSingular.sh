#!/bin/bash

# Set up environment
#export LD_LIBRARY_PATH=/home/atraore/gpi/try_gpi/Reduce/install_dir:$LD_LIBRARY_PATH
export SINGULAR_HOME=/home/gnawali/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.4.0/singular-latest-tlsjiiw3fxdlbammb2k2zoqi46awr6w2

# Activate Spack environment
spack env activate -p gpispace

# Ensure the example library file is copied to the working directory
cp /home/atraore/gpi/try_gpi/Reduce/install_dir/share/examples/templategp.lib /home/atraore/gpi/try_gpi/Reduce/example_dir
$SINGULAR_HOME/lib
# Run Singular with the specified script
$SINGULAR_HOME/bin/Singular example.sing