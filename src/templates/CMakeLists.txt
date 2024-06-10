cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PaaghVest VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the executable
add_executable( PaaghVest
                main.c
                pv_app.c
)

# Include directories
target_include_directories(PaaghVest PUBLIC "${PROJECT_SOURCE_DIR}")

# Optionally, you can add additional compiler options or definitions
# target_compile_options(MyProject PRIVATE -Wall -Wextra -pedantic)
# target_compile_definitions(MyProject PRIVATE SOME_DEFINITION)
