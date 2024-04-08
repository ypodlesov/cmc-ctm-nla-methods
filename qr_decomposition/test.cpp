#include "helpers.h"
#include "matrix.h"
#include "qr_decomposition.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

using Catch::Generators::random;
using Catch::Generators::take;

static void Test(const uint32_t n) {
    Matrix<double> q, r;
    Matrix<double> a;
    NHelpers::GenRandomMatrix(a, n, n, true);
    q = a;
    REQUIRE(q == a);
    REQUIRE(QRDecomposition(q, r));
    Matrix<double> c;
    Matrix<double>::MMMult(q, r, c);
    a.PlusAX(c, -1);
    double diff = a.Norm2();
    std::cout << diff << std::endl;
    constexpr double eps = 1e-6;
    REQUIRE(NHelpers::RoughEq(diff, 0.0, eps));
    REQUIRE(r.IsTriangular(NHelpers::ETriangularType::Upper));
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