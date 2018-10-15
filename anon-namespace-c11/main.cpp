#include "controllers/Peripherals.h"
#include "controllers/Logger.h"

#include <iostream>

int main() {
    std::cout << "main/Peripherals:" << std::endl;
    auto c = controllers::Peripherals("fake.cfg");
    c.Status();

    std::cout << "main/Logger:" << std::endl;
    auto l = controllers::Logger();
    l.Print();
    return 0;
}