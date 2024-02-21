# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1)


if (NOT DEFINED ENV{JAR_FILE_PATH})
    message(FATAL_ERROR "Path for codegen.jar not set via environment variable JAR_FILE_PATH")
else()
    message(STATUS "Used codegen (JAR_FILE_PATH) = $ENV{JAR_FILE_PATH}")
endif()

# Project's name
project(oven
        LANGUAGES CXX 
)

#set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Set path to the JAR file
# e.g. export JAR_FILE_PATH=/home/myuser/codegen.jar
set(JAR_FILE_PATH $ENV{JAR_FILE_PATH})

# Set path to the JAR file
#set(JAR_FILE_PATH /Users/pmueller/develop/sinelabore/codegen/codegen/codegen.jar)

# Add custom command to generate source files
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/oven.cpp
    COMMAND java -jar ${JAR_FILE_PATH} -l cppx -p ssc -U ${CMAKE_SOURCE_DIR}/codegen.cfg -o oven ${CMAKE_SOURCE_DIR}/oven.xml
    DEPENDS ${CMAKE_SOURCE_DIR}/oven.xml
    DEPENDS ${JAR_FILE_PATH}
)

find_package(Threads REQUIRED)

# Add header files directory
include_directories(${CMAKE_SOURCE_DIR}/inc ${CMAKE_CURRENT_BINARY_DIR})

# Add generated source files to the project
add_executable(oven oven.cpp main.cpp)

TARGET_LINK_LIBRARIES( oven Threads::Threads)