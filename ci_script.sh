#!/bin/bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 16 all
sudo apt-get install git gdb cmake ninja-build clang-format clang-tidy libtbb-dev
mkdir build
cd build
cmake ..
make test_qr_decomposition
./test_qr_decomposition
make test_conjugate_gradient
./test_conjugate_gradient