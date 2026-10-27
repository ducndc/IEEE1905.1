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
    ReadUint8(ptr); // Message Version
    ReadUint8(ptr); // Ignore Reserved byte
    MessageType type = ReadUint16(ptr);
    MessageId id = ReadUint16(ptr);
    FragmentId fragment_id = ReadUint8(ptr);
    MessageFlags flags = ReadUint8(ptr);

    // Initialize CMDU
    Cmdu cmdu(type, id, flags, fragment_id);

    // 2. Read the TLVs
    while (ptr < end_ptr) {
        if (end_ptr - ptr < Tlv::TLV_HEADER_SIZE) {
            throw std::runtime_error("Incomplete TLV header found at end of buffer.");
        }

        TlvType tlv_type = ReadUint8(ptr);
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
        // then there are no more TLVs after that. Any remaining bytes are
        // Ethernet padding (minimum frame size) and are ignored.
        if (tlv_type == TlvTypes::END_OF_MESSAGE) {
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