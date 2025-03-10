#!/bin/bash

# Change to the example directory
cd "/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/example_dir" || { echo "Failed to cd into example_dir"; exit 1; }

# Load the correct Singular from Spack
spack load singular@4.4.0p2
spack load gpi-space@24.12

# Set the correct Singular binary (absolute path)
SINGULAR_BIN="/net/nascip131/users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/singular-4.4.0p2-ycxrx2pdawxkvpglgirh4s77ag24vomy/bin/Singular"
GPISPACE_ROOT="/net/nascip131/users/cip/users/atraore/Dokumente/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-fjs7sy3f6o4dhm4i44ogigr7kszvw6oc"

export GSPC_HOME="$GPISPACE_ROOT"

if [ ! -x "$SINGULAR_BIN" ]; then
    echo "Error: Singular not found at $SINGULAR_BIN"
    exit 1
fi
echo "Using Singular from: $SINGULAR_BIN"

# Set SINGULARPATH to the installation directory (use absolute path)
SINGULARPATH="/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/install_dir"
if [ ! -d "$SINGULARPATH" ]; then
    echo "Error: SINGULARPATH directory does not exist: $SINGULARPATH"
    exit 1
fi
echo "SINGULARPATH is set to: $SINGULARPATH"

# Ensure that the workflow and lib directories are in LD_LIBRARY_PATH
export LD_LIBRARY_PATH="$SINGULARPATH/libexec/workflow:$SINGULARPATH/lib:$LD_LIBRARY_PATH"
echo "LD_LIBRARY_PATH is: $LD_LIBRARY_PATH"

SOURCE_SO="/users/cip/users/atraore/Dokumente/gpispace/gspc-gromovwitten/build_dir/src/libSINGULAR-template-module.so"
TARGET_SO="$SINGULARPATH/libexec/workflow/libSINGULAR-template-module.so"
if [ -f "$SOURCE_SO" ]; then
    echo "Copying libSINGULAR-template-module.so to workflow directory..."
    cp -f "$SOURCE_SO" "$TARGET_SO"
    chmod +x "$TARGET_SO"
else
    echo "Warning: Source .so file not found at $SOURCE_SO"
fi

# Verify that the target .so file exists
if [ ! -f "$TARGET_SO" ]; then
    echo "Error: Target .so file not found at $TARGET_SO"
    exit 1
fi
ls -lh "$TARGET_SO"

# Finally, run Singular with the example script using the correct binary.
"$SINGULAR_BIN" example.sing
