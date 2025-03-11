#!/bin/bash

# 🚀 **Step 1: Clean the build directory**
rm -rf ~/gpi/try_gpi/gpispace/build/*
rm -rf ~/gpi/try_gpi/gpispace/bin/*
rm -rf ~/gpi/try_gpi/gpispace/workflow/*

# 🛠 **Step 2: Set GPISpace and FLINT paths**
GPISPACE_ROOT="/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5"
export GSPC_HOME=$GPISPACE_ROOT

FLINT_HOME="/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/flint-2.6.3-pb3i4qjyjz7pqkpf6cs7wk6ro5pl564i"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/boost-1.63.0-6xz3jlconrhe7i2kz6tqvmom3huzco76/lib

spack load gpi-space@24.12

# 🚀 **Step 3: Configure CMake**
cmake \
  -D GPISpace_ROOT=~/spack/ \
  -D GSPC_WITH_MONITOR_APP=OFF \
  -D GPISpace_ROOT=$GPISPACE_ROOT \
  -D CMAKE_INSTALL_PREFIX=~/gpi/try_gpi/gpispace/bin \
  -D Boost_NO_BOOST_CMAKE=TRUE \
  -B "~/gpi/try_gpi/gpispace/build" \
  -S "~/gpi/try_gpi/gpispace/"

# 🚀 **Step 4: Build & Install**
cmake --build "~/gpi/try_gpi/gpispace/build" --target install -j$(nproc)

# 🚀 **Step 5: Ensure the module library is copied correctly**
LIB_PATH=~/gpi/try_gpi/gpispace/build/gen/pnetc/op
BIN_LIB_PATH=~/gpi/try_gpi/gpispace/bin/lib

if [ -f "$LIB_PATH/libfeynman.so" ]; then
    echo "✅ libfeynman.so found. Copying to correct locations..."
   # cp "$LIB_PATH/libfeynman.so" "$BIN_LIB_PATH/libfeynman_module.so"
    cp "$LIB_PATH/libfeynman.so" "$BIN_LIB_PATH/"
fi

pnetc /home/atraore/gpi/try_gpi/gpispace/workflow/feynman.xpnet | pnet2dot | dot -T svg > /home/atraore/gpi/try_gpi/gpispace/workflow/fey.svg

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BIN_LIB_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5/lib





# 🚀 **Step 10: Run the executable**
echo "🔄 Running feynman executable..."
time ~/gpi/try_gpi/gpispace/bin/bin/feynman \
  --gspc-home $GPISPACE_ROOT \
  --nodefile ~/gpispace/nodefile \
  --rif-strategy ssh \
  --topology "worker:7" \
  --input "{1,2,3}" \
  --P0  "1" \
  --log-host localhost \
  --log-port 9876 \
  2>&1 | tee ~/gpi/try_gpi/gpispace/workflow/feynman_run.log

echo "✅ feynman execution completed!"
