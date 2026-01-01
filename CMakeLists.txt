cmake_minimum_required(VERSION 3.10)
project(LiteVCS VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/cli.cpp
    src/repository.cpp
)

# Create executable
add_executable(vcs ${SOURCES})

# Link libraries
target_link_libraries(vcs 
    PRIVATE 
        OpenSSL::SSL 
        OpenSSL::Crypto 
        ZLIB::ZLIB
)

# Include directories
target_include_directories(vcs PRIVATE src)

# Compiler warnings
if(MSVC)
    target_compile_options(vcs PRIVATE /W4)
else()
    target_compile_options(vcs PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation
install(TARGETS vcs DESTINATION bin)

# Print build information
message(STATUS "LiteVCS Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  OpenSSL Version: ${OPENSSL_VERSION}")
message(STATUS "  ZLIB Version: ${ZLIB_VERSION_STRING}")
