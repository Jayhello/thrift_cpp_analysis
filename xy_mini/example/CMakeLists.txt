project(xy_thrift_example)

INCLUDE_DIRECTORIES(../src)

add_executable(xy_example xy_example.cpp)
target_link_libraries(
        xy_example
        xy_thrift_lib
)
