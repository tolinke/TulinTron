void HandleResult(std::string addr, std::string priv) {
    if (IsSuper(addr)) {
        SaveToFile("Super.txt", addr, priv); // S/s ¿‡
    }
    else {
        SaveToFile("Normal.txt", addr, priv); // A/a ¿‡
    }
}