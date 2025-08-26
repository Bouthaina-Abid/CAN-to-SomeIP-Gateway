cmake_minimum_required(VERSION 3.5)
project(vsomeip_gateway)

set(CMAKE_CXX_STANDARD 17)

# Ajouter les includes de vsomeip (si installés dans ../install)
include_directories(../install/include)

# Chercher les bibliothèques dans ../install/lib
link_directories(../install/lib)

add_executable(vsomeip_gateway
    main.cpp
    can_socket.cpp
    someip_gateway.cpp
)

# Lier les bibliothèques nécessaires
target_link_libraries(vsomeip_gateway
    vsomeip3
    pthread
    dl
)
