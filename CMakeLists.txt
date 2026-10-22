cmake_minimum_required(VERSION 3.16)
project(bakery_oop VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(bakery_lib
    src/product.cpp src/bread.cpp src/pastry.cpp src/cake.cpp
    src/order_item.cpp src/cart.cpp src/product_factory.cpp)
target_include_directories(bakery_lib PUBLIC include)
if(MSVC)
    target_compile_options(bakery_lib PUBLIC /utf-8 /W4)
else()
    target_compile_options(bakery_lib PUBLIC -Wall -Wextra -Wpedantic)
endif()
add_executable(bakery src/main.cpp src/console_app.cpp)
target_link_libraries(bakery PRIVATE bakery_lib)

include(CTest)
if(BUILD_TESTING)
    add_executable(bakery_tests tests/bakery_tests.cpp)
    target_link_libraries(bakery_tests PRIVATE bakery_lib)
    add_test(NAME bakery_tests COMMAND bakery_tests)
endif()
