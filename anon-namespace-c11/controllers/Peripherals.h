#pragma once

#include <string>

namespace controllers {
class Peripherals {
public:
    explicit Peripherals(const std::string& configData);
    virtual ~Peripherals() = default;

    virtual void Status() const;

private:
    bool BackendCondition; // true indicates all good.
    const std::string& peripherialsConfigurationData;
};
}
