#!/bin/bash

# Navigate to example directory
cd /home/atraore/gpi/try_gpi/Reduce/example_dir

# Remove temporary files
rm -rf temp/*

# Load Spack and GPI-Space
spack load gpi-space@24.12

# Set paths
export GSPC_HOME=/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5
export PATH=/home/atraore/Singular4/bin:$PATH

# Set FLINT and Library Paths
FLINT_HOME="/usr/local/"
export LD_LIBRARY_PATH=$GSPC_HOME/lib:$FLINT_HOME/lib:/home/atraore/git/Singular4/flint/flint2:$LD_LIBRARY_PATH

# Ensure Singular is loaded
spack load singular

# Run Singular with the example script
export SINGULARPATH=/home/atraore/gpi/try_gpi/Reduce/install_dir
Singular example.sing
