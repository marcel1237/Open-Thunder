#include <array>
#include <cstdint>
#include <iostream>
#include "kernel/thunder_hex_utils.h"
#include "kernel/thunder_simd_accelerator.h"
#include "network/thunder_net_optimizer.h"
#include "network/thunder_url_warp.h"
#include "adblock/adblockmanager.h"

namespace {
int failures = 0;
void check(bool condition, const char* message) {
    if (!condition) { std::cerr << "FAIL: " << message << '\n'; ++failures; }
}
}

int main() {
    check(Td::Utils::nibbleToHex(10) == 'A', "nibble A");
    uint32_t parsed = 0;
    check(Td::Utils::hexToUint32("DeadBEEF", parsed) && parsed == 0xDEADBEEF, "valid hex");
    check(!Td::Utils::hexToUint32("12x", parsed), "invalid hex");

    std::array<uint8_t, 37> bytes{};
    bytes.back() = 0x7F;
    check(Td::Hardware::fastScanByte(bytes.data(), 0x7F, bytes.size()) == bytes.data() + 36, "SIMD tail");
    check(Td::Hardware::fastScanByte(nullptr, 1, 0) == nullptr, "empty scan");

    const std::array<uint64_t, 3> masks{1, 2, 8};
    check(Td::Network::parallelBitmaskFilter(8, masks.data(), masks.size()), "bitmask tail match");
    check(!Td::Network::parallelBitmaskFilter(4, masks.data(), masks.size()), "bitmask no match");

    const std::array<uint8_t, 12> webp{'R','I','F','F',0,0,0,0,'W','E','B','P'};
    check(Td::Network::fastDetectImageType(webp.data(), webp.size()) == Td::Network::ImageType::WEBP, "WEBP");
    check(Td::Network::isHttps("https://x", 9), "HTTPS");
    check(!Td::Network::isHttps("http", 4), "short URL");

    auto& adblock = AdBlockManager::instance();
    adblock.loadRules({"! comment", "||ads.example.com^", "@@||ads.example.com/allowed^", "tracker*.js"});
    check(adblock.ruleCount() == 3, "adblock valid rule count");
    check(adblock.shouldBlock(QUrl("https://cdn.ads.example.com/banner.png")), "domain rule");
    check(!adblock.shouldBlock(QUrl("https://ads.example.com/allowed/file.js")), "exception rule");
    check(adblock.shouldBlock(QUrl("https://example.org/tracker-v2.js")), "wildcard rule");
    check(!adblock.shouldBlock(QUrl("https://example.org/content.js")), "adblock no match");
    adblock.clear();
    return failures == 0 ? 0 : 1;
}
