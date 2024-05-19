#include "matrix_powers_mv.h"
#include "sparse_matrix.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <cstdint>

static void Test(const uint32_t n) {
    SparseMatrix<double> a_no_diag;
    {
        std::stringstream file_name;
        file_name << std::filesystem::current_path().string() << "/../" << "matrix_examples/sparse_spd/" << n;
        std::ifstream fstream;
        fstream.open(file_name.str());
        REQUIRE(fstream.is_open());
        SparseMatrix<double> a;
        fstream >> a;
        fstream.close();
        REQUIRE(a.data_);
    }

    REQUIRE(result == METIS_OK);
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