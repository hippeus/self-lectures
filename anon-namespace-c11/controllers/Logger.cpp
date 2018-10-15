#include "controllers/Logger.h"

#include <iostream>

namespace{
class ComplexLegacyCode {
    public:
    bool Validate(const std::string& cfg) {
        (void)cfg;
        std::cout << "Logging stuff" << std::endl;
        return true;
    }
};
}

using namespace controllers;

void Logger::Print() const{
    std::cout << "[Logger/Print]: ";
    auto ret = ComplexLegacyCode().Validate("fake.cfg");
    (void)ret;
}