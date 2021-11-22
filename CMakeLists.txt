cmake_minimum_required(VERSION 3.21.4)
project(Entra VERSION 0.1.0)

set(CMAKE_BUILD_TYPE Release)
set(BUILD_TESTS OFF)
set(BUILD_RUN OFF)

add_library(Entra src/Registry.cpp src/Component.cpp)
target_include_directories(Entra PUBLIC ./include)

if (BUILD_RUN)
    add_executable(EntraRun Entra.cpp)
    target_link_libraries(EntraRun PRIVATE Entra)
endif(BUILD_RUN)

if (BUILD_TESTS)
    include(CTest)
    enable_testing()

    set(CPACK_PROJECT_NAME ${PROJECT_NAME})
    set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
    include(CPack)

    add_subdirectory(./googletest)
    include_directories(./googletest/googletest/include)
    add_executable(EntraTest test/Entra.cpp test/Registry.cpp test/Entity.cpp test/Component.cpp test/System.cpp)
    target_link_libraries(EntraTest Entra gtest)

    add_test(UnitTests runUnitTests)
endif(BUILD_TESTS)