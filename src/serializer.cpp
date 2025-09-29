/**
 * serializer.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/serializer.h"
#include "ieee1905_1/tlv.h"

namespace ieee1905_1 {

// ---------------------------------------------
// Handle Endianness (Write Big-Endian)
// ---------------------------------------------
// Write Big-Endian is required because this is Network Byte Order.

void MessageSerializer::WriteUint16(uint8_t*& ptr, uint16_t value) 
{
    // High byte first (Big-Endian)
    *ptr++ = static_cast<uint8_t>((value >> 8) & 0xFF); 
    *ptr++ = static_cast<uint8_t>(value & 0xFF);        
}

void MessageSerializer::WriteUint8(uint8_t*& ptr, uint8_t value) 
{
    *ptr++ = value;
}

std::vector<uint8_t> MessageSerializer::Pack(const Cmdu& cmdu) 
{
    size_t total_size = Cmdu::CMDU_HEADER_SIZE;
    
    for (const auto& tlv : cmdu.GetTlvs()) {
        total_size += tlv.GetPacketSize();
    }
    
    std::vector<uint8_t> buffer(total_size);
    uint8_t* ptr = buffer.data();

    // 2. Write CMDU Header (6 bytes: Type, ID, Flags, Reserved)
    WriteUint16(ptr, cmdu.GetType());
    WriteUint16(ptr, cmdu.GetId());
    WriteUint8(ptr, cmdu.GetFlags());
    WriteUint8(ptr, 0); // Reserved byte

    // 3. Write each TLV
    for (const auto& tlv : cmdu.GetTlvs()) {
        WriteUint16(ptr, tlv.GetType());
        WriteUint16(ptr, tlv.GetLength());
        const auto& value_data = tlv.GetValue();
        std::copy(value_data.begin(), value_data.end(), ptr);
        ptr += value_data.size();
    }

    return buffer;
}

} // namespace ieee1905_1