/**
 * serializer.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/serializer.h"
#include "ieee1905_1/tlv.h"

#include <algorithm>

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
    const auto& tlvs = cmdu.GetTlvs();
    // Every CMDU must end with an END_OF_MESSAGE TLV; add one if the caller did not.
    const bool append_end_of_message = 
        tlvs.empty() || tlvs.back().GetType() != TlvTypes::END_OF_MESSAGE;

    size_t total_size = Cmdu::CMDU_HEADER_SIZE;
    
    for (const auto& tlv : tlvs) {
        total_size += tlv.GetPacketSize();
    }

    if (append_end_of_message) {
        total_size += Tlv::TLV_HEADER_SIZE;
    }
    
    std::vector<uint8_t> buffer(total_size);
    uint8_t* ptr = buffer.data();

    // 2. Write CMDU Header (8 bytes: Version, Reserved, Type, ID, Fragment ID, Flags)
    WriteUint8(ptr, cmdu.GetVersion());
    WriteUint8(ptr, 0); // Reserved byte
    WriteUint16(ptr, cmdu.GetType());
    WriteUint16(ptr, cmdu.GetId());
    WriteUint8(ptr, cmdu.GetFragmentId());
    WriteUint8(ptr, cmdu.GetFlags());

    // 3. Write each TLV (Type 1 byte, Length 2 bytes, Value)
    for (const auto& tlv : tlvs) {
        WriteUint8(ptr, tlv.GetType());
        WriteUint16(ptr, tlv.GetLength());
        const auto& value_data = tlv.GetValue();
        std::copy(value_data.begin(), value_data.end(), ptr);
        ptr += value_data.size();
    }

    if (append_end_of_message) {
        WriteUint8(ptr, TlvTypes::END_OF_MESSAGE);
        WriteUint16(ptr, 0);
    }

    return buffer;
}

} // namespace ieee1905_1