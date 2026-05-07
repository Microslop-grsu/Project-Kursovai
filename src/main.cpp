#include "Exotic.h"
#include <iostream>

int main() {
    Exotic exotic("cmo", 3, 25.5, false);

    exotic.printInfo();
    std::cout << exotic.makeSound() << std::endl;
    std::cout << exotic.getDiet() << std::endl;
    std::cout << exotic.getCareInstructions() << std::endl;

    return 0;
}