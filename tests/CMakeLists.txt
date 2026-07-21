include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
)

set (INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(StaticVectorTests
    test_static_vector.cpp
)

target_link_libraries(StaticVectorTests PRIVATE
        DespicableHeap
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(StaticVectorTests)