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

~/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/flint-3.1.2-nkrmyxtfk72g74hlkhu7pfnpjmihm2ti