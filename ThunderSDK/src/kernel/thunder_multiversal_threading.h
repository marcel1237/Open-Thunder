/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_MULTIVERSAL_THREADING_H
#define THUNDER_MULTIVERSAL_THREADING_H

#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sched.h>
#include <iostream>
#include "app/thundercommon.h"

namespace Td {
namespace Hardware {

/**
 * @brief TH-1020: Multiversal Task Warp.
 * A hardware-pinned thread pool that eliminates context switching overhead.
 */
class MultiversalThreading {
public:
    static MultiversalThreading* instance() {
        static MultiversalThreading inst;
        return &inst;
    }

    /**
     * @brief Pushes a task into the high-priority multiversal queue.
     */
    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_tasks.push(std::move(task));
        }
        m_condition.notify_one();
    }

    size_t workerCount() const { return m_workers.size(); }

private:
    MultiversalThreading() : m_stop(false) {
        int cores = std::thread::hardware_concurrency();
        if (cores == 0) cores = 4;

        std::cout << "[TH-1020] Multiversal Warp: Spawning " << cores << " hardware-pinned workers." << std::endl;

        for (int i = 0; i < cores; ++i) {
            m_workers.emplace_back([this, i] {
                // TH-1021: Pin worker to specific hardware core
                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                CPU_SET(i, &cpuset);
                sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(m_queueMutex);
                        m_condition.wait(lock, [this] { return m_stop || !m_tasks.empty(); });
                        if (m_stop && m_tasks.empty()) return;
                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~MultiversalThreading() {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_stop = true;
        }
        m_condition.notify_all();
        for (std::thread& worker : m_workers) {
            if (worker.joinable()) worker.join();
        }
    }

    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_queueMutex;
    std::condition_variable m_condition;
    std::atomic<bool> m_stop;
};

} // namespace Hardware
} // namespace Td

#endif // THUNDER_MULTIVERSAL_THREADING_H
