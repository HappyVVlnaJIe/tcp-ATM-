cmake_minimum_required(VERSION 3.0.0)
project(ATMServer VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_executable(ATMServer main.cpp ATM.cpp)

add_library(database STATIC database.cpp)
add_library(server STATIC server.cpp)



target_link_libraries(ATMServer database server)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
