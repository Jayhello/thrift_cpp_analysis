project(xy_thrift_lib)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)

file(GLOB COMMON_SRCS ./*.cpp ./*.h)

add_library(xy_thrift_lib
        ${COMMON_SRCS}
        )