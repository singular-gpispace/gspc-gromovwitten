ssh cipserv05

spack load gpi-space@23.06
rm -r /users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/build/*
cmake \
  -D GPISpace_ROOT=~/spack/ \
 -D GSPC_WITH_MONITOR_APP=OFF \
  -D GPISpace_ROOT=/net/nascip131/users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-23.06-g577g3pdx4rbi5o3den56mxyqnrdewia \
  -D Boost_ROOT=/net/nascip131/users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/boost-1.63.0-dngif4szxmkdhl6om57hiczdkg6d44ww \
  -D CMAKE_INSTALL_PREFIX=/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/bin \
  -D Boost_NO_BOOST_CMAKE=TRUE \
  -B "/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/build" \
  -S "/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/"


cmake \
  --build "/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/build" \
  --target install \
  -j $(nproc)


time /users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/bin/bin/feynman \
  --gspc-home /net/nascip131/users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-23.06-g577g3pdx4rbi5o3den56mxyqnrdewia \
  --nodefile /users/cip/users/atraore/Dokumente/gpispace/nodefile \
  --rif-strategy ssh \
  --topology "worker:100" \
  --m 1 \
  --N 6 \
  --degree 14 \
  --graph "{{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}}" \
  --log-host localhost \
  --log-port 9876


rm -r /users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/monitor.txt

cd /net/nascip131/users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-23.06-g577g3pdx4rbi5o3den56mxyqnrdewia/bin&& \
./gspc-logging-to-stdout.exe --port 9876 \
    > /users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/monitor.txt 2>&1
