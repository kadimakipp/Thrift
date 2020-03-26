set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set(gen_head ${CMAKE_CURRENT_SOURCE_DIR}/../gen-cpp)
include_directories(${gen_head})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#add_definitions(-DIMageTest)

add_executable(Server CppServer.cpp ktimer.h)
target_link_libraries(Server 
        gencpp
        ${OpenCV_LIBS})
target_compile_options(Server ${WXN})


add_executable(Client CppClient.cpp ktimer.h)
target_link_libraries(Client 
        gencpp
        ${OpenCV_LIBS})
target_compile_options(Client ${WXN})


