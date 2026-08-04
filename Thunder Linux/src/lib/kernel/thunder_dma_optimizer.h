/* ============================================================
* Thunder - Qt web browser
* Copyright (C) 2025 Marcel
*
* Zero-Copy DMA (Direct Memory Access) Buffer Optimization
* ============================================================ */
#ifndef THUNDER_DMA_OPTIMIZER_H
#define THUNDER_DMA_OPTIMIZER_H

#include <cstdint>
#include <sys/ioctl.h>
#include "thundercommon.h"

namespace Td {
namespace Kernel {

/**
 * Hexadecimal IOCTL codes for DMA-BUF synchronization.
 * Direct communication with the Linux DMA-BUF subsystem.
 */
#define T_DMA_BUF_BASE       0x62 // 'b'
#define T_DMA_BUF_IOCTL_SYNC _IOW(T_DMA_BUF_BASE, 0, struct dma_buf_sync)

// Hex-coded DMA Sync Flags
#define T_DMA_BUF_SYNC_READ      0x1
#define T_DMA_BUF_SYNC_WRITE     0x2
#define T_DMA_BUF_SYNC_START     (0x0 << 2)
#define T_DMA_BUF_SYNC_END       (0x1 << 2)

struct dma_buf_sync {
    uint64_t flags;
};

/**
 * @brief Optimizes a file descriptor for DMA-BUF sharing.
 * Bypasses CPU cache for graphics-ready buffers.
 */
inline void hardwareSyncBuffer(int fd, bool start) {
    struct dma_buf_sync sync;
    sync.flags = T_DMA_BUF_SYNC_READ | T_DMA_BUF_SYNC_WRITE;
    sync.flags |= start ? T_DMA_BUF_SYNC_START : T_DMA_BUF_SYNC_END;

    // Direct IOCTL to Kernel DMA subsystem
    ioctl(fd, T_DMA_BUF_IOCTL_SYNC, &sync);
}

} // namespace Kernel
} // namespace Td

#endif // THUNDER_DMA_OPTIMIZER_H
