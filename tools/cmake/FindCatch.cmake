Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2
)

FetchContent_MakeAvailable(Catch2)

add_library(catch_main tools/catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)
