# cp /home/atraore/gpi/try_gpi/Reduce/install_dir/share/examples/templategp.lib /home/atraore/gpi/try_gpi/Reduce/example_dir
# spack env activate -p gpispace
# SINGULARPATH=/home/atraore/gpi/try_gpi/Reduce/install_dir Singular example.sing




#!/bin/bash

# Ensure that LD_LIBRARY_PATH is set correctly for library resolution
export LD_LIBRARY_PATH=/home/atraore/gpi/try_gpi/Reduce/install_dir:$LD_LIBRARY_PATH

# Set the SINGULARPATH environment variable
export SINGULARPATH=/home/atraore/gpi/try_gpi/Reduce/install_dir

# Copy the example library to the example directory
cp /home/atraore/gpi/try_gpi/Reduce/install_dir/share/examples/templategp.lib /home/atraore/gpi/try_gpi/Reduce/example_dir
cd /home/atraore/gpi/try_gpi/Reduce/example_dir
rm -rf temp/*
spack load gpi-space@23.06

export PATH=/home/atraore/Singular4/bin:$PATH

# Run Singular with the example script
SINGULARPATH=/home/atraore/gpi/try_gpi/Reduce/install_dir Singular example.sing
