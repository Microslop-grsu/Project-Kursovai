#include "Bird.h"
#include <iostream>

int main() {
    Bird bird("Kesha", 1, 0.5, 30.0);

    bird.printInfo();
    std::cout << bird.makeSound() << std::endl;
    std::cout << bird.getDiet() << std::endl;
    std::cout << bird.getCareInstructions() << std::endl;

    return 0;
}