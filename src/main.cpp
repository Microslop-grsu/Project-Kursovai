#include "Cat.h"
#include <iostream>
using namespace std;

int main() {
    Cat cat("Luna", 2, 4.2, "hairless");

    cat.printInfo();
    std::cout << cat.makeSound() << std::endl;
    std::cout << cat.getDiet() << std::endl;
    std::cout << cat.getCareInstructions() << std::endl;

    return 0;
}