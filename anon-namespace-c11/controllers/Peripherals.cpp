#include "controllers/Peripherals.h"

#include <iostream>
#include <chrono>
#include <thread>

namespace{
class ComplexLegacyCode {
    public:

    bool Validate(const std::string& peripherialsConfigurationData) {
        using namespace std::chrono;
        using namespace std::this_thread;
        // simulate config consumption and validation for blocking operations
            (void)peripherialsConfigurationData;
        std::cout << "Simulate latency..."; std::cout.flush();
        sleep_for(seconds(2));
        std::cout << "Done!\n";
        return true;
    }
};
}

using namespace controllers;

Peripherals::Peripherals(const std::string& configData): peripherialsConfigurationData{configData}{
    std::cout << "[Peripherals/Initializing]: ";
    BackendCondition = ComplexLegacyCode().Validate(peripherialsConfigurationData);
    if (not BackendCondition) {
        throw std::runtime_error{"Peripherals::Missing peripherals"};
    }
}

void Peripherals::Status() const {
    std::cout << "[Peripherals/Checks]: ";
    auto status = ComplexLegacyCode().Validate(peripherialsConfigurationData);
    // status validation skipped for the brevity
    (void)status;
    // system notication skipped for the brevity
}

