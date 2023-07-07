project(xy_thrift_example)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)

INCLUDE_DIRECTORIES(../src)

add_executable(xy_example xy_example.cpp)
target_link_libraries(
        xy_example
        xy_thrift_lib
)
