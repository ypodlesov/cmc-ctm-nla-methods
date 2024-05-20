add_library(BASIC_LA STATIC matrix.cpp common_matrix.cpp vector.cpp sparse_matrix.cpp matrix_powers_mv.cpp)
target_include_directories(BASIC_LA PUBLIC ${METIS_INCLUDE_PATH})
target_link_libraries(BASIC_LA PUBLIC ${METIS_LIB_PATH})