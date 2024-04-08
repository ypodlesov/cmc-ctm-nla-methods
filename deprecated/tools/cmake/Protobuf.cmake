find_package(Protobuf)

function(add_proto_library target_name proto_file)
  get_filename_component(ABS_FILE ${proto_file} ABSOLUTE)
  get_filename_component(FILE_WE ${proto_file} NAME_WE)

  set(PROTOC_GEN_RPC ${CMAKE_BINARY_DIR}/protoc-gen-rpc)
  set(HEADER ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.pb.h)
  set(SRC ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.pb.cc)

  add_custom_command(
    OUTPUT ${HEADER} ${SRC}
    COMMAND  ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
         --rpc_out=${CMAKE_CURRENT_BINARY_DIR}
         --plugin=protoc-gen-rpc=${PROTOC_GEN_RPC}
         --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
         ${ABS_FILE}
    DEPENDS ${ABS_FILE} ${Protobuf_PROTOC_EXECUTABLE} ${PROTOC_GEN_RPC}
    VERBATIM)

  add_library(${target_name} ${SRC})
  target_link_libraries(${target_name} PUBLIC cactus protobuf)
  target_include_directories(${target_name} PUBLIC ${CMAKE_BINARY_DIR}/tools)
endfunction()
