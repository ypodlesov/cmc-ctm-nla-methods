cmake_minimum_required(VERSION 3.14)
project(cmc_ctm_matrices)

set(CMAKE_CXX_STANDARD             20)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/tools/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

find_package(Catch REQUIRED)

set(Clang_DIR "/usr/lib/llvm-16/lib/cmake/clang")
set(LLVM_DIR "/usr/lib/llvm-16/lib/cmake/llvm")
find_package(Clang CONFIG)

include_directories(tools/commons)
include(tools/cmake/TestSolution.cmake)
include(tools/cmake/BuildFlags.cmake)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/private)
  add_subdirectory(private)
endif()

function(add_if_exists name)
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${name})
    add_subdirectory(${name})
  endif()
endfunction()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
set(DEPS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/.deps)
set(BASIC_LA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/basic_la)
set(METIS_INCLUDE_PATH ${DEPS_PATH}/include)
set(METIS_LIB_PATH ${DEPS_PATH}/lib/libmetis.so)

add_if_exists(basic_la)
add_if_exists(qr_decomposition)
add_if_exists(conjugate_gradient)
add_if_exists(conjugate_gradient_parallel)
add_if_exists(conjugate_gradient_omp)

add_if_exists(matrix_powers_mv)