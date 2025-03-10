
 link l="ssi:r temp/waas_314170_1730377827391274083";
 def m=read(l);
 m.r_data;
 def nr=m.r_data;
 setring nr;
 m.data[1];



cd ~/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/bin && ./gspc-logging-to-stdout.exe --port 9876  ~/gpi/try_gpi/Reduce/monitor.txt 2&1


link l="ssi:r tempbonsoir.ssi";
def m=read(l);


rm -r /users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/monitor.txt

cd /users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-23.06-g577g3pdx4rbi5o3den56mxyqnrdewia/bin&& \
./gspc-logging-to-stdout.exe --port 9876 \
    > /users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/monitor.txt 2>&1

