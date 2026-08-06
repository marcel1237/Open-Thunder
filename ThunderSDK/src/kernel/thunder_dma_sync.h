/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_DMA_SYNC_H
#define THUNDER_DMA_SYNC_H

#include <iostream>
#include "thunder_dma_optimizer.h"

namespace Td {
namespace Hardware {

/**
 * @brief TH-37: Direct-Silicon DMA-BUF Sync.
 */
inline bool syncDmaBuffer(int fd) {
#ifdef Q_OS_LINUX
    return Td::Kernel::hardwareSyncBuffer(fd, true) && Td::Kernel::hardwareSyncBuffer(fd, false);
#else
    (void)fd;
    return false;
#endif
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_DMA_SYNC_H
