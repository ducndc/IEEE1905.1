// src/wsc.cpp
#include "ieee1905_1/wsc.h"
#include <stdexcept>
#include <algorithm>
#include <cstring> // Cho memcpy

namespace ieee1905_1 {

// =======================================================
// Tiện ích Đóng gói/Phân tích (WSC dùng 16-bit Type/Length)
// =======================================================

// Hàm tiện ích để ghi dữ liệu theo Network Byte Order (Big-Endian) - Giống Serializer
static void WriteUint16BE(uint8_t*& ptr, uint16_t value) {
    *ptr++ = static_cast<uint8_t>((value >> 8) & 0xFF); 
    *ptr++ = static_cast<uint8_t>(value & 0xFF);        
}

// Hàm tiện ích để đọc dữ liệu theo Network Byte Order (Big-Endian)
static uint16_t ReadUint16BE(const uint8_t*& ptr) {
    uint16_t value = (*ptr++) << 8; 
    value |= (*ptr++);              
    return value;
}

// =======================================================
// Triển khai WscMessage
// =======================================================

WscMessage::WscMessage(const std::vector<uint8_t>& raw_wsc_data) {
    ParseAttributes(raw_wsc_data);
}

void WscMessage::AddAttribute(uint16_t attribute_type, const std::vector<uint8_t>& value_data) {
    // WSC Attributes Length cũng là 16-bit, max 65535.
    if (value_data.size() > 0xFFFF) {
        throw std::length_error("WSC Attribute value length exceeds 16-bit limit.");
    }
    attributes_[attribute_type] = value_data;
}

// ---------------------------------------------
// Đóng gói WSC Attributes -> Buffer
// ---------------------------------------------

std::vector<uint8_t> WscMessage::PackAttributes() const {
    size_t total_size = 0;
    // Tính tổng kích thước: (Type 2 byte + Length 2 byte + Value Length) cho mỗi Attribute
    for (const auto& pair : attributes_) {
        total_size += 4 + pair.second.size();
    }

    std::vector<uint8_t> buffer(total_size);
    uint8_t* ptr = buffer.data();

    for (const auto& pair : attributes_) {
        uint16_t attr_type = pair.first;
        const auto& attr_value = pair.second;
        uint16_t attr_length = static_cast<uint16_t>(attr_value.size());

        // 1. Ghi Attribute Type (2 bytes, Big-Endian)
        WriteUint16BE(ptr, attr_type);
        
        // 2. Ghi Attribute Length (2 bytes, Big-Endian)
        WriteUint16BE(ptr, attr_length);

        // 3. Ghi Attribute Value
        std::copy(attr_value.begin(), attr_value.end(), ptr);
        ptr += attr_value.size();
    }
    
    return buffer;
}

// ---------------------------------------------
// Phân tích cú pháp Buffer -> WSC Attributes
// ---------------------------------------------

void WscMessage::ParseAttributes(const std::vector<uint8_t>& raw_wsc_data) {
    attributes_.clear(); // Xóa attributes cũ
    
    const uint8_t* ptr = raw_wsc_data.data();
    const uint8_t* end_ptr = ptr + raw_wsc_data.size();

    while (ptr < end_ptr) {
        if (end_ptr - ptr < 4) { // Cần tối thiểu 4 byte cho Type (2) + Length (2)
            throw std::runtime_error("Incomplete WSC Attribute header in WscMessage.");
        }

        uint16_t attr_type = ReadUint16BE(ptr);
        uint16_t attr_length = ReadUint16BE(ptr);

        if (end_ptr - ptr < attr_length) {
            throw std::runtime_error("WSC Attribute value truncated.");
        }

        // Đọc Value
        std::vector<uint8_t> value(ptr, ptr + attr_length);
        ptr += attr_length;

        attributes_[attr_type] = std::move(value);
    }
}

// ---------------------------------------------
// Chuyển đổi thành TLV 1905.1
// ---------------------------------------------

Tlv WscMessage::ToTlv() const {
    std::vector<uint8_t> packed_attributes = PackAttributes();
    // Tạo TLV 1905.1 với WSC_MESSAGING_TLV Type và dữ liệu WSC đã đóng gói.
    return Tlv(WscTlvType::WSC_MESSAGING_TLV, std::move(packed_attributes));
}

// ---------------------------------------------
// Getters
// ---------------------------------------------

std::vector<uint8_t> WscMessage::GetAttributeValue(uint16_t attribute_type) const {
    auto it = attributes_.find(attribute_type);
    if (it != attributes_.end()) {
        return it->second;
    }
    return {}; // Trả về vector rỗng nếu không tìm thấy
}

} // namespace ieee1905_1