/**
 * parser.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/parser.h"
#include "ieee1905_1/tlv.h"

namespace ieee1905_1 {

// ---------------------------------------------
// Handle Endianness (Read Big-Endian)
// ---------------------------------------------

uint16_t MessageParser::ReadUint16(const uint8_t*& ptr) 
{
    uint16_t value = (*ptr++) << 8; // Byte high
    value |= (*ptr++);              // Byte low
    return value;
}

uint8_t MessageParser::ReadUint8(const uint8_t*& ptr) 
{
    return *ptr++;
}

Cmdu MessageParser::Parse(const std::vector<uint8_t>& buffer) 
{
    if (buffer.size() < Cmdu::CMDU_HEADER_SIZE) {
        throw std::runtime_error("Buffer size is smaller than the minimum CMDU header size.");
    }

    const uint8_t* ptr = buffer.data();
    const uint8_t* end_ptr = ptr + buffer.size();

    // 1. Read CMDU Header
    MessageType type = ReadUint16(ptr);
    MessageId id = ReadUint16(ptr);
    MessageFlags flags = ReadUint8(ptr);
    ReadUint8(ptr); // Ignore Reserved bytes

    // Initialize CMDU
    Cmdu cmdu(type, id, flags);
    
    // Remove the default TLV_END_OF_MESSAGE added in the constructor
    // to prepare for adding TLVs from the buffer.
    // Although Cmdu is designed to automatically manage END_OF_MESSAGE,
    // in the parser, we only need to add parsed TLVs.
    // However, since Cmdu is designed to manage END_OF_MESSAGE, we will
    // let the parser add it after parsing other TLVs.

    // 2. Read the TLVs
    while (ptr < end_ptr) {
        if (end_ptr - ptr < Tlv::TLV_HEADER_SIZE) {
            throw std::runtime_error("Incomplete TLV header found at end of buffer.");
        }

        TlvType tlv_type = ReadUint16(ptr);
        TlvLength tlv_length = ReadUint16(ptr);

        // Check if Value data is sufficient
        if (end_ptr - ptr < tlv_length) {
            throw std::runtime_error("Buffer ended prematurely while reading TLV value.");
        }

        // Read Value
        std::vector<uint8_t> value(ptr, ptr + tlv_length);
        ptr += tlv_length;

        // Add TLV to CMDU (use move)
        cmdu.AddTlv(Tlv(tlv_type, std::move(value)));
        
        // According to the 1905.1 standard, if the TLV is read as END_OF_MESSAGE,
        // then there are no more TLVs after that.
        if (tlv_type == TlvTypes::END_OF_MESSAGE) {
            // Check if buffer is empty
            if (ptr != end_ptr) {
                 throw std::runtime_error("Data exists after END_OF_MESSAGE TLV.");
            }
            break;
        }
    }

    // Check if CMDU ends with END_OF_MESSAGE (just check the last one)
    if (cmdu.GetTlvs().empty() || cmdu.GetTlvs().back().GetType() != TlvTypes::END_OF_MESSAGE) {
        throw std::runtime_error("CMDU does not terminate with END_OF_MESSAGE TLV.");
    }
    
    return cmdu;
}

} // namespace ieee1905_1