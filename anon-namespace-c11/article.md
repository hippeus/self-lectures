# Anonymous namespaces and what problem they solve

## Context

Imagine that your company released an application called _MyApp_ sometime ago and only very recently your team has shipped a new version (v2.0) to the production.

Apart from new features, version 2 is a major improvement in a terms of code reliability and maintainability. Your co-workers worked really hard to limit new API to its essentials, so it's a way easier to use for all teams around your organization.

Down the road, further code improvements are scheduled. However, the problem with v2 is that it is still hard to test.

Your team already identified that in order to make testing more accessible the whole API will have to change to make dependencies more explicit. This, however, would break API backward compatibility rule, so you already plan ahead to release a new major 3.0 version somewhere in the future.

As bumping versions is not a discrete action but rather a process of few steps while at the time v2 is in the field, the heavy refactoring takes place under the hood. Basically, the rules for refactoring are simple:

- do not change public API for v2.x releases (hpp),

- hide complecity from the user (in cpp),

- mark/group internal dependencies for your components inside source code (they will be candidates for explicit dependencies in v3)

Now fast forward a few weeks as new minor relases for version 2 hits production.

## Problem

With a release of 2.4 of _MyApp_ a weird behavior has been observed. Two separated software testers have raised independent tickets in your company issue tracker. Basically:

> Jenny observed component _A_ started to behave exactly the same way as totally unrelated component _B_.

> Adam reported that in his test - contrary to Jenny's conclusion - it's component _B_ acting like component _A_, and not the other way.

That's weird...

## Code example

Now, once we have enough context and the acctual problem which we can fill into, Let's look at the code.

This is a simplified code of ours theoretical _MyApp_ v2 application.

What it does is just initialize some peripherals, checks their status and then call logger to record the entire system state.

```cpp
#include "controllers/Peripherals.h" // componentA
#include "controllers/Logger.h" // componentB

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
```

In this snipped _Peripherals_ represents a blocking operation (meaning it takes actual CPU time to configure them). Login on the other hand is an inexpensive operation (relatively quick).

This is what Jenny logs look like:

```bash
main/Peripherals:
[Peripherals/Initializing]: Simulate latency...Done!
[Peripherals/Checks]: Simulate latency...Done!
main/Logger:
[Logger/Print]: Simulate latency...Done!
0:06.00 real,	0.00 user,	0.00 sys
```

And this is what Adam recorded:

```bash
main/Peripherals:
[Peripherals/Initializing]: Logging stuff
[Peripherals/Checks]: Logging stuff
main/Logger:
[Logger/Print]: Logging stuff
0:00.00 real,	0.00 user,	0.00 sys
```

So, both Jenny and Adam worked with the same code revision (you already confirmed that). What's more, you also build the same code yourself and confirmed that there is no compilation error or warning (hint: -Wall -Wextra flags).

Yet Jenny code runs for 6 seconds where Adam's binary finished almost instantly...

Wait a minute... Jenny's Logger acts as Peripherals object and for Adam's it is the other way around!

Let's us examine *cpp files!

Peripherals.cpp:

```cpp
#include "controllers/Peripherals.h"

#include <iostream>
#include <chrono>
#include <thread>

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
```

Logger.cpp:

```cpp
#include "controllers/Logger.h"

#include <iostream>

class ComplexLegacyCode {
    public:
    bool Validate(const std::string& cfg) {
        (void)cfg;
        std::cout << "Logging stuff" << std::endl;
        return true;
    }
};

using namespace controllers;

void Logger::Print() const{
    std::cout << "[Logger/Print]: ";
    auto ret = ComplexLegacyCode().Validate("fake.cfg");
    (void)ret;
}
```

- Have you spotted that nasty bug already?
- Can you fix it quickly?
- Can you explain what was wrong?
- Can you explain why Jenny and Adam had different results for the same code?

Here is the output after applying a patch to fix the issue:

```bash
main/Peripherals:
[Peripherals/Initializing]: Simulate latency...Done!
[Peripherals/Checks]: Simulate latency...Done!
main/Logger:
[Logger/Print]: Logging stuff
0:04.00 real,	0.00 user,	0.00 sys
```

## Explanation

The bug affecting the system was relatively small, yet impossible to detect by the compiler. While preparing the legacy code for the version 3 release your teammate extracted "ugly" code to a separate object called _ComplexLegacyCode_ inside _Peripherals.cpp_. Since this object was scoped to a cpp, he was fairly confident that this won't affect other parts of the system.

Ideally, he would like to mark this type declarations as _static_ to limit its usage to a single translation unit. However, since _ComplexLegacyCode_ is declared in-place type in a cpp file, that operation is illegal with _static_ keyword.

In the same fashion, a similar type declaration with the same name (_ComplexLegacyCode_) was introduced inside _Logger.cpp_.

Both those types had a similar class member _bool ComplexLegacyCode::Verify(std::string&)_ with a different implementation but __the same signature__.

Since your mate failed to assure that those classes are scoped to a translation unit **the linker** assumed that both those object are actually the same.

The reason why Jenny and Adam ended up with different results was that __order of linking objects matters__.

In Jenny's build, linker picked implementation from _Peripherals.o_ and used it for _Logger_ call.

For Adam it was different, linker picked _ComplexLegacyCode_ implementation from _Logger.cpp_ and used if for Peripherals.

The fix was easy. Instead of using _static_ keyword for your globally scoped variables and declaration in cpp, use _anonymous namespaces_ instead. That will enforce correctness and encapsulation in a single translation unit.

```cpp
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
```

## Take Aways

> favor _annonymous_ namespaces vs _static_ as better defaults for expressing global scopes.

> If you need binary reproducible builds for your manifest files, add _-frandom-seed=xyz.cpp_ (GCC only). See: [stackoverflow][1]

> Although it didn't help this particular use case, Always build with **-Wall** **-Wextra** flags, to report all warning as errors.

[1]: https://stackoverflow.com/questions/4140329/binary-object-file-changing-in-each-build
