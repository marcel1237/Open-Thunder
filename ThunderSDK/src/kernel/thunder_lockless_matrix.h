/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_LOCKLESS_MATRIX_H
#define THUNDER_LOCKLESS_MATRIX_H

#include <atomic>
#include <cstdint>
#include "thunder_cache_optimizer.h"

namespace Td {
namespace Hardware {

/**
 * @brief TH-24: Lockless-Stream Matrix.
 * @brief TH-79: Atomic-Warp UI-Thread.
 * @brief TH-87: Atomic-Warp Garbage-Collector.
 * @brief TH-95: Atomic-Warp Resource-Loader.
 */
template <typename T, size_t Capacity>
class LocklessStream {
public:
    static_assert(Capacity > 1, "LocklessStream needs at least two slots");
    // Single-producer/single-consumer queue. Concurrent producers or consumers
    // require external serialization.
    LocklessStream() : m_head(0), m_tail(0) {}

    bool push(const T& item) {
        size_t head = m_head.load(std::memory_order_relaxed);
        size_t next_head = (head + 1) % Capacity;
        if (next_head == m_tail.load(std::memory_order_acquire)) return false;
        m_buffer[head] = item;
        m_head.store(next_head, std::memory_order_release);
        return true;
    }

    bool pop(T& item) {
        size_t tail = m_tail.load(std::memory_order_relaxed);
        if (tail == m_head.load(std::memory_order_acquire)) return false;
        item = m_buffer[tail];
        m_tail.store((tail + 1) % Capacity, std::memory_order_release);
        return true;
    }

private:
    T m_buffer[Capacity];
    THUNDER_CACHE_ALIGNED std::atomic<size_t> m_head;
    THUNDER_CACHE_ALIGNED std::atomic<size_t> m_tail;
};

} // namespace Hardware
} // namespace Td

#endif // THUNDER_LOCKLESS_MATRIX_H
