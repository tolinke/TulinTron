# 🚀 TulinTron TRON波场靓号 CUDA离线版  (v2.0)

**全球最快的波场 (TRON) 靓号生成器 | 基于 CUDA 指令级优化 | 支持 RTX 40/50 系列**

[![Release](https://img.shields.io/badge/Release-v2.0--Stable-blue.svg)]()
[![CUDA](https://img.shields.io/badge/CUDA-12.0%2B-green.svg)]()
[![Performance](https://img.shields.io/badge/RTX%205090-3.3%20GKey%2Fs-red.svg)]()
[![Security](https://img.shields.io/badge/Security-100%25%20Offline-orange.svg)]()


![Image text](https://github.com/tolinke/TulinTron/blob/main/doc/01.png)

## 📢 2026-02-09 v2.0 重大更新：性能跃迁 20%

通过对内核 **Warp 调度** 的重新建模以及 **LOP3.LUT 指令路径** 的深度修剪，最新版本在原有基础上实现了 **20% 以上的算力飞跃**。这是目前全网唯一针对 NVIDIA **Blackwell (50系)** 架构进行深度适配并压榨极限性能的项目。

- **指令级优化**：深度融合三元逻辑运算指令，大幅减少流水线停顿。
- **算力巅峰**：RTX 5090 实测突破 **3.3 GKey/s (33亿次/秒)**。
- **RTX 50/40 全速适配**：自动识别显卡架构，动态分配最优线程束。
- **工业级产出**：让 8-9 位顶级靓号从“月产”缩短至“小时产”。
- **智能分类**：结果自动按 豹子(A/a)、顺子(S/s) 及 自定义匹配 动态分类保存。

---

## ⚡️ 巅峰算力排行榜 (Benchmarks)

*测试环境：CUDA 12.4, 默认算力负载，单卡表现*

| 显卡型号 | v1.0 算力 | **v2.0 算力 (最新)** | **提升幅度** |
| :--- | :--- | :--- | :--- |
| **NVIDIA RTX 5090** | 2.7 GKey/s | **3.3 GKey/s** | **+22.2%** 🚀 |
| **NVIDIA RTX 4090** | 1.8 GKey/s | **2.2 GKey/s** | **+22.0%** |
| **NVIDIA RTX 4060 Ti** | 500 MKey/s | **660 MKey/s** | **+32.0%** |

---


#### 📊 产出周期详解 (单台 4090 算力参考)
抛开运气成分，平均产出一个靓号的期望时间
⚡️ 任意 6位豹子：约 1  秒
💎 任意 7位豹子：约 15 秒 
💎 指定 7位豹子：约 14 分钟 (如：T...8888888)
👑 任意 8位豹子：约 15 分钟 
👑 指定 8位豹子：约 14 小时 
🏆 任意 9位豹子：约 14 小时 
🏆 指定 9位豹子：约 35 天 

## 环境准备

## 🛠 使用方法
> 1.安装CUDA Toolkit 12.4+ （https://developer.nvidia.com/cuda-12-4-0-download-archive?target_os=Windows&target_arch=x86_64&target_version=10&target_type=exe_local）

![Image text](https://github.com/tolinke/TulinTron/blob/main/doc/02.png)

> 2.下载最新release-x64包（https://github.com/tolinke/TulinTron/releases/tag/v2.0）

![Image text](https://github.com/tolinke/TulinTron/blob/main/doc/03.png)


## 其他操作 

### 1. 查看设备
列出所有可用的 GPU 设备及 ID，默认使用所有GPU，可支持指定GPU：
```bash
./tulinTron --list-devices

# 指定使用第 1 个 GPU (ID为0)
./tulinTron --leopard 8 --device 0

# 使用所有可用 GPU 并行工作 (默认开启)
./tulinTron --leopard 9 --all-gpus

```

### 模式一：靓号模式 (Leopard/Straight)

自动寻找末尾 N 位相同或连续的地址：
```bash
# 生成末尾 8 位相同的豹子号 (例: T...88888888)
./tulinTron --leopard 8

# 生成末尾 8 位连续的顺子号 (例: T...12345678)
./tulinTron --straight 8
```

### 模式二：匹配模式 (Matching)

通过规则文件进行前后缀精准匹配：
```bash
# matching.txt 格式: 前缀*后缀 (如: ABC*XYZ)
./tulinTron --matching matching.txt
```

### 模式三：匹配模式 (Matching)+靓号模式（Leopard）
通过指定靓号位数及规则文件进行前后缀精准匹配：
```bash
# matching.txt 格式: 前缀*后缀 (如: ABC*XYZ)
./tulinTron --matching matching.txt --leopard 8
```


## ✨ 核心优势
- 智能自动分拣：结果自动按 豹子(A/a)、顺子(S/s) 及 自定义匹配 动态分类保存至 Result/ 目录。
- 极致安全保证：100% 离线生成。本程序无需网络权限，私钥生成完全基于本地硬件级 CSPRNG 随机数，绝不上传任何数据。
- 多卡无损并行：支持多显卡协同工作，算力随硬件数量线性无损累加。
- 字符串混淆加密：内置字符串加密保护，有效规避杀毒软件误报。

## ⚠️ 免责声明
- 本工具仅用于技术交流、密码学研究与安全教育，请勿用于任何非法用途。
- 开发者对使用本工具造成的任何资金损失或法律责任不承担任何后果。
- 生成靓号后，请务必妥善保存 result.txt的私钥，丢失无法找回，使用时建议多签。


## 🌟 关注与支持
如果你喜欢这个项目，或者它帮你跑出了心仪的靓号，请点一个 Star！这是我持续优化算力内核的最大动力。
- 项目地址: https://github.com/tolinke/TulinTron
- 软件作者: https://t.me/tolinke @tolinke
- 软件频道: https://t.me/tulinjs @tulinjs

## 打赏地址

TRC-20（TRON链）：
```bash
TGQ32UPMgZNFirNtbS9VDGuXai1Lnn8xXX
```

![Image text](https://github.com/tolinke/TulinTron/blob/main/doc/02.jpg)
