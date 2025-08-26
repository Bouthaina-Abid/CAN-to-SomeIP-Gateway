cmake_minimum_required (VERSION 3.13)

set (CMAKE_CXX_FLAGS "-g -std=c++0x")

find_package (vsomeip3 3.4.10 REQUIRED)
find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )

include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
)

add_executable(service-example ../src/service-example.cpp)
target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})

add_executable(service-example-availability ../src/service-example-availability.cpp)
target_link_libraries(service-example-availability vsomeip3 ${Boost_LIBRARIES})

add_executable(client-example-availability ../src/client-example-availability.cpp)
target_link_libraries(client-example-availability vsomeip3 ${Boost_LIBRARIES})

add_executable(test_service ../src/test_service.cpp)
target_link_libraries(test_service vsomeip3 ${Boost_LIBRARIES})

add_executable(test_client ../src/test_client.cpp)
target_link_libraries(test_client vsomeip3 ${Boost_LIBRARIES})

add_executable(test1 ../src/test1.cpp)
target_link_libraries(test1 vsomeip3 ${Boost_LIBRARIES})

add_executable(test2 ../src/test2.cpp)
target_link_libraries(test2 vsomeip3 ${Boost_LIBRARIES})

add_executable(client2 ../src/client2.cpp)
target_link_libraries(client2 vsomeip3 ${Boost_LIBRARIES})

add_executable(someip_server src/someip_server.cpp)
target_link_libraries(someip_server vsomeip3 ${Boost_LIBRARIES})

add_executable(test3 ../src/test3.cpp)
target_link_libraries(test3 vsomeip3 ${Boost_LIBRARIES})

add_executable(test4 ../src/test4.cpp)
target_link_libraries(test4 vsomeip3 ${Boost_LIBRARIES})

add_executable(test5 ../src/test5.cpp)
target_link_libraries(test5 vsomeip3 ${Boost_LIBRARIES})

add_executable(test6 ../src/test6.cpp)
target_link_libraries(test6 vsomeip3 ${Boost_LIBRARIES})
