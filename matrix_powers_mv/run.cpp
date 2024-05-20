#include <filesystem>
#include <fstream>
#include <sstream>
#include <matrix_powers_mv.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

void PrepareMatrix(SparseMatrix<double>& a, const size_t n) {
    std::stringstream file_name;
    file_name << std::filesystem::current_path().string() << "/../" << "matrix_examples/sparse_spd/" << n;
    std::ifstream fstream;
    fstream.open(file_name.str());
    REQUIRE(fstream.is_open());
    fstream >> a;
    fstream.close();
    REQUIRE(a.data_);
}

TEST_CASE("Benchmark MatrixPowers kernel") {
    {
        constexpr int64_t n = 1024;
        constexpr int64_t s = 1024;
        SparseMatrix<double> a_sparse;
        PrepareMatrix(a_sparse, n);
        std::vector<Vector<double>> result(s);
        for (auto& single_res : result) {
            single_res = Vector<double>(n);
        }
        Vector<double> b;
        NHelpers::GenRandomVector(b, n, true);
        BENCHMARK("MatrixPowersMV with s=1024, size=1024:") {
            MatrixPowersMV(a_sparse, b, result);
        };

        BENCHMARK("Sequential MatVec's with s=1024, size=1024:") {
            Vector<double> cur_vector = b;
            for (int64_t i = 1; i < s; ++i) {
                Vector<double> cur_result(n);
                a_sparse.VecMult(cur_vector, cur_result);
                cur_vector = std::move(cur_result);
            }
        };
    }
    {
        constexpr int64_t n = 2048;
        constexpr int64_t s = 2048;
        SparseMatrix<double> a_sparse;
        PrepareMatrix(a_sparse, n);
        std::vector<Vector<double>> result(s);
        for (auto& single_res : result) {
            single_res = Vector<double>(n);
        }
        Vector<double> b;
        NHelpers::GenRandomVector(b, n, true);
        BENCHMARK("MatrixPowersMV with s=2048, size=2048:") {
            MatrixPowersMV(a_sparse, b, result);
        };

        BENCHMARK("Sequential MatVec's with s=2048, size=2048:") {
            Vector<double> cur_vector = b;
            for (int64_t i = 1; i < s; ++i) {
                Vector<double> cur_result(n);
                a_sparse.VecMult(cur_vector, cur_result);
                cur_vector = std::move(cur_result);
            }
        };
    }
    {
        constexpr int64_t n = 4096;
        constexpr int64_t s = 4096;
        SparseMatrix<double> a_sparse;
        PrepareMatrix(a_sparse, n);
        std::vector<Vector<double>> result(s);
        for (auto& single_res : result) {
            single_res = Vector<double>(n);
        }
        Vector<double> b;
        NHelpers::GenRandomVector(b, n, true);
        BENCHMARK("MatrixPowersMV with s=4096, size=4096:") {
            MatrixPowersMV(a_sparse, b, result);
        };

        BENCHMARK("Sequential MatVec's with s=4096, size=4096:") {
            Vector<double> cur_vector = b;
            for (int64_t i = 1; i < s; ++i) {
                Vector<double> cur_result(n);
                a_sparse.VecMult(cur_vector, cur_result);
                cur_vector = std::move(cur_result);
            }
        };
    }
    {
        constexpr int64_t n = 8192;
        constexpr int64_t s = 8192;
        SparseMatrix<double> a_sparse;
        PrepareMatrix(a_sparse, n);
        std::vector<Vector<double>> result(s);
        for (auto& single_res : result) {
            single_res = Vector<double>(n);
        }
        Vector<double> b;
        NHelpers::GenRandomVector(b, n, true);
        BENCHMARK("MatrixPowersMV with s=8192, size=8192:") {
            MatrixPowersMV(a_sparse, b, result);
        };

        BENCHMARK("Sequential MatVec's with s=8192, size=8192:") {
            Vector<double> cur_vector = b;
            for (int64_t i = 1; i < s; ++i) {
                Vector<double> cur_result(n);
                a_sparse.VecMult(cur_vector, cur_result);
                cur_vector = std::move(cur_result);
            }
        };
    }
    {
        constexpr int64_t n = 16384;
        constexpr int64_t s = 16384;
        SparseMatrix<double> a_sparse;
        PrepareMatrix(a_sparse, n);
        std::vector<Vector<double>> result(s);
        for (auto& single_res : result) {
            single_res = Vector<double>(n);
        }
        Vector<double> b;
        NHelpers::GenRandomVector(b, n, true);
        BENCHMARK("MatrixPowersMV with s=16384, size=16384:") {
            MatrixPowersMV(a_sparse, b, result);
        };

        BENCHMARK("Sequential MatVec's with s=16384, size=16384:") {
            Vector<double> cur_vector = b;
            for (int64_t i = 1; i < s; ++i) {
                Vector<double> cur_result(n);
                a_sparse.VecMult(cur_vector, cur_result);
                cur_vector = std::move(cur_result);
            }
        };
    }
}
