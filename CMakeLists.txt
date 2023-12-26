cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

set(CMAKE_C_STANDARD 11)  # Adjust as needed

# Add your source files
file(GLOB_RECURSE SOURCES src/*.c src/*/*.c)

# Add your header files
file(GLOB_RECURSE HEADERS include/*.h include/*/*.h)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wextra -DDEBUG -g3")

# Include directories
include_directories(include)

# Create executable
add_executable(main ${SOURCES} ${HEADERS}
        src/test.c
        include/test.h)
