void AdjustWorkload(int& stride) {
    // 逻辑：如果 GPU 占用率 < 95% 且温度 < 80度，则 stride += 32
    // 这样 5090 会从 128 快速爬升至 2048+ 的最优步进
}