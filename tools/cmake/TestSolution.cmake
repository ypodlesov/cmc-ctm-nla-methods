option(GRADER "Building on CI" OFF)

function(add_executable_custom NAME)
  add_executable(${NAME} ${ARGN})
  set_target_properties(${NAME} PROPERTIES COMPILE_FLAGS "-Wall -Werror -Wextra -Wpedantic")
endfunction()

function(add_catch TARGET)
  add_executable_custom(${TARGET} ${ARGN})
  target_link_libraries(${TARGET} PRIVATE catch_main)
endfunction()
