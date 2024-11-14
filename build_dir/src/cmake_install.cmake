# Install script for directory: /home/atraore/gpi/try_gpi/Reduce/template/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/atraore/gpi/try_gpi/Reduce/install_dir")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so"
         RPATH "/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/libexec/iml:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/boost-1.63.0-q5xh5vlq3ihagc7f5ql4erhsofvamt6m/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/lib:/usr/lib/gcc/x86_64-linux-gnu/11:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/atraore/gpi/try_gpi/Reduce/build_dir/src/libSINGULAR-template-installation.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so"
         OLD_RPATH "/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/libexec/iml:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/boost-1.63.0-q5xh5vlq3ihagc7f5ql4erhsofvamt6m/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/lib:/usr/lib/gcc/x86_64-linux-gnu/11:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib:"
         NEW_RPATH "/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/libexec/iml:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/boost-1.63.0-q5xh5vlq3ihagc7f5ql4erhsofvamt6m/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/lib:/usr/lib/gcc/x86_64-linux-gnu/11:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/atraore/gpi/try_gpi/Reduce/install_dir/libSINGULAR-template-module.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/atraore/gpi/try_gpi/Reduce/install_dir/libSINGULAR-template-module.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/atraore/gpi/try_gpi/Reduce/install_dir/libSINGULAR-template-module.so"
         RPATH "\$ORIGIN/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/libexec/iml:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/boost-1.63.0-q5xh5vlq3ihagc7f5ql4erhsofvamt6m/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/lib:/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/singular-snapshot_22_03-5jvwtprazqirywu2triw6rprjazzi3so/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gmp-6.2.1-u7pgmurjsjr7b4bbxlt3ra4tdux3hdrc/lib:/usr/lib/gcc/x86_64-linux-gnu/11:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/atraore/gpi/try_gpi/Reduce/install_dir/libSINGULAR-template-module.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/atraore/gpi/try_gpi/Reduce/install_dir" TYPE SHARED_LIBRARY FILES "/home/atraore/gpi/try_gpi/Reduce/build_dir/src/libSINGULAR-template-module.so")
  if(EXISTS "$ENV{DESTDIR}/home/atraore/gpi/try_gpi/Reduce/install_dir/libSINGULAR-template-module.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/atraore/gpi/try_gpi/Reduce/install_dir/libSINGULAR-template-module.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/atraore/gpi/try_gpi/Reduce/install_dir/libSINGULAR-template-module.so"
         OLD_RPATH "/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/libexec/iml:/home/atraore/gpi/try_gpi/Reduce/build_dir/src:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/boost-1.63.0-q5xh5vlq3ihagc7f5ql4erhsofvamt6m/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/lib:/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/singular-snapshot_22_03-5jvwtprazqirywu2triw6rprjazzi3so/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gmp-6.2.1-u7pgmurjsjr7b4bbxlt3ra4tdux3hdrc/lib:/usr/lib/gcc/x86_64-linux-gnu/11:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib:"
         NEW_RPATH "\$ORIGIN/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/libexec/iml:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/boost-1.63.0-q5xh5vlq3ihagc7f5ql4erhsofvamt6m/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gpi-space-23.06-gxye6b7ngsnbxnzjkfsfqtvanynyghdk/lib:/home/atraore/singular-gpispace/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/singular-snapshot_22_03-5jvwtprazqirywu2triw6rprjazzi3so/lib:/home/atraore/spack/opt/spack/linux-ubuntu22.04-skylake/gcc-11.3.0/gmp-6.2.1-u7pgmurjsjr7b4bbxlt3ra4tdux3hdrc/lib:/usr/lib/gcc/x86_64-linux-gnu/11:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/atraore/gpi/try_gpi/Reduce/install_dir/libSINGULAR-template-module.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

