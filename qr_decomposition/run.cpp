#include "qr_decomposition.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

TEST_CASE("Benchmark") {
    {
        size_t n = 512;
        SquareMatrix<double> q, r;
        SquareMatrix<double> a(n);
        a.GenRandom(n * n, true);
        q = a;
        BENCHMARK("Size 512") {
            REQUIRE(QRDecomposition(q, r));
        };
    }
    {
        size_t n = 1024;
        SquareMatrix<double> q, r;
        SquareMatrix<double> a(n);
        a.GenRandom(n * n, true);
        q = a;
        BENCHMARK("Size 1024") {
            REQUIRE(QRDecomposition(q, r));
        };
    }
    {
        size_t n = 2048;
        SquareMatrix<double> q, r;
        SquareMatrix<double> a(n);
        a.GenRandom(n * n, true);
        q = a;
        BENCHMARK("Size 2048") {
            REQUIRE(QRDecomposition(q, r));
        };
    }
}
