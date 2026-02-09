__global__ void StrideKernel(Result* results, uint8_t* baseSeed, int strideSize) {
    uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    Point p;
    GPU_InitPoint(&p, baseSeed, tid);

    for (int i = 0; i < strideSize; i++) {
        // 1. 点加 (P = P + G) - 步进逻辑
        secp256k1_add_g(&p);

        // 2. 计算哈希 (Keccak)
        uint8_t hash[32];
        Keccak256_sm89(hash, p.x, p.y);

        // 3. T58 快速过滤 (只检查首位是否匹配 T)
        if (hash[12] == 0x41) { // TRON 地址前缀探测
            if (T58_Probe(hash, results)) {
                // 发现靓号，存入结果缓冲区
                save_result(results, p.privateKey);
            }
        }
    }
}