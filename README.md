```
# GromovWitten computation using GPI-Space

This package computes the generating series for tropical Hurwitz numbers of elliptic curves via mirror symmetry and Feynman integrals.

## Overview

The GromovWitten package computes generating series for tropical Hurwitz numbers of elliptic curves through mirror symmetry and Feynman integrals. Via a correspondence theorem, it also determines Hurwitz numbers in the context of algebraic geometry. Additionally, the package provides methods for calculating Gromov-Witten invariants for elliptic curves.

GromovWitten is implemented in C++ and XML, leveraging GPI-Space for Feynman integral computations.GromovWitten is written using C++ and XML which is used to compute the Feynman via [GPI-Space](https://www.gpi-space.de/)

# Installation

## Prerequisites

- C++ compiler
- GPI-Space
- flint

## Installation of GPI-Space and flint

To install GPI-Space and flint, we will install spack package manager and then install GPI-Space and flint.
follow the instructions [here](https://spack.readthedocs.io/en/latest/getting_started.html) to install spack.
After installing spack, we can install GPI-Space and flint.

```bash
spack install gpi-space@24.12
spack install flint
```

To install flint, please follow the instructions [here](https://github.com/wbhart/flint2/releases).

## Installation of GromovWitten

To install GromovWitten, we need to clone the repository.

```bash
git clone https://github.com/singular-gpispace/gspc-gromovwitten.git
```

After cloning the repository, we need to modify the `CMakeLists.txt`  and `install.sh` file to add the path to the GPI-Space and flint libraries.
You can launch the monitor of the GPI-Space by running the following command:

```bash
spack load gpi-space@24.12 # to load the GPI-Space
gspc-monitor --port 9876 & # to launch the monitor
```

Then we can run the `install.sh` file to run an exmaple.

```bash
./install.sh
```

```

```

