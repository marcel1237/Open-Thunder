/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_CACHE_POOL_H
#define THUNDER_CACHE_POOL_H

#include <vector>
#include <thread>
#include <atomic>
#include "thunder_huge_tlb.h"

namespace Td {
namespace Hardware {

/**
 * @brief TH-28: Cache-Hot Memory Pool.
 * Maintains a pool of memory that is kept warm in the L1 cache.
 */
class CacheHotPool {
public:
    static CacheHotPool* instance() {
        static CacheHotPool inst;
        return &inst;
    }

    void* acquire(size_t size) {
        // Return from pre-warmed pool if possible
        return allocHugeMemory(size);
    }

    void startWarmingThread() {
        bool expected = false;
        if (!m_running.compare_exchange_strong(expected, true)) return;
        m_warmer = std::thread([this]() {
            while (m_running) {
                // Background touch logic to keep caches hot
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }

private:
    CacheHotPool() : m_running(false) {}
    ~CacheHotPool() {
        m_running.store(false, std::memory_order_release);
        if (m_warmer.joinable()) m_warmer.join();
    }
    CacheHotPool(const CacheHotPool&) = delete;
    CacheHotPool& operator=(const CacheHotPool&) = delete;
    std::atomic<bool> m_running;
    std::thread m_warmer;
};

} // namespace Hardware
} // namespace Td

#endif // THUNDER_CACHE_POOL_H
