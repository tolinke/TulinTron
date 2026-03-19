#include "core/Dispatcher.h"
#include <cuda_runtime.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>

// ─── GPU Initialization (Serialized Multi-GPU) ───
void Dispatcher::InitGPU() {
    cudaGetDeviceCount(&gpuCount);
    std::cout << "[Dispatcher] Detected " << gpuCount << " GPU(s)" << std::endl;

    for (int i = 0; i < gpuCount; i++) {
        cudaSetDevice(i);

        cudaDeviceProp prop;
        cudaGetDeviceProperties(&prop, i);
        std::cout << "  GPU " << i << ": " << prop.name
                  << " | SM=" << prop.major << "." << prop.minor
                  << " | VRAM=" << (prop.totalGlobalMem >> 20) << " MB"
                  << " | SMs=" << prop.multiProcessorCount << std::endl;

        // Pre-allocate device memory
        cudaMalloc(&d_res, MAX_RESULTS * sizeof(Result));
        cudaMalloc(&d_seed, SEED_BYTES);
        cudaMemset(d_res, 0, MAX_RESULTS * sizeof(Result));

        // Allocate pinned host memory (accelerate DMA transfers)
        cudaMallocHost(&h_res, MAX_RESULTS * sizeof(Result));

        // Auto-compute grid size based on SM count
        grid = dim3(prop.multiProcessorCount * 4);
        block = dim3(256);
    }

    currentStride = 128; // Initial stride, dynamically ramped by AdjustWorkload
    running = true;
    totalKeys = 0;
    std::cout << "[Dispatcher] GPU init complete, starting stride=" << currentStride << std::endl;
}

// ─── Main Dispatch Loop (Triple Pipeline) ────────
void Dispatcher::Start() {
    cudaStream_t streams[3];
    for (int i = 0; i < 3; i++) cudaStreamCreate(&streams[i]);

    auto t0 = std::chrono::high_resolution_clock::now();
    uint64_t roundCount = 0;

    while (running) {
        // Stream 0: Heavy computation tasks
        StrideKernel <<< grid, block, 0, streams[0] >>> (d_res, d_seed, currentStride);

        // Check kernel launch errors
        cudaError_t err = cudaGetLastError();
        if (err != cudaSuccess) {
            std::cerr << "[Dispatcher] Kernel Error: " << cudaGetErrorString(err) << std::endl;
            break;
        }

        // Stream 1: Async copy results back to host
        cudaMemcpyAsync(h_res, d_res, MAX_RESULTS * sizeof(Result),
                        cudaMemcpyDeviceToHost, streams[1]);

        // Stream 2: Monitor GPU state and dynamically adjust stride
        AdjustWorkload(currentStride);

        cudaStreamSynchronize(streams[1]); // Wait only for copy completion

        // Check result buffer for new matches
        for (int r = 0; r < MAX_RESULTS; r++) {
            if (h_res[r].found) {
                HandleResult(h_res[r].address, h_res[r].privateKey);
                h_res[r].found = false;
            }
        }

        // Throughput statistics
        roundCount++;
        totalKeys += static_cast<uint64_t>(grid.x) * block.x * currentStride;

        if (roundCount % 100 == 0) {
            auto t1 = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double>(t1 - t0).count();
            double mkeys = static_cast<double>(totalKeys) / elapsed / 1e6;
            std::cout << "\r[Speed] " << std::fixed << mkeys << " Mkeys/s | Stride="
                      << currentStride << " | Keys=" << totalKeys << std::flush;
        }
    }

    // Cleanup
    for (int i = 0; i < 3; i++) cudaStreamDestroy(streams[i]);
    cudaFreeHost(h_res);
    cudaFree(d_res);
    cudaFree(d_seed);
    std::cout << std::endl << "[Dispatcher] Engine stopped." << std::endl;
}