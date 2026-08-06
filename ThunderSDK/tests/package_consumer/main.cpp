#include <thunder/kernel/thunder_hex_utils.h>

int main() {
    uint32_t value = 0;
    return Td::Utils::hexToUint32("FF", value) && value == 255 ? 0 : 1;
}
