cmake_minimum_required(VERSION 3.14)

project(SilKitSomeIpGateway LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SilKit and vsomeip paths
set(SILKIT_INCLUDE_DIR "../../install/include")
set(SILKIT_LIB_DIR "../../install/lib")
set(VSOMEIP_INCLUDE_DIR "../../install_folder/include")
set(VSOMEIP_LIB_DIR "../../install_folder/lib")

# Common headers (optional)
set(COMMON_HEADERS "")

# GatewayNotifier executable
add_executable(GatewayNotifier
    src/main.cpp
    src/GatewayApp.cpp
    src/SomeIPGateway.cpp
    ${COMMON_HEADERS}
)
target_include_directories(GatewayNotifier PRIVATE 
    ${SILKIT_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIR}
    ../../sil-kit/Demos/communication/include
    ../../sil-kit/Demos/communication/Can
)
target_link_directories(GatewayNotifier PRIVATE 
    ${SILKIT_LIB_DIR}
    ${VSOMEIP_LIB_DIR}
)
target_link_libraries(GatewayNotifier PRIVATE 
    pthread
    SilKit
    vsomeip3
    vsomeip3-cfg
    vsomeip3-e2e
    vsomeip3-sd
)

# SOME/IP client executable
add_executable(someip_client
    src_client/main.cpp
    src_client/SomeIPClient.cpp
)
target_include_directories(someip_client PRIVATE 
    ${VSOMEIP_INCLUDE_DIR}
)
target_link_directories(someip_client PRIVATE 
    ${VSOMEIP_LIB_DIR}
)
target_link_libraries(someip_client PRIVATE 
    pthread
    vsomeip3
    vsomeip3-cfg
    vsomeip3-e2e
    vsomeip3-sd
)
