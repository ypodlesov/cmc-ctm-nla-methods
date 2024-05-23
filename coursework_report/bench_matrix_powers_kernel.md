# Benchmark Results

## MatrixPowers Kernel

# Benchmark Results

| Benchmark Name                    | Samples | Iterations | Estimated Run Time | Mean          | Low Mean      | High Mean    | Std Dev       | Low Std Dev   | High Std Dev  |
|-----------------------------------|---------|------------|--------------------|---------------|---------------|--------------|---------------|---------------|---------------|
| MatrixPowersMV with s=1024, size=1024 | 5 | 1 | 1.41871 s             | 286.653 ms    | 274.893 ms    | 291.928 ms   | 8.81498 ms    | 2.91472 ms    | 12.1654 ms    |
| Sequential MatVec's with s=1024, size=1024 | 5 | 1 | 1.43249 s             | 272.036 ms | 266.156 ms | 277.563 ms | 6.86691 ms | 5.50556 ms | 7.31723 ms |
| MatrixPowersMV with s=2048, size=2048 | 5 | 1 | 9.74372 s             | 1.9255 s       | 1.89672 s     | 1.95129 s    | 31.6177 ms    | 16.6668 ms    | 43.4212 ms    |
| Sequential MatVec's with s=2048, size=2048 | 5 | 1 | 9.74436 s             | 1.92448 s    | 1.89521 s    | 1.95178 s   | 32.7367 ms    | 17.7491 ms    | 46.1934 ms    |
| MatrixPowersMV with s=4096, size=4096 | 5 | 1 | 26.4375 s             | 5.37807 s     | 5.31418 s     | 5.43983 s    | 72.5384 ms    | 42.2715 ms    | 99.5027 ms    |
| Sequential MatVec's with s=4096, size=4096 | 5 | 1 | 40.0383 s             | 7.90444 s    | 7.84786 s    | 7.98448 s   | 77.0525 ms    | 32.2863 ms    | 110.963 ms    |
| MatrixPowersMV with s=8192, size=8192 | 5 | 1 | 34.0671 s             | 6.71484 s     | 6.68201 s     | 6.73016 s    | 24.4507 ms    | 7.76424 ms    | 34.2454 ms    |
| Sequential MatVec's with s=8192, size=8192 | 5 | 1 | 1.59192 m             | 19.241 s     | 19.1744 s     | 19.2905 s    | 64.8687 ms    | 37.7646 ms    | 94.7522 ms    |
| MatrixPowersMV with s=16384, size=16384 | 5 | 1 | 2.10464 m             | 23.5549 s     | 23.472 s      | 23.6395 s    | 99.5368 ms    | 79.7288 ms    | 106.88 ms     |
| Sequential MatVec's with s=16384, size=16384 | 5 | 1 | 6.22988 m OOM Killed            | _     | _      | _    | _    | _    | _     |

### Virtualization Features
- Hypervisor vendor: KVM
- Virtualization type: full

### Caches (Sum of All)
- L1d: 256 KiB (8 instances)
- L1i: 256 KiB (8 instances)
- L2: 16 MiB (4 instances)
- L3: 16 MiB (1 instance)

### NUMA
- NUMA node(s): 1
  - NUMA node0 CPU(s): 0-7

### Vulnerabilities
- Gather data sampling: Unknown: Dependent on hypervisor status
- Itlb multihit: Not affected
- L1tf: Not affected
- Mds: Not affected
- Meltdown: Not affected
- Mmio stale data: Vulnerable: Clear CPU buffers attempted, no microcode; SMT Host state unknown
- Retbleed: Not affected
- Spec rstack overflow: Not affected
- Spec store bypass: Mitigation; Speculative Store Bypass disabled via prctl and seccomp
- Spectre v1: Mitigation; usercopy/swapgs barriers and __user pointer sanitization
- Spectre v2: Mitigation; Enhanced IBRS; IBPB conditional; RSB filling; PBRSB-eIBRS SW sequence; BHI Syscall hardening, KVM SW loop
- Srbds: Not affected
- Tsx async abort: Not affected