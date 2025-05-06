rm -r ~/gpi/try_gpi/gspc-gromovwitten/build/* 
rm -r ~/gpi/try_gpi/gspc-gromovwitten/bin/*

# # Compile and run generate_fey.cpp to generate new feynman.xpnet
#  cd "~/gpi/try_gpi/gspc-gromovwitten/cpp/" && g++ generate_fey.cpp -o generate_fey && "~/gpi/try_gpi/gspc-gromovwitten/cpp/"generate_fey
# mv ~/gpi/try_gpi/gspc-gromovwitten/cpp/feynman.xpnet ~/gpi/try_gpi/gspc-gromovwitten/workflow/feynman.xpnet

# Generate SVG workflow diagram
spack load gpi-space@24.12
pnetc ~/gpi/try_gpi/gspc-gromovwitten/workflow/feynman.xpnet | pnet2dot | dot -T svg > ~/gpi/try_gpi/gspc-gromovwitten/workflow/feynman.svg

# Set GPISpace root path
GPISPACE_ROOT="~/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5"
export GSPC_HOME=$GPISPACE_ROOT

cmake \
  -D GPISpace_ROOT=$GPISPACE_ROOT \
  -D GSPC_WITH_MONITOR_APP=OFF \
  -D Boost_ROOT=$GPISPACE_ROOT \
  -D CMAKE_INSTALL_PREFIX=~/gpi/try_gpi/gspc-gromovwitten/bin \
  -D Boost_NO_BOOST_CMAKE=TRUE \
  -B "~/gpi/try_gpi/gspc-gromovwitten/build" \
  -S "~/gpi/try_gpi/gspc-gromovwitten/"


cmake \
  --build "~/gpi/try_gpi/gspc-gromovwitten/build" \
  --target install \
  -j $(nproc)


# time ~/gpi/try_gpi/gspc-gromovwitten/bin/bin/feynman \
#   --gspc-home ~/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5/ \
#   --nodefile ~/gpi/try_gpi/gspc-gromovwitten/nodefile \
#   --rif-strategy ssh \
#   --topology "worker:7" \
#   --N 6 \
#   --degree 4 \
#   --graph "{{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}}" \
#   --log-host localhost \
#   --log-port 9876


time ~/gpi/try_gpi/gspc-gromovwitten/bin/bin/feynman \
  --gspc-home ~/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5/ \
  --nodefile ~/gpi/try_gpi/gspc-gromovwitten/nodefile \
  --rif-strategy ssh \
  --topology "worker:7" \
  --N 6 \
  --degree 4 \
  --graph "{{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}" \
  --log-host localhost \
  --log-port 9876


# time ~/gpi/try_gpi/gspc-gromovwitten/bin/bin/feynman \
#   --gspc-home ~/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5/ \
#   --nodefile ~/gpi/try_gpi/gspc-gromovwitten/nodefile \
#   --rif-strategy ssh \
#   --topology "worker:7" \
#   --N 9 \
#   --degree 4 \
#   --graph "{ {1, 2}, {1, 2}, {1, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 6}, {5, 6}, {5, 6} }" \
#   --log-host localhost \
#   --log-port 9876
