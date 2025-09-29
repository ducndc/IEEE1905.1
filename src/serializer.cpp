// src/serializer.cpp
#include "ieee1905_1/serializer.h"
#include "ieee1905_1/tlv.h"

namespace ieee1905_1 {

// ---------------------------------------------
// Xử lý Endianness (Ghi Big-Endian)
// ---------------------------------------------
// Ghi Big-Endian là bắt buộc vì đây là Network Byte Order.

void MessageSerializer::WriteUint16(uint8_t*& ptr, uint16_t value) {
    // Byte cao trước (Big-Endian)
    *ptr++ = static_cast<uint8_t>((value >> 8) & 0xFF); 
    *ptr++ = static_cast<uint8_t>(value & 0xFF);        
}

void MessageSerializer::WriteUint8(uint8_t*& ptr, uint8_t value) {
    *ptr++ = value;
}

// ---------------------------------------------
// Triển khai Pack
// ---------------------------------------------

std::vector<uint8_t> MessageSerializer::Pack(const Cmdu& cmdu) {
    // 1. Tính toán tổng kích thước gói tin
    size_t total_size = Cmdu::CMDU_HEADER_SIZE;
    for (const auto& tlv : cmdu.GetTlvs()) {
        total_size += tlv.GetPacketSize();
    }
    
    std::vector<uint8_t> buffer(total_size);
    uint8_t* ptr = buffer.data();

    // 2. Ghi CMDU Header (6 bytes: Type, ID, Flags, Reserved)
    WriteUint16(ptr, cmdu.GetType());
    WriteUint16(ptr, cmdu.GetId());
    WriteUint8(ptr, cmdu.GetFlags());
    WriteUint8(ptr, 0); // Reserved byte

    // 3. Ghi từng TLV
    for (const auto& tlv : cmdu.GetTlvs()) {
        // Ghi Type (2 bytes)
        WriteUint16(ptr, tlv.GetType());
        
        // Ghi Length (2 bytes)
        WriteUint16(ptr, tlv.GetLength());

        // Ghi Value (Length bytes)
        const auto& value_data = tlv.GetValue();
        std::copy(value_data.begin(), value_data.end(), ptr);
        ptr += value_data.size();
    }

    return buffer;
}

} // namespace ieee1905_1