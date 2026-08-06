/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_IO_MATRIX_H
#define THUNDER_IO_MATRIX_H

#ifdef Q_OS_LINUX
#include <sys/syscall.h>
#include <sys/mman.h>
#include <linux/io_uring.h>
#include <unistd.h>
#include <cstring>
#endif
#include "app/thundercommon.h"

namespace Td {
namespace IO {

/**
 * @brief TH-12: IO-Uring Warp Matrix.
 * Bypasses standard synchronous syscalls using raw Linux io_uring syscalls.
 * Direct silicon communication for storage and sockets.
 */
class THUNDER_EXPORT ThunderIORing {
public:
    static ThunderIORing* instance() {
        static ThunderIORing inst;
        return &inst;
    }

    bool init() {
#ifdef Q_OS_LINUX
        if (m_ringFd >= 0) return true;
        struct io_uring_params p;
        memset(&p, 0, sizeof(p));
        int fd = syscall(SYS_io_uring_setup, 0x100, &p);
        if (fd != -1) m_ringFd = fd;
        return m_ringFd >= 0;
#else
        return false;
#endif
    }

private:
    ThunderIORing() : m_ringFd(-1) {}
    ~ThunderIORing() { if (m_ringFd >= 0) close(m_ringFd); }
    ThunderIORing(const ThunderIORing&) = delete;
    ThunderIORing& operator=(const ThunderIORing&) = delete;
    int m_ringFd;
};

/**
 * @brief TH-13: Silicon-Direct TLS (kTLS).
 */
inline void enableKernelTLS(int fd) {
#ifdef Q_OS_LINUX
    // SOL_TCP=6, TCP_ULP=31
    syscall(SYS_setsockopt, fd, 6, 31, "tls", 3);
#endif
}

} // namespace IO
} // namespace Td

#endif // THUNDER_IO_MATRIX_H
