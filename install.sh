rm -r ~/gpi/try_gpi/gpispace/build/*
cmake \
  -D GPISpace_ROOT=~/spack/ \
 -D GSPC_WITH_MONITOR_APP=OFF \
  -D GPISpace_ROOT=~/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk \
  -D Boost_ROOT=~/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/boost-1.63.0-q5xh5vlq3ihagc7f5ql4erhsofvamt6m \
  -D CMAKE_INSTALL_PREFIX=~/gpi/try_gpi/gpispace/bin \
  -D Boost_NO_BOOST_CMAKE=TRUE \
  -B "~/gpi/try_gpi/gpispace/build" \
  -S "~/gpi/try_gpi/gpispace/"


cmake \
  --build "~/gpi/try_gpi/gpispace/build" \
  --target install \
  -j $(nproc)


time ~/gpi/try_gpi/gpispace/bin/bin/feynman \
  --gspc-home ~/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/ \
  --nodefile ~/gpispace/nodefile \
  --rif-strategy ssh \
  --topology "worker:7" \
  --m 7\
  --N 6 \
  --degree 14 \
  --graph "{{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}}" \
  --log-host localhost \
  --log-port 9876

spack load gpi-space@23.06