cmake_minimum_required(VERSION 3.10)
project(Hive LANGUAGES CXX)

# Require C++20 or newer
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define include directory
set(HIVE_INCLUDE_DIR "include")

# Set install destination for headers
install(DIRECTORY ${HIVE_INCLUDE_DIR}/ DESTINATION include)

# Create an interface library
add_library(HIVE INTERFACE)

# Use a relative include path
target_include_directories(HIVE INTERFACE "$<INSTALL_INTERFACE:include>")

# Install the library
install(TARGETS HIVE EXPORT HiveConfig)

# Install CMake config files for find_package
install(EXPORT HiveConfig DESTINATION share/Hive)
