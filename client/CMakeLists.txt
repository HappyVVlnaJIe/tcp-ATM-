cmake_minimum_required(VERSION 3.0.0)
project(ATMclient VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(ATMclient main.cpp)
add_library(client STATIC client.cpp)

target_link_libraries(ATMclient client)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
