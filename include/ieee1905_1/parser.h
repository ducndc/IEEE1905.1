/**
 * parser.h: parser definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef IEEE1905_1_PARSER_H
#define IEEE1905_1_PARSER_H

#include "cmdu.h"
#include <vector>
#include <stdexcept>

namespace ieee1905_1 {

/**
 * @brief Parses the raw byte stream into a Cmdu object.
 * Static utility class, responsible for handling Endianness and checking for length errors.
 */
class MessageParser {
public:
    /**
     * @brief Parses the binary byte buffer into a Cmdu object.
     * @param buffer The byte buffer containing the 1905.1 packet.
     * @return The parsed Cmdu object.
     * @throws std::runtime_error if the packet format is invalid.
     */
    static Cmdu Parse(const std::vector<uint8_t>& buffer);

private:
    // Utility functions to read data in Network Byte Order (Big-Endian)
    static uint16_t ReadUint16(const uint8_t*& ptr);
    static uint8_t ReadUint8(const uint8_t*& ptr);
};

} // namespace ieee1905_1

#endif // IEEE1905_1_PARSER_H