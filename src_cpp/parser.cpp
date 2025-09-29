// src/parser.cpp
#include "ieee1905_1/parser.h"
#include "ieee1905_1/tlv.h"

namespace ieee1905_1 {

// ---------------------------------------------
// Xử lý Endianness (Đọc Big-Endian)
// ---------------------------------------------

uint16_t MessageParser::ReadUint16(const uint8_t*& ptr) {
    uint16_t value = (*ptr++) << 8; // Byte cao
    value |= (*ptr++);              // Byte thấp
    return value;
}

uint8_t MessageParser::ReadUint8(const uint8_t*& ptr) {
    return *ptr++;
}

// ---------------------------------------------
// Triển khai Parse
// ---------------------------------------------

Cmdu MessageParser::Parse(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < Cmdu::CMDU_HEADER_SIZE) {
        throw std::runtime_error("Buffer size is smaller than the minimum CMDU header size.");
    }

    const uint8_t* ptr = buffer.data();
    const uint8_t* end_ptr = ptr + buffer.size();

    // 1. Đọc CMDU Header
    MessageType type = ReadUint16(ptr);
    MessageId id = ReadUint16(ptr);
    MessageFlags flags = ReadUint8(ptr);
    ReadUint8(ptr); // Bỏ qua Reserved byte

    // Khởi tạo CMDU
    Cmdu cmdu(type, id, flags);
    
    // Xóa TLV_END_OF_MESSAGE mặc định được thêm trong cấu tử
    // để chuẩn bị cho việc thêm TLV từ buffer.
    // Dù Cmdu đã được thiết kế để tự động quản lý END_OF_MESSAGE, 
    // trong parser, ta chỉ cần thêm các TLV được parse.
    // Tuy nhiên, vì Cmdu được thiết kế để quản lý END_OF_MESSAGE, ta sẽ
    // để parser thêm nó vào sau khi parse xong các TLV khác.

    // 2. Đọc các TLV
    while (ptr < end_ptr) {
        if (end_ptr - ptr < Tlv::TLV_HEADER_SIZE) {
            throw std::runtime_error("Incomplete TLV header found at end of buffer.");
        }

        TlvType tlv_type = ReadUint16(ptr);
        TlvLength tlv_length = ReadUint16(ptr);

        // Kiểm tra xem dữ liệu Value có đủ không
        if (end_ptr - ptr < tlv_length) {
            throw std::runtime_error("Buffer ended prematurely while reading TLV value.");
        }

        // Đọc Value
        std::vector<uint8_t> value(ptr, ptr + tlv_length);
        ptr += tlv_length;

        // Thêm TLV vào CMDU (dùng move)
        cmdu.AddTlv(Tlv(tlv_type, std::move(value)));
        
        // Theo chuẩn 1905.1, nếu TLV được đọc là END_OF_MESSAGE, 
        // thì không còn TLV nào sau đó.
        if (tlv_type == TlvTypes::END_OF_MESSAGE) {
            // Kiểm tra xem đã hết buffer chưa
            if (ptr != end_ptr) {
                 throw std::runtime_error("Data exists after END_OF_MESSAGE TLV.");
            }
            break;
        }
    }

    // Kiểm tra xem CMDU có kết thúc bằng END_OF_MESSAGE không (chỉ cần kiểm tra cái cuối cùng)
    if (cmdu.GetTlvs().empty() || cmdu.GetTlvs().back().GetType() != TlvTypes::END_OF_MESSAGE) {
        throw std::runtime_error("CMDU does not terminate with END_OF_MESSAGE TLV.");
    }
    
    return cmdu;
}

} // namespace ieee1905_1