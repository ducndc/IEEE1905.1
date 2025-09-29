// src/lldp.cpp
#include "ieee1905_1/lldp.h"
#include <iostream>

namespace ieee1905_1 {

// ---------------------------------------------
// Triển khai Cấu tử
// ---------------------------------------------

LldpPdu::LldpPdu() {
    // Theo tiêu chuẩn LLDP, PDU phải kết thúc bằng TLV_END_OF_LLDPDU (Type 0).
    // Chúng ta sẽ thêm nó ở cuối khi Pack, hoặc đảm bảo nó luôn là phần tử cuối cùng.
    // Để đơn giản, ta chỉ thêm nó ở cuối khi đóng gói.
}

// ---------------------------------------------
// Triển khai Quản lý TLV
// ---------------------------------------------

void LldpPdu::AddTlv(Tlv&& tlv) {
    // Kiểm tra và ngăn chặn việc thêm TLV_END_OF_LLDPDU sớm
    if (tlv.GetType() == LldpTlvTypes::END_OF_LLDPDU) {
        throw std::invalid_argument("END_OF_LLDPDU must not be added manually. It is managed by LldpPdu.");
    }

    // Kiểm tra giới hạn LLDP TLV Type (7 bit)
    if (tlv.GetType() > LldpTlvTypes::ORGANIZATION_SPECIFIC) {
        throw std::invalid_argument("LLDP TLV Type exceeds 7-bit limit (max 127).");
    }

    // Kiểm tra giới hạn LLDP TLV Length (9 bit)
    if (tlv.GetLength() > 0x1FF) { // 2^9 - 1 = 511 bytes
        throw std::length_error("LLDP TLV Value length exceeds 9-bit limit (max 511 bytes).");
    }

    // Thêm TLV vào danh sách
    tlvs_.push_back(std::move(tlv));
}

// ---------------------------------------------
// Triển khai Tính toán Kích thước
// ---------------------------------------------

size_t LldpPdu::GetPackedSize() const {
    size_t total_size = 0;

    // Tính kích thước cho từng TLV (Header 2 byte + Value Length)
    for (const auto& tlv : tlvs_) {
        // Kiểm tra lại, phòng trường hợp TLV bị thêm với Length quá lớn
        if (tlv.GetLength() > 0x1FF) {
             throw std::runtime_error("Cannot pack: A TLV length exceeds 511 bytes.");
        }
        total_size += LLDP_TLV_HEADER_SIZE + tlv.GetLength();
    }

    // Bắt buộc phải thêm kích thước của TLV_END_OF_LLDPDU
    // END_OF_LLDPDU (Type 0, Length 0) có kích thước là 2 byte header
    total_size += LLDP_TLV_HEADER_SIZE; 
    
    return total_size;
}

// ---------------------------------------------
// Ghi chú về Serializer/Parser LLDP
// ---------------------------------------------
/*
    Lưu ý quan trọng: Để đóng gói (Pack) hoặc phân tích cú pháp (Parse) LldpPdu,
    bạn sẽ cần một MessageSerializer/Parser riêng biệt cho LLDP.

    Hàm đóng gói LLDP sẽ cần thực hiện thao tác bit sau:
    uint16_t packed_header = (tlv.GetType() << 9) | tlv.GetLength();
    Sau đó ghi packed_header này theo Network Byte Order (Big-Endian) vào buffer.
*/

} // namespace ieee1905_1