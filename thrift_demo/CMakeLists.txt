cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
IF(MSVC)
	set(WXN PRIVATE /W0) # /WX)
ELSEIF(UNIX)
	set(WXN PRIVATE -W -Wall -Wno-unused 
			-Wno-strict-aliasing -Wno-format-overflow  -Wno-int-in-bool-context 
			-Wno-sign-compare -Wno-reorder -Wextra -Wno-variadic-macros
			-Wno-comment
			-fPIC
			) 
ENDIF()


project(ThriftDemo)

#link to thrift
link_directories(/usr/local/lib/)

#create gen-cpp lib
file(GLOB gen_src gen-cpp/*.h gen-cpp/*.cpp)
message(${gen_src})

add_library(gencpp STATIC ${gen_src})
target_link_libraries(gencpp
    thrift
)
target_compile_options(gencpp ${WXN})
add_subdirectory(cpp)

