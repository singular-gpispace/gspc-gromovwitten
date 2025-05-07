rm -r ~/gpi/try_gpi/gspc-gromovwitten/build/* 
rm -r ~/gpi/try_gpi/gspc-gromovwitten/bin/*
# Generate SVG workflow diagram
spack load gpi-space@24.12
pnetc ~/gpi/try_gpi/gspc-gromovwitten/workflow/feynman.xpnet | pnet2dot | dot -T svg > ~/gpi/try_gpi/gspc-gromovwitten/workflow/feynman.svg

# Set GPISpace root path
GPISPACE_ROOT="/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5"
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



time ~/gpi/try_gpi/gspc-gromovwitten/bin/bin/feynman \
  --gspc-home $GPISPACE_ROOT \
  --nodefile ~/gpi/try_gpi/gspc-gromovwitten/nodefile \
  --rif-strategy ssh \
  --topology "worker:7" \
  --N 3 \
  --degree 6 \
  --genus 2 \
  --graph "{{1, 2}, {1, 2}, {1, 2}}" \
  --log-host localhost \
  --log-port 9876



# time ~/gpi/try_gpi/gspc-gromovwitten/bin/bin/feynman \
#   --gspc-home $GPISPACE_ROOT \
#   --nodefile ~/gpi/try_gpi/gspc-gromovwitten/nodefile \
#   --rif-strategy ssh \
#   --topology "worker:7" \
#   --N 6 \
#   --degree 4 \
#   --genus 3 \
#   --graph "{{1, 3}, {1, 2}, {1, 2}, {2, 4}, {3, 4}, {3, 4}}" \
#   --log-host localhost \
#   --log-port 9876



# time ~/gpi/try_gpi/gspc-gromovwitten/bin/bin/feynman \
#   --gspc-home $GPISPACE_ROOT \
#   --nodefile ~/gpi/try_gpi/gspc-gromovwitten/nodefile \
#   --rif-strategy ssh \
#   --topology "worker:7" \
#   --N 6 \
#   --degree 22 \
#   --genus 3 \
#   --graph "{{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}" \
#   --log-host localhost \
#   --log-port 9876


# time ~/gpi/try_gpi/gspc-gromovwitten/bin/bin/feynman \
#   --gspc-home $GPISPACE_ROOT \
#   --nodefile ~/gpi/try_gpi/gspc-gromovwitten/nodefile \
#   --rif-strategy ssh \
#   --topology "worker:7" \
#   --N 9 \
#   --degree 4 \
#   --graph "{ {1, 2}, {1, 2}, {1, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 6}, {5, 6}, {5, 6} }" \
#   --log-host localhost \
#   --log-port 9876


# cd $GPISPACE_ROOT/bin && ./gspc-logging-to-stdout.exe --port 9876 > ~/gpi/try_gpi/gspc-gromovwitten/monitor.txt 2>&1

# : > ~/gpi/try_gpi/gspc-gromovwitten/monitor.txt # erase the content of the file monitor.txt