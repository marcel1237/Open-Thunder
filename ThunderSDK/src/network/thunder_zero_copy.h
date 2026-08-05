/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_ZERO_COPY_H
#define THUNDER_ZERO_COPY_H

#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

namespace Td {
namespace Network {

/**
 * @brief TH-36: Zero-Copy Kernel Pipes.
 * Uses vmsplice to map user memory into a kernel pipe without copying.
 */
inline ssize_t pushZeroCopy(int pipe_fd, void* data, size_t len) {
#ifdef Q_OS_LINUX
    struct iovec iov;
    iov.iov_base = data;
    iov.iov_len = len;

    // SPLICE_F_MOVE | SPLICE_F_GIFT to "steal" the page for the kernel
    return vmsplice(pipe_fd, &iov, 1, 0x1 | 0x8);
#else
    return -1;
#endif
}

} // namespace Network
} // namespace Td

#endif // THUNDER_ZERO_COPY_H
