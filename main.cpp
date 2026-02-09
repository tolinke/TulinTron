#include "core/Dispatcher.h"
#include "security/Auth.h"
#include <iostream>

int main() {
    // 1. 授权校验
    if (!CheckLicense()) {
        std::cout << "License Error!" << std::endl;
        return -1;
    }

    // 2. 打印配置信息 (skCrypter 保护)
    std::cout << skCrypter("TulinTron Dynamic Stride Engine Starting...").decrypt() << std::endl;

    // 3. 启动异步调度引擎
    Dispatcher engine;
    engine.InitGPU(); // 这里会执行内部初始化，秒开
    engine.Start();   // 进入算力爬升循环

    return 0;
}