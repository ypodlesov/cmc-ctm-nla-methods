#include "helpers.h"
#include <matrix_powers_mv.h>
#include <sparse_matrix.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <cstdint>

static void Test(const int64_t n) {
    SparseMatrix<double> a_no_diag;
    SparseMatrix<double> a;
    {
        std::stringstream file_name;
        file_name << std::filesystem::current_path().string() << "/../" << "matrix_examples/sparse_spd/" << n;
        std::ifstream fstream;
        fstream.open(file_name.str());
        REQUIRE(fstream.is_open());
        fstream >> a;
        fstream.close();
        REQUIRE(a.data_);
    }
    Vector<double> x(n);
    NHelpers::GenRandomVector(x, n, true);
    int64_t s = std::min<int64_t>(n, 1 << 10);
    std::vector<Vector<double>> result(s);
    for (auto& single_res : result) {
        single_res = Vector<double>(n);
    }
    REQUIRE(MatrixPowersMV(a, x, result));
    REQUIRE(static_cast<int64_t>(result.size()) == s);

    constexpr double eps = 1e-6;
    Vector<double> last_res(x);
    for (int64_t i = 1; i < s; ++i) {
        Vector<double> local_res(n);
        a.VecMult(last_res, local_res);
        last_res = local_res;
        local_res.PlusAX(result[i], -1);
        REQUIRE(NHelpers::RoughEq(local_res.Norm2(), 0.0, eps));
    }
}

TEST_CASE("Size 4") {
    Test(4);
}

// TEST_CASE("Size 128") {
//     Test(128);
// }

// TEST_CASE("Size 256") {
//     Test(256);
// }

// TEST_CASE("Size 512") {
//     Test(512);
// }

// TEST_CASE("Size 1024") {
//     Test(1024);
// }

// TEST_CASE("Size 2048") {
//     Test(2048);
// }