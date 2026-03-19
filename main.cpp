#include "core/Dispatcher.h"
#include "security/Auth.h"
#include <iostream>

int main() {
    // 1. License verification
    if (!CheckLicense()) {
        std::cout << "License Error!" << std::endl;
        return -1;
    }

    // 2. Print config info (protected by skCrypter)
    std::cout << skCrypter("TulinTron Dynamic Stride Engine Starting...").decrypt() << std::endl;

    // 3. Start async dispatch engine
    Dispatcher engine;
    engine.InitGPU(); // Internal initialization, instant start
    engine.Start();   // Enter compute ramp-up loop

    return 0;
}