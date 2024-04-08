#include "conjugate_gradient.h"
#include "helpers.h"
#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <cstdint>

using Catch::Generators::random;
using Catch::Generators::take;

static void Test(const uint32_t n) {
    constexpr double eps = 0.000001;
    std::srand(std::time(nullptr));
    auto a = SparseMatrix<double>(n, n, n << 1);
    NHelpers::GenRandom(a, true);
    Vector<double> b;
    b.GenRandom(n, true);
    Vector<double> x;
    REQUIRE(ConjugateGradient(a, b, x));
    Vector<double> result;
    a.VecMult(x, result);
    Vector<double> diff;
    diff.AXPlusBY(b, 1, result, 1);
    std::cout << diff.Norm2() << std::endl;
    REQUIRE(NHelpers::RoughEq<double, double>(diff.Norm2(), 0.0, eps));
    // std::srand(std::time(nullptr));
    // const TMatrix<double> A = TMatrix<double>::GenerateRandomSymmetricDefinite(n, EDefiniteness::Positive);
    // const TVector<double> b = TVector<double>::CreateRandom(n);
    // TVector<double> x;
    // ConjugateGradient(A, b, x);
    // TVector<double> result;
    // REQUIRE(TMatrix<double>::MVMultiply(A, x, result));
    // REQUIRE(RoughEq<double, double>(TVector<double>::Norm2(result - b), 0, 0.001));
}

TEST_CASE("Size 3") {
    Test(3);
}

// TEST_CASE("Size 512") {
//     Test(512);
// }

// TEST_CASE("Size 1024") {
//     Test(1024);
// }

// TEST_CASE("Size 2048") {
//     Test(2048);
// }

// TEST_CASE("Size 4096") {
//     Test(4096);
// }

// TEST_CASE("Size 8192") {
//     Test(8192);
// }