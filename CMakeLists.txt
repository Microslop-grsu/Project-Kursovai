cmake_minimum_required(VERSION 3.8)
project(Project_Kursovai)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project_Kursovai
        src/main.cpp "src/Pet.h" "src/Pet.cpp" "src/Dog.h" "src/Dog.cpp" "src/Cat.h")
