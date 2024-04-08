#include "matrix.h"
#include "qr_decomposition.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

using Catch::Generators::random;
using Catch::Generators::take;

static void Test(const uint32_t n) {
    SquareMatrix<double> q, r;
    SquareMatrix<double> a(n);
    a.GenRandom(n * n, true);
    q = a;
    REQUIRE(QRDecomposition(q, r));
    SquareMatrix<double> c;
    SquareMatrix<double>::MMMult(q, r, c);
    REQUIRE(a == c);
    REQUIRE(r.IsTriangular(NHelpers::ETriangularType::Upper));
}

TEST_CASE("Size 10") {
    Test(10);
}

TEST_CASE("Size 100") {
    Test(100);
}