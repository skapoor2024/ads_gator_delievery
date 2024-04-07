cmake_minimum_required(VERSION 3.10) # Set the minimum version of CMake. Adjust according to your CMake version.

project(GatorDelProject VERSION 1.0 DESCRIPTION "Gator Delivery Service Project" LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use libc++ as the standard library for clang++
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# Add the executable
add_executable(gatorDelivery main_2.cpp gatordel.cpp avltree.cpp node.cpp order.cpp)

# If your project uses any libraries, link them here. For example:
# target_link_libraries(main_2 some_library)
