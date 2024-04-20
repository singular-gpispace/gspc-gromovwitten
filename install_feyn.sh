rm -r build/*
cmake \
  -D GPISpace_ROOT=/home/atraore/spack/ \
  -D GPISpace_ROOT=/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk \
  -D Boost_ROOT=/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/boost-1.63.0-q5xh5vlq3ihagc7f5ql4erhsofvamt6m \
  -D CMAKE_INSTALL_PREFIX=/home/atraore/gpi/try_gpi/gpispace/bin \
  -D Boost_NO_BOOST_CMAKE=TRUE \
  -B "/home/atraore/gpi/try_gpi/gpispace/build" \
  -S "/home/atraore/gpi/try_gpi/gpispace/"


cmake \
  --build "/home/atraore/gpi/try_gpi/gpispace/build" \
  --target install \
  -j $(nproc)


cmake --build "/home/atraore/gpi/try_gpi/gpispace/build" --target install -j $(nproc) >> ~/Dropbox/sage/error.txt 2>&1
cmake --build "/home/atraore/gpi/try_gpi/gpispace/build" --target install -j $(nproc) >> error.txt 2>&1


###############################################

./aggregate_sum \
  --gspc-home /home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/ \
  --nodefile /home/atraore/gpispace/nodefile \
  --rif-strategy ssh \
  --topology "worker:2" \
  --N 10000 \
  --log-host localhost \
  --log-port 6439



/home/atraore/gpi/try_gpi/gpispace/bin/bin/feynman \
  --gspc-home /home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/ \
  --nodefile /home/atraore/gpispace/nodefile \
  --rif-strategy ssh \
  --topology "worker:8" \
  --N 6 \
  --degree 4 \
  --graph "{{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}}" \
  --log-host localhost \
  --log-port 9876



/home/atraore/gpi/try_gpi/gpispace/bin/bin/feynman \
  --gspc-home /home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/ \
  --nodefile /home/atraore/gpispace/nodefile \
  --rif-strategy ssh \
  --topology "worker:8" \
  --N 3 \
  --degree 4 \
  --graph "{{1, 2}, {1, 2}, {1, 2}}" \
  --log-host localhost \
  --log-port 9876

