#include <cuda_runtime.h>
#include "secp256k1.h" // 包含必要的 ECC 定义

__device__ void GPU_InitPoint(Point* p, uint8_t* seed, uint32_t tid) {
    // 每个线程分配一个唯一的标量：Seed + tid
    Int scalar;
    scalar.SetBytes(seed);
    scalar.Add(tid);

    // 使用 GPU 内置预计算表计算 P = scalar * G
    // 这里的核心是直接调用优化后的 ECC 乘法，不经过主机
    secp256k1_mul_g(p, scalar);
}