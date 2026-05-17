cmake_minimum_required(VERSION 3.8)
project(ShelterCore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Shelter
        main.cpp
        src/core/Pet.cpp
        src/core/Dog.cpp
        src/core/Cat.cpp
        src/core/Bird.cpp
        src/core/Exotic.cpp
        src/core/MedicalRecord.cpp
        src/services/FeedingMonitor.cpp
        src/storage/DataLoader.cpp
        src/utils/Logger.cpp
        src/storage/PetFactory.cpp
        src/storage/PetRepository.cpp
        src/services/ShelterManager.cpp
        src/ui/ConsoleView.cpp
        include/shelter/core/Pet.h
        include/shelter/core/Dog.h
        include/shelter/core/Cat.h
        include/shelter/core/Bird.h
        include/shelter/core/Exotic.h
        include/shelter/core/MedicalRecord.h
        include/shelter/services/FeedingMonitor.h
        include/shelter/storage/DataLoader.h
        include/shelter/utils/Logger.h
        include/shelter/storage/json.hpp
        include/shelter/storage/PetFactory.h
        include/shelter/storage/PetRepository.h
        include/shelter/services/ShelterManager.h
        include/shelter/ui/ConsoleView.h
)

target_include_directories(Shelter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

