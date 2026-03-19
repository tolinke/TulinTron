#pragma once
#include "skCrypter.h"
#include <string>

bool CheckLicense() {
    // Example: simple HWID + XOR verification
    std::string key = skCrypter("YOUR_LICENSE_KEY_HERE").decrypt();
    // Add your HWID verification logic here
    return true;
}