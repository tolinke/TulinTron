void Dispatcher::Start() {
    cudaStream_t streams[3];
    for (int i = 0; i < 3; i++) cudaStreamCreate(&streams[i]);

    while (running) {
        // 流 0：负责繁重的计算任务
        StrideKernel << <grid, block, 0, streams[0] >> > (d_res, d_seed, currentStride);

        // 流 1：异步拷贝结果回主机
        cudaMemcpyAsync(h_res, d_res, size, cudaMemcpyDeviceToHost, streams[1]);

        // 流 2：监控 GPU 状态并动态调整 stride
        AdjustWorkload(currentStride);

        cudaDeviceSynchronize(); // 仅在必要时同步
    }
}