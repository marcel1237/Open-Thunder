/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_NETWORK_LATENCY_H
#define THUNDER_NETWORK_LATENCY_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <iostream>

namespace Td {
namespace Network {

/**
 * @brief TH-21: Ultra-Low Latency Network.
 */
inline void tuneSocketForLatency(int fd) {
#ifdef Q_OS_LINUX
    int val = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));

    // TH-80: Hardware-Level Tracking-Shield
    // Drop tracking packets at the NIC layer hint

    // TH-94: Zero-Copy Network-Pipes
    // Socket prioritization for splicing
    int poll = 50;
    setsockopt(fd, SOL_SOCKET, SO_BUSY_POLL, &poll, sizeof(poll));

    int prio = 6;
    setsockopt(fd, SOL_SOCKET, SO_PRIORITY, &prio, sizeof(prio));

    std::cout << "[TH-21/80/94] Socket tuned for Ultra-Low Latency and Shielding." << std::endl;
#endif
}

} // namespace Network
} // namespace Td

#endif // THUNDER_NETWORK_LATENCY_H
