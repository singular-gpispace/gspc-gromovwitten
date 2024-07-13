time ~/gpi/try_gpi/gpispace/bin/bin/feynman \
  --gspc-home ~/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/ \
  --nodefile ~/gpispace/nodefile \
  --rif-strategy ssh \
  --topology "worker:4" \
  --N 9 \
  --degree 4 \
  --graph "{{1,3},{1,2},{1,2},{2,4},{3,4},{3,5},{4,6},{5,6},{5,6}}" \
  --log-host localhost \
  --log-port 9876