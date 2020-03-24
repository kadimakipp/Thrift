set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set(gen_head ${CMAKE_CURRENT_SOURCE_DIR}/../gen-cpp)
message(${gen_head})
include_directories(${gen_head})

add_executable(Server CppServer.cpp)
target_link_libraries(Server gencpp)
target_compile_options(Server ${WXN})


add_executable(Client CppClient.cpp)
target_link_libraries(Client gencpp)
target_compile_options(Client ${WXN})


