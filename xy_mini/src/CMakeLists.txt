project(xy_thrift_lib)

file(GLOB COMMON_SRCS ./*.cpp ./*.h)

add_library(xy_thrift_lib
        ${COMMON_SRCS}
        )