/**
 * serializer.h: serializer definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef IEEE1905_1_SERIALIZER_H
#define IEEE1905_1_SERIALIZER_H

#include "cmdu.h"

#include <vector>

namespace ieee1905_1 {

/**
 * @brief Converts a Cmdu object into a byte stream ready to be sent over the network.
 * Static utility class, responsible for handling Endianness (Byte Order).
 */
class MessageSerializer {
public:
    /**
     * @brief Packs the Cmdu and TLVs into a complete byte buffer.
     * @param cmdu The Cmdu object to pack.
     * @return std::vector<uint8_t> containing the binary packet.
     */
    static std::vector<uint8_t> Pack(const Cmdu& cmdu);

private:
    // Utility functions to write data in Network Byte Order (Big-Endian)
    static void WriteUint16(uint8_t*& ptr, uint16_t value);
    static void WriteUint8(uint8_t*& ptr, uint8_t value);
};

} // namespace ieee1905_1

#endif // IEEE1905_1_SERIALIZER_H