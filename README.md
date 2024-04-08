# Basic matrix algorithms

This repository is devoted to study course of computational linear algebra in  **MSU-CMC-CTM** (Moscow State University, Faculty of Computational Mathematics and Cybernetics, Department of Computational Technologies and Modeling).

Here you can also find use-examples of some instruments like **conan package manager**, **google-test**, **google-benchmark** and maybe something else.

## Project file tree

`git ls-tree -r --name-only HEAD | tree --fromfile`

```bash
cmc-ctm-matrix-algos
├── basic_la
│   ├── CMakeLists.txt
│   ├── helpers.h
│   ├── matrix.cpp
│   ├── matrix.h
│   ├── vector.cpp
│   └── vector.h
├── CMakeLists.txt
├── conjugate_gradient
│   ├── bench_results.txt
│   ├── CMakeLists.txt
│   ├── conjugate_gradient.cpp
│   ├── conjugate_gradient.h
│   ├── README.md
│   ├── run.cpp
│   └── test.cpp
├── .gitignore
├── qr_decomposition
│   ├── CMakeLists.txt
│   ├── qr_decomposition.cpp
│   ├── qr_decomposition.h
│   ├── run.cpp
│   └── test.cpp
├── README.md
└── tools
    ├── catch_main.cpp
    ├── cmake
    │   ├── BuildFlags.cmake
    │   ├── FindCatch.cmake
    │   ├── FindFFTW.cmake
    │   ├── Protobuf.cmake
    │   └── TestSolution.cmake
    └── commons
        ├── run_channel.h
        ├── runner.h
        ├── test_channel.h
        └── util.h
```