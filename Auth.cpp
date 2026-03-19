#include "security/Auth.h"
#include "skCrypter.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#else
#include <fstream>
#include <cpuid.h>
#include <net/if.h>
#include <ifaddrs.h>
#endif

// ─── Hardware Fingerprint Collection ──────────────
static std::string GetCpuId() {
    int cpuInfo[4] = { 0 };
    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(8) << cpuInfo[0]
        << std::setw(8) << cpuInfo[3];
    return oss.str();
}

static std::string GetMachineGuid() {
}

// ─── Simple XOR Obfuscation ──────────────────────
static std::string XorObfuscate(const std::string& input, uint8_t key) {
    std::string out = input;
    for (size_t i = 0; i < out.size(); i++) {
        out[i] ^= (key + static_cast<uint8_t>(i * 7));
    }
    return out;
}

// ─── Generate Hardware ID ─────────────────────────
std::string GenerateHWID() {
    std::string cpuId   = GetCpuId();
    std::string machGuid = GetMachineGuid();

    // Concatenate all hardware features and compute a simple hash
    std::string combined = cpuId + "|" + machGuid;
    uint32_t hash = 0x811C9DC5u; // FNV-1a initial value
    for (char c : combined) {
        hash ^= static_cast<uint8_t>(c);
        hash *= 0x01000193u;
    }

    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << hash;
    return oss.str();
}

// ─── License Verification Entry ───────────────────
bool CheckLicense() {
    std::string hwid = GenerateHWID();
    std::cout << skCrypter("[Auth] Machine HWID: ").decrypt() << hwid << std::endl;

    // Read license file
    std::string licenseKey;
    char buf[512] = { 0 };
    DWORD bytesRead = 0;
    HANDLE hFile = CreateFileA("license.dat", GENERIC_READ, FILE_SHARE_READ,
                               NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        ReadFile(hFile, buf, sizeof(buf) - 1, &bytesRead, NULL);
        CloseHandle(hFile);
        licenseKey = std::string(buf, bytesRead);
    }

    if (licenseKey.empty()) {
        std::cout << skCrypter("[Auth] license.dat not found!").decrypt() << std::endl;
        return false;
    }

    // De-obfuscate license key and compare with HWID
    std::string decoded = XorObfuscate(licenseKey, 0xAB);
    if (decoded.find(hwid) == std::string::npos) {
        std::cout << skCrypter("[Auth] License mismatch!").decrypt() << std::endl;
        return false;
    }

    // Check license expiry (UNIX timestamp stored in last 10 chars of key)
    if (decoded.size() >= 10) {
        std::string tsStr = decoded.substr(decoded.size() - 10);
        try {
            long long expiry = std::stoll(tsStr);
            auto now = std::chrono::system_clock::now();
            auto epoch = now.time_since_epoch();
            long long nowSec = std::chrono::duration_cast<std::chrono::seconds>(epoch).count();
            if (nowSec > expiry) {
                std::cout << skCrypter("[Auth] License expired!").decrypt() << std::endl;
                return false;
            }
        } catch (...) {
            // Timestamp parse failed, treat as permanent license
        }
    }

    std::cout << skCrypter("[Auth] License OK").decrypt() << std::endl;
    return true;
}
