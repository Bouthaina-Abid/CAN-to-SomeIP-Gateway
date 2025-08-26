
cmake_minimum_required(VERSION 3.14)

project(CanDemo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SilKit paths
set(SILKIT_INCLUDE_DIR "../../../../install/include")
set(SILKIT_LIB_DIR "../../../../install/lib")

# Common headers
set(COMMON_HEADERS CanDemoCommon.hpp)

# CanWriterDemo
add_executable(CanWriterDemo CanWriterDemo.cpp ${COMMON_HEADERS})
target_include_directories(CanWriterDemo PRIVATE ${SILKIT_INCLUDE_DIR} ../SilKit)
target_link_directories(CanWriterDemo PRIVATE ${SILKIT_LIB_DIR})
target_link_libraries(CanWriterDemo pthread SilKit)

# CanReaderDemo
add_executable(CanReaderDemo CanReaderDemo.cpp ${COMMON_HEADERS})
target_include_directories(CanReaderDemo PRIVATE ${SILKIT_INCLUDE_DIR} ../SilKit)
target_link_directories(CanReaderDemo PRIVATE ${SILKIT_LIB_DIR})
target_link_libraries(CanReaderDemo pthread SilKit)