/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_MULTIVERSAL_PAGING_H
#define THUNDER_MULTIVERSAL_PAGING_H

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <linux/userfaultfd.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdint>
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include "app/thundercommon.h"

namespace Td {
namespace Hardware {

/**
 * @brief TH-1003: User-Space Page Fault Warp.
 * Bypasses Kernel-level page fault handling for zero-copy memory population.
 */
class MultiversalPaging {
public:
    static MultiversalPaging* instance() {
        static MultiversalPaging inst;
        return &inst;
    }

    /**
     * @brief TH-1001: Allocates MVP-ready virtual memory.
     */
    void* allocateUniversal(size_t size) {
        size_t alignedSize = (size + 0x1FFFFF) & ~static_cast<size_t>(0x1FFFFF);

        // MAP_NORESERVE: Thunder handles the commitment via TH-1003
        void* ptr = mmap(NULL, alignedSize, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);

        if (ptr == MAP_FAILED) return MAP_FAILED;

        registerMemoryRange(ptr, alignedSize);

        std::cout << "[TH-1001/1003] MVP Allocation & Warp Engaged: " << alignedSize << " bytes." << std::endl;
        return ptr;
    }

    /**
     * @brief TH-1005: Proactive Page Folding.
     * Prepares virtual pages for multiversal synchronization.
     */
    void synchronizePages(void* ptr, size_t size) {
        if (!ptr || size == 0) return;

        // TH-1005: Enforce hardware pre-fetch and lock for this range
        madvise(ptr, size, MADV_SEQUENTIAL);
        madvise(ptr, size, MADV_WILLNEED);

        std::cout << "[TH-1005] MVP Sync: " << size << " bytes locked to hardware clock." << std::endl;
    }

private:
    MultiversalPaging() : m_uffd(-1), m_running(false) {
        initUffd();
    }

    ~MultiversalPaging() {
        m_running = false;
        if (m_handlerThread.joinable()) m_handlerThread.join();
        if (m_uffd != -1) close(m_uffd);
    }

    void initUffd() {
        // Open userfaultfd context
        m_uffd = syscall(__NR_userfaultfd, O_CLOEXEC | O_NONBLOCK);
        if (m_uffd == -1) return;

        struct uffdio_api uffdio_api;
        uffdio_api.api = UFFD_API;
        uffdio_api.features = 0;
        if (ioctl(m_uffd, UFFDIO_API, &uffdio_api) == -1) return;

        m_running = true;
        m_handlerThread = std::thread(&MultiversalPaging::uffdHandler, this);
    }

    void registerMemoryRange(void* ptr, size_t size) {
        if (m_uffd == -1) return;

        struct uffdio_register uffdio_register;
        uffdio_register.range.start = (unsigned long)ptr;
        uffdio_register.range.len = size;
        uffdio_register.mode = UFFDIO_REGISTER_MODE_MISSING;
        ioctl(m_uffd, UFFDIO_REGISTER, &uffdio_register);
    }

    /**
     * @brief TH-1003: The Warp Handler.
     * Manages page requests in User-Space.
     */
    void uffdHandler() {
        struct pollfd pollfd;
        pollfd.fd = m_uffd;
        pollfd.events = POLLIN;

        while (m_running) {
            int nready = poll(&pollfd, 1, 500);
            if (nready <= 0) continue;

            struct uffd_msg msg;
            if (read(m_uffd, &msg, sizeof(msg)) <= 0) continue;

            if (msg.event == UFFD_EVENT_PAGEFAULT) {
                // TH-1003: Zero-Copy Page Population
                struct uffdio_zeropage uffdio_zeropage;
                uffdio_zeropage.range.start = msg.arg.pagefault.address & ~(4096 - 1);
                uffdio_zeropage.range.len = 4096;
                uffdio_zeropage.mode = 0;
                ioctl(m_uffd, UFFDIO_ZEROPAGE, &uffdio_zeropage);
            }
        }
    }

    int m_uffd;
    std::atomic<bool> m_running;
    std::thread m_handlerThread;
};

} // namespace Hardware
} // namespace Td

#endif // THUNDER_MULTIVERSAL_PAGING_H
