#include <filesystem>
#include <fstream>
#include <sstream>
#include "conjugate_gradient_parallel.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

void Prepare(SparseMatrix<double>& a, Vector<double>& b, const size_t n) {
    std::stringstream file_name;
    file_name << std::filesystem::current_path().string() << "/../" << "matrix_examples/sparse_spd/" << n;
    std::ifstream fstream;
    fstream.open(file_name.str());
    REQUIRE(fstream.is_open());
    fstream >> a;
    fstream.close();
    REQUIRE(a.data_);
    NHelpers::GenRandomVector(b, n, true);
    Vector<double> x(n);
}

TEST_CASE("Benchmark CG") {
    {
        SparseMatrix<double> a_sparse;
        Vector<double> b;
        Vector<double> x;
        Prepare(a_sparse, b, 4096);
        BENCHMARK("Size 4096") {
            ConjugateGradientParallel(a_sparse, b, x);
        };
    }
    {
        SparseMatrix<double> a_sparse;
        Vector<double> b;
        Vector<double> x;
        Prepare(a_sparse, b, 8192);
        BENCHMARK("Size 8192") {
            ConjugateGradientParallel(a_sparse, b, x);
        };
    }
    {
        SparseMatrix<double> a_sparse;
        Vector<double> b;
        Vector<double> x;
        Prepare(a_sparse, b, 16384);
        BENCHMARK("Size 16384") {
            ConjugateGradientParallel(a_sparse, b, x);
        };
    }
}
