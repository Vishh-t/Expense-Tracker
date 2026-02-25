cmake_minimum_required(VERSION 3.26)

project(final_boss)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(final_boss
        main.cpp
        auth.cpp
        helpers.cpp
        expense_tracker.cpp
)