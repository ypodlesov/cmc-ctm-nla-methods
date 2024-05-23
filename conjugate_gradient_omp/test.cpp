#include "conjugate_gradient_omp.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <matrix.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <cstdint>

static void Test(const uint32_t n) {

    std::stringstream file_name;
    file_name << std::filesystem::current_path().string() << "/../" << "matrix_examples/sparse_spd/" << n;
    std::ifstream fstream;
    fstream.open(file_name.str());
    REQUIRE(fstream.is_open());
    SparseMatrix<double> a;
    fstream >> a;
    fstream.close();
    REQUIRE(a.data_);
    Vector<double> b(n);
    NHelpers::GenRandomVector(b, n, true);

    Vector<double> x(n);
    REQUIRE(ConjugateGradientOMP(a, b, x));

    Vector<double> result(n);
    a.VecMult(x, result);
    result.PlusAX(b, -1);
    constexpr double eps = 0.001;
    std::cout << result.Norm2() << std::endl;
    REQUIRE(NHelpers::RoughEq<double, double>(result.Norm2(), 0.0, eps));
}

TEST_CASE("Size 16") {
    Test(16);
}

TEST_CASE("Size 128") {
    Test(128);
}

TEST_CASE("Size 256") {
    Test(256);
}

TEST_CASE("Size 512") {
    Test(512);
}

TEST_CASE("Size 1024") {
    Test(1024);
}