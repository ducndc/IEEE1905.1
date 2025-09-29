// cmdu.cpp
#include "ieee1905_1/cmdu.h"
#include <iostream>

namespace ieee1905_1 {

// ---------------------------------------------
// Triển khai Cấu tử
// ---------------------------------------------

Cmdu::Cmdu(MessageType type, MessageId id, MessageFlags flags)
    : type_(type), id_(id), flags_(flags) {
    // Không cần logic phức tạp trong cấu tử này
}

// ---------------------------------------------
// Triển khai Quản lý TLV
// ---------------------------------------------

void Cmdu::AddTlv(Tlv&& tlv) {
    // Theo tiêu chuẩn 1905.1, TLV_END_OF_MESSAGE phải là TLV cuối cùng
    // Chúng ta có thể thêm một kiểm tra ở đây để đảm bảo logic TLV_END_OF_MESSAGE
    // Tuy nhiên, để đơn giản, chúng ta chỉ thêm vào cuối:
    
    // Kiểm tra xem đã có END_OF_MESSAGE chưa (nếu có, cần xóa nó đi trước)
    if (!tlvs_.empty() && tlvs_.back().GetType() == TlvTypes::END_OF_MESSAGE) {
        // Nếu đã có TLV_END_OF_MESSAGE, thì đẩy nó ra để thêm TLV mới vào giữa
        Tlv end_tlv = std::move(tlvs_.back());
        tlvs_.pop_back();
        
        tlvs_.push_back(std::move(tlv));
        tlvs_.push_back(std::move(end_tlv));
    } else {
        // Thêm TLV mới vào cuối
        tlvs_.push_back(std::move(tlv));
    }
}

const Tlv* Cmdu::FindTlv(TlvType type) const {
    // Tìm kiếm trong vector các TLV
    auto it = std::find_if(tlvs_.begin(), tlvs_.end(), 
        [type](const Tlv& tlv) {
            return tlv.GetType() == type;
        });

    if (it != tlvs_.end()) {
        // Trả về con trỏ tới TLV được tìm thấy
        return &(*it);
    }
    
    // Không tìm thấy
    return nullptr;
}

// ---------------------------------------------
// Ghi chú về Header
// ---------------------------------------------
// Thứ tự các trường trong Header 1905.1:
// 1. Message Type (2 bytes)
// 2. Message ID (2 bytes)
// 3. Fragment ID and Flags (1 byte)
// 4. Reserved (1 byte, luôn là 0)
// 5. Reserved (2 bytes, luôn là 0)
// Kích thước tổng cộng là 6 byte + 2 byte Reserved, hoặc 4 byte tùy biến thể. 
// Chúng ta đang dùng: 2+2+1+1 = 6 bytes (và 2 byte Reserved nữa trong thực tế nếu theo đúng chuẩn).
// Hiện tại, giữ CMDU_HEADER_SIZE = 6 bytes cho mục đích đơn giản.

} // namespace ieee1905_1