#!/bin/bash
# ============================================================
# Thunder SDK - Kernel Authorization Script
# RUN WITH SUDO: sudo ./thunder_unleash_kernel.sh
# ============================================================

echo "[Thunder] Unleashing Kernel-level hardware access..."

# 1. NitroCore: Disable RT Throttling (Allows 100% CPU usage for RT tasks)
echo -1 > /proc/sys/kernel/sched_rt_runtime_us

# 2. OmniLock: Increase Virtual Memory Mapping (For HugePages)
echo 1000000 > /proc/sys/vm/max_map_count

# 3. HexaDrive/NAPI: Networking Busy Poll settings
echo 50 > /proc/sys/net/core/busy_poll
echo 50 > /proc/sys/net/core/busy_read
echo 10000 > /proc/sys/net/core/netdev_max_backlog

# 4. XDP/BPF: Enable High-speed JIT
echo 1 > /proc/sys/net/core/bpf_jit_enable

# 5. L3-Sentinel: Ensure resctrl is mounted (if supported)
mount -t resctrl resctrl /sys/fs/resctrl 2>/dev/null

# 6. Apply Capabilities to Binaries (Granting specific hardware rights)
THUNDER_BUILD_DIR="/home/marcel1237/Thunder/Thunder Linux/build"

# CAP_SYS_NICE: For SCHED_FIFO scheduling
# CAP_IPC_LOCK: For mlockall (RAM locking)
# CAP_NET_ADMIN: For XDP and Network tuning
# CAP_SYS_RAWIO: For direct hardware/PCIe access
setcap 'cap_sys_nice,cap_ipc_lock,cap_net_admin,cap_sys_rawio+ep' "$THUNDER_BUILD_DIR/ThunderBrowser"
setcap 'cap_sys_nice,cap_ipc_lock,cap_net_admin,cap_sys_rawio+ep' "$THUNDER_BUILD_DIR/With_Thunder"
setcap 'cap_sys_nice,cap_ipc_lock,cap_net_admin,cap_sys_rawio+ep' "$THUNDER_BUILD_DIR/ThunderGenericExample"

echo "[Thunder] Hardware Synchronized and Authorized."
