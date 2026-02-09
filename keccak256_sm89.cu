// 针对 Ada Lovelace 的 LOP3.LUT 指令
// 逻辑：(a & b) ^ c -> LUT 掩码 0xCA
__device__ __forceinline__ uint64_t LOP3_CHI(uint64_t a, uint64_t b, uint64_t c) {
    uint32_t low_a = (uint32_t)a, low_b = (uint32_t)b, low_c = (uint32_t)c;
    uint32_t high_a = a >> 32, high_b = b >> 32, high_c = c >> 32;
    uint32_t res_l, res_h;

    asm("lop3.b32 %0, %1, %2, %3, 0xCA;" : "=r"(res_l) : "r"(low_a), "r"(low_b), "r"(low_c));
    asm("lop3.b32 %0, %1, %2, %3, 0xCA;" : "=r"(res_h) : "r"(high_a), "r"(high_b), "r"(high_c));

    return ((uint64_t)res_h << 32) | res_l;
}

// 在 Keccak 轮函数中使用 LOP3 替换标准的位运算