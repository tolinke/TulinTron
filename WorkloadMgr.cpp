#include <iostream>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

// NVML forward declarations (runtime dynamic loading, avoids link dependency)
typedef void* nvmlDevice_t;
typedef int nvmlReturn_t;
typedef struct { unsigned int gpu; unsigned int memory; } nvmlUtilization_t;

// Function pointers (lazy binding)
static nvmlReturn_t (*p_nvmlInit)()                                    = nullptr;
static nvmlReturn_t (*p_nvmlDeviceGetHandleByIndex)(unsigned, nvmlDevice_t*) = nullptr;
static nvmlReturn_t (*p_nvmlDeviceGetUtilizationRates)(nvmlDevice_t, nvmlUtilization_t*) = nullptr;
static nvmlReturn_t (*p_nvmlDeviceGetTemperature)(nvmlDevice_t, int, unsigned*) = nullptr;

static bool nvmlLoaded = false;

static void TryLoadNVML() {
    if (nvmlLoaded) return;
#ifdef _WIN32
    HMODULE lib = LoadLibraryA("nvml.dll");
    if (!lib) lib = LoadLibraryA("C:\\Windows\\System32\\nvml.dll");
    if (lib) {
        p_nvmlInit = (decltype(p_nvmlInit))GetProcAddress(lib, "nvmlInit_v2");
        p_nvmlDeviceGetHandleByIndex = (decltype(p_nvmlDeviceGetHandleByIndex))
            GetProcAddress(lib, "nvmlDeviceGetHandleByIndex_v2");
        p_nvmlDeviceGetUtilizationRates = (decltype(p_nvmlDeviceGetUtilizationRates))
            GetProcAddress(lib, "nvmlDeviceGetUtilizationRates");
        p_nvmlDeviceGetTemperature = (decltype(p_nvmlDeviceGetTemperature))
            GetProcAddress(lib, "nvmlDeviceGetTemperature");
        if (p_nvmlInit) p_nvmlInit();
    }
#endif
    nvmlLoaded = true;
}

// ─── Dynamic Workload Adjustment ─────────────────
void AdjustWorkload(int& stride) {
    TryLoadNVML();

    unsigned gpuUtil = 95; // Default (assume full load when NVML unavailable)
    unsigned gpuTemp = 60;

    if (p_nvmlDeviceGetHandleByIndex && p_nvmlDeviceGetUtilizationRates) {
        nvmlDevice_t dev;
        if (p_nvmlDeviceGetHandleByIndex(0, &dev) == 0) {
            nvmlUtilization_t util;
            if (p_nvmlDeviceGetUtilizationRates(dev, &util) == 0) {
                gpuUtil = util.gpu;
            }
            if (p_nvmlDeviceGetTemperature) {
                p_nvmlDeviceGetTemperature(dev, 0, &gpuTemp); // NVML_TEMPERATURE_GPU=0
            }
        }
    }

    // Adjustment strategy
    const int MIN_STRIDE = 64;
    const int MAX_STRIDE = 4096;

    if (gpuTemp >= 85) {
        // Temperature too high, actively throttle down
        stride = std::max(MIN_STRIDE, stride - 64);
    } else if (gpuTemp >= 80) {
        // Temperature high, stop ramping
        // Keep current stride unchanged
    } else if (gpuUtil < 90) {
        // GPU underutilized, increase stride for better saturation
        stride = std::min(MAX_STRIDE, stride + 64);
    } else if (gpuUtil < 95) {
        // Near saturation, ramp up slightly
        stride = std::min(MAX_STRIDE, stride + 32);
    }
    // gpuUtil >= 95 && temp < 80: optimal state, keep unchanged
}
