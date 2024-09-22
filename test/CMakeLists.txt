
include(../cmake/gtest-dep.cmake)
include_directories(.)

include(GoogleTest)

add_executable(ext_any_gtest ext_any_gtest.cpp)

target_link_libraries(ext_any_gtest  GTest::gtest GTest::gtest_main)

# add_compile_options(-W -Wall -Wextra -Wshadow -Wconversion -Werror)


