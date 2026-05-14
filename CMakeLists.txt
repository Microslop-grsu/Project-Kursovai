cmake_minimum_required(VERSION 3.8)
project(ShelterCore)

set(CMAKE_CXX_STANDARD 20)

add_executable(Shelter
        "src/main.cpp"
        "include/shelter/core/Pet.h"
        "src/core/Pet.cpp"
        "include/shelter/core/Dog.h"
        "src/core/Dog.cpp"
        "include/shelter/core/Cat.h"
        "src/core/Cat.cpp"
        "src/core/Bird.cpp"
        "include/shelter/core/Bird.h"
        "src/utils/DataLoader.cpp"
        "include/shelter/utils/DataLoader.h"
        "src/core/Exotic.cpp"
        "include/shelter/core/Exotic.h"
        "include/shelter/utils/Logger.h"
        "src/utils/Logger.cpp"
        "include/shelter/utils/json.hpp"
        "include/shelter/core/MedicalRecord.h"
        "src/core/MedicalRecord.cpp"
        include/shelter/core/FeedingMonitor.h
        src/core/FeedingMonitor.cpp
        src/main.cpp
        include/shelter/core/Pet.h
        src/core/Pet.cpp
        include/shelter/core/Dog.h
        src/core/Dog.cpp
        include/shelter/core/Cat.h
        src/core/Cat.cpp
        src/core/Bird.cpp
        include/shelter/core/Bird.h
        src/utils/DataLoader.cpp
        include/shelter/utils/DataLoader.h
        src/core/Exotic.cpp
        include/shelter/core/Exotic.h
        include/shelter/utils/Logger.h
        src/utils/Logger.cpp
        include/shelter/utils/json.hpp
        include/shelter/utils/PetFactory.h
        src/utils/PetFactory.cpp
        include/shelter/utils/PetRepository.h
        src/utils/PetRepository.cpp
)
