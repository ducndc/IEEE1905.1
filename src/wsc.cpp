/**
 * wsc.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/wsc.h"

#include <stdexcept>
#include <algorithm>
#include <cstring>

namespace ieee1905_1 {

// =======================================================
// Packet/Analyze utility (WSC uses 16-bit Type/Length)
// ====================================================================

// Utility function to write data in Network Byte Order (Big-Endian) - Same as Serializer
static void WriteUint16BE(uint8_t*& ptr, uint16_t value) 
{
    *ptr++ = static_cast<uint8_t>((value >> 8) & 0xFF); 
    *ptr++ = static_cast<uint8_t>(value & 0xFF);        
}

// Utility function to read data in Network Byte Order (Big-Endian)
static uint16_t ReadUint16BE(const uint8_t*& ptr) 
{
    uint16_t value = (*ptr++) << 8; 
    value |= (*ptr++);              
    return value;
}

WscMessage::WscMessage(const std::vector<uint8_t>& raw_wsc_data) 
{
    ParseAttributes(raw_wsc_data);
}

void WscMessage::AddAttribute(uint16_t attribute_type, const std::vector<uint8_t>& value_data) 
{
    // WSC Attributes Length is also 16-bit, max 65535.
    if (value_data.size() > 0xFFFF) {
        throw std::length_error("WSC Attribute value length exceeds 16-bit limit.");
    }

    attributes_[attribute_type] = value_data;
}

// ---------------------------------------------
// Packing WSC Attributes -> Buffer
// ---------------------------------------------
std::vector<uint8_t> WscMessage::PackAttributes() const 
{
    size_t total_size = 0;

    // Calculate total size: (Type 2 bytes + Length 2 bytes + Value Length) for each Attribute
    for (const auto& pair : attributes_) {
        total_size += 4 + pair.second.size();
    }

    std::vector<uint8_t> buffer(total_size);
    uint8_t* ptr = buffer.data();

    for (const auto& pair : attributes_) {
        uint16_t attr_type = pair.first;
        const auto& attr_value = pair.second;
        uint16_t attr_length = static_cast<uint16_t>(attr_value.size());

        // 1. Write Attribute Type (2 bytes, Big-Endian)
        WriteUint16BE(ptr, attr_type);
        
        // 2. Write Attribute Length (2 bytes, Big-Endian)
        WriteUint16BE(ptr, attr_length);

        // 3. Write Attribute Value
        std::copy(attr_value.begin(), attr_value.end(), ptr);
        ptr += attr_value.size();
    }
    
    return buffer;
}

void WscMessage::ParseAttributes(const std::vector<uint8_t>& raw_wsc_data) 
{
    attributes_.clear();
    
    const uint8_t* ptr = raw_wsc_data.data();
    const uint8_t* end_ptr = ptr + raw_wsc_data.size();

    while (ptr < end_ptr) {
        if (end_ptr - ptr < 4) { // Minimum 4 bytes required for Type (2) + Length (2)
            throw std::runtime_error("Incomplete WSC Attribute header in WscMessage.");
        }

        uint16_t attr_type = ReadUint16BE(ptr);
        uint16_t attr_length = ReadUint16BE(ptr);

        if (end_ptr - ptr < attr_length) {
            throw std::runtime_error("WSC Attribute value truncated.");
        }

        std::vector<uint8_t> value(ptr, ptr + attr_length);
        ptr += attr_length;

        attributes_[attr_type] = std::move(value);
    }
}

Tlv WscMessage::ToTlv() const 
{
    std::vector<uint8_t> packed_attributes = PackAttributes();
    // Create TLV 1905.1 with WSC_MESSAGING_TLV Type and encapsulated WSC data.
    return Tlv(WscTlvType::WSC_MESSAGING_TLV, std::move(packed_attributes));
}

std::vector<uint8_t> WscMessage::GetAttributeValue(uint16_t attribute_type) const 
{
    auto it = attributes_.find(attribute_type);

    if (it != attributes_.end()) {
        return it->second;
    }
    return {};
}

} // namespace ieee1905_1