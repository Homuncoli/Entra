cmake_minimum_required(VERSION 3.0.0)
project(Entra VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(Entra src/Registry.cpp)
target_include_directories(Entra PUBLIC ./include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_executable(EntraRun Entra.cpp)
target_link_libraries(EntraRun PRIVATE Entra)