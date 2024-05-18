#include <filesystem>
#include <fstream>
#include <sstream>
#include "conjugate_gradient.h"

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
        Matrix<double> a_dense;
        Vector<double> b;
        Vector<double> x;
        Prepare(a_sparse, b, 256);
        BENCHMARK("Size 256") {
            ConjugateGradient(a_sparse, b, x);
        };
    }
    {
        SparseMatrix<double> a_sparse;
        Matrix<double> a_dense;
        Vector<double> b;
        Vector<double> x;
        Prepare(a_sparse, b, 512);
        BENCHMARK("Size 512") {
            ConjugateGradient(a_sparse, b, x);
        };
    }
    {
        SparseMatrix<double> a_sparse;
        Matrix<double> a_dense;
        Vector<double> b;
        Vector<double> x;
        Prepare(a_sparse, b, 1024);
        BENCHMARK("Size 1024") {
            ConjugateGradient(a_sparse, b, x);
        };
    }
    {
        SparseMatrix<double> a_sparse;
        Matrix<double> a_dense;
        Vector<double> b;
        Vector<double> x;
        Prepare(a_sparse, b, 2048);
        BENCHMARK("Size 2048") {
            ConjugateGradient(a_sparse, b, x);
        };
    }
    {
        SparseMatrix<double> a_sparse;
        Matrix<double> a_dense;
        Vector<double> b;
        Vector<double> x;
        Prepare(a_sparse, b, 4096);
        BENCHMARK("Size 4096") {
            ConjugateGradient(a_sparse, b, x);
        };
    }
    {
        SparseMatrix<double> a_sparse;
        Matrix<double> a_dense;
        Vector<double> b;
        Vector<double> x;
        Prepare(a_sparse, b, 8192);
        BENCHMARK("Size 8192") {
            ConjugateGradient(a_sparse, b, x);
        };
    }
    {
        SparseMatrix<double> a_sparse;
        Matrix<double> a_dense;
        Vector<double> b;
        Vector<double> x;
        Prepare(a_sparse, b, 16384);
        BENCHMARK("Size 16384") {
            ConjugateGradient(a_sparse, b, x);
        };
    }
}
