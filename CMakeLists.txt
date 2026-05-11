cmake_minimum_required(VERSION 3.8)
project(Project_Kursovai)

set(CMAKE_CXX_STANDARD 17)

add_executable(Shelter
        "src/main.cpp"
        "include/shelter/core/Pet.h"
        "src/core/Pet.cpp"
        "include/shelter/core/Dog.h"
        "src/core/Dog.cpp"
        "include/shelter/core/Cat.h"
        "src/core/Cat.cpp"
        "src/core/Bird.cpp"
        "src/utils/DataLoader.cpp"
        "include/shelter/utils/DataLoader.h"
        "src/core/Exotic.cpp"
        "include/shelter/core/Exotic.h"
        )
