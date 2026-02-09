#pragma once
#include "skCrypter.h"
#include <string>

bool CheckLicense() {
    // 示例：简单的硬件 ID + XOR 校验
    std::string key = skCrypter("YOUR_LICENSE_KEY_HERE").decrypt();
    // 此处添加你的 HWID 校验逻辑
    return true;
}