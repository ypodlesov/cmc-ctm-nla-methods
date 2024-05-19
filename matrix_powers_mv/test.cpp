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
        SparseMatrix<double> a_no_diag;
        a.RemoveDiag(a_no_diag);
        idx_t* perm = new idx_t[a_no_diag.row_cnt_];
        idx_t* iperm = new idx_t[a_no_diag.row_cnt_];
        idx_t options[METIS_NOPTIONS];
        METIS_SetDefaultOptions(options);
        options[METIS_OPTION_NUMBERING] = 0;
        // int METIS_NodeND(idx_t *nvtxs, idx_t *xadj, idx_t *adjncy, idx_t *vwgt,
        //   idx_t *options, idx_t *perm, idx_t *iperm);

        int result = METIS_NodeND(
            &a_no_diag.row_cnt_
            , a_no_diag.i_a_
            , a_no_diag.j_a_
            , nullptr
            , options
            , perm
            , iperm
        );
        REQUIRE(result == METIS_OK);
    }

    // REQUIRE(ReorderMatrix(a_no_diag) == METIS_OK);
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