/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_IPC_WARP_H
#define THUNDER_IPC_WARP_H

#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include "thunder_huge_tlb.h"

namespace Td {
namespace Kernel {

/**
 * @brief TH-71: Kernel-Warp IPC Bypass.
 * Uses memfd_create and shared memory for near-zero latency IPC.
 */
class IPCWarp {
public:
    static IPCWarp* instance() {
        static IPCWarp inst;
        return &inst;
    }

    /**
     * @brief Creates a hardware-locked shared memory segment.
     */
    void* createSegment(const char* name, size_t size) {
#ifdef Q_OS_LINUX
        if (!name || size == 0) return nullptr;
        int fd = memfd_create(name, MFD_CLOEXEC);
        if (fd == -1) return nullptr;

        if (ftruncate(fd, size) == -1) {
            close(fd);
            return nullptr;
        }

        // Map as HugePages if possible via OmniLock
        void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_LOCKED, fd, 0);

        close(fd);
        if (ptr != MAP_FAILED) {
            std::cout << "[TH-71] IPC Segment Created: " << name << " at " << ptr << std::endl;
            return ptr;
        }
#endif
        return nullptr;
    }

private:
    IPCWarp() = default;
};

} // namespace Kernel
} // namespace Td

#endif // THUNDER_IPC_WARP_H
