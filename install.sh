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
  --N 3 \
  --degree 6 \
  --genus 2 \
  --graph "{{1, 2}, {1, 2}, {1, 2}}" \
  --log-host localhost \
  --log-port 9876




time ~/gpi/try_gpi/gpispace/bin/bin/feynman \
  --gspc-home ~/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/ \
  --nodefile ~/gpispace/nodefile \
  --rif-strategy ssh \
  --topology "worker:7" \
  --N 6 \
  --degree 4 \
  --genus 3 \
  --graph "{{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}}" \
  --log-host localhost \
  --log-port 9876

spack load gpi-space@23.06


time ~/gpi/try_gpi/gpispace/bin/bin/feynman \
  --gspc-home ~/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/ \
  --nodefile ~/gpispace/nodefile \
  --rif-strategy ssh \
  --topology "worker:7" \
  --N 6 \
  --degree 22 \
  --genus 3 \
  --graph "{{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}" \
  --log-host localhost \
  --log-port 9876


time ~/gpi/try_gpi/gpispace/bin/bin/feynman \
  --gspc-home ~/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/ \
  --nodefile ~/gpispace/nodefile \
  --rif-strategy ssh \
  --topology "worker:7" \
  --N 9 \
  --degree 4 \
  --graph "{ {1, 2}, {1, 2}, {1, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 6}, {5, 6}, {5, 6} }" \
  --log-host localhost \
  --log-port 9876


cd ~/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/bin && ./gspc-logging-to-stdout.exe --port 9876 > ~/gpi/try_gpi/gpispace/monitor.txt 2>&1

: > monitor.txt # erase the content of the file monitor.txt