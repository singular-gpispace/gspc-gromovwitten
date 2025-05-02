
 link l="ssi:r temp/waas_314170_1730377827391274083";
 def m=read(l);
 m.r_data;
 def nr=m.r_data;
 setring nr;
 m.data[1];



cd ~/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5/bin && ./gspc-logging-to-stdout.exe --port 9876 >> ~/gpi/try_gpi/gspc-gromovwitten/monitor.txt 2>&1


link l="ssi:r tempbonsoir.ssi";
def m=read(l);



rm -f ~/gpi/try_gpi/gspc-gromovwitten/monitor.txt && touch ~/gpi/try_gpi/gspc-gromovwitten/monitor.txt && cd ~/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-24.12-jz6b4m6ql54fmhkpq6gbico2neic3kd5/bin && ./gspc-logging-to-stdout.exe --port 9876 >> ~/gpi/try_gpi/gspc-gromovwitten/monitor.txt 2>&1 & sleep 1 && tail -f ~/gpi/try_gpi/gspc-gromovwitten/monitor.txt