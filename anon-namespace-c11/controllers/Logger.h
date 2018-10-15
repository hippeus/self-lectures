#pragma once

namespace controllers {
class Logger {
public:
    explicit Logger() = default;
    virtual ~Logger() = default;

    virtual void Print() const;
};
}