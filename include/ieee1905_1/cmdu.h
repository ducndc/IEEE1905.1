// cmdu.h
#ifndef IEEE1905_1_CMDU_H
#define IEEE1905_1_CMDU_H

#include "tlv.h" // Bao gồm định nghĩa Tlv
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>

namespace ieee1905_1 {

// Định nghĩa các kiểu dữ liệu cho Header
using MessageType = uint16_t;
using MessageId = uint16_t;
using MessageFlags = uint8_t; // Chỉ dùng 1 byte cho Flags/FragmentID

/**
 * @brief Đại diện cho một gói tin Điều khiển CMDU hoàn chỉnh của IEEE 1905.1.
 */
class Cmdu {
public:
    // Kích thước cố định của CMDU Header (không bao gồm Ethernet Header)
    static constexpr size_t CMDU_HEADER_SIZE = 
        sizeof(MessageType) + 
        sizeof(MessageId) + 
        sizeof(MessageFlags) + 
        1; // Byte dành riêng (Reserved)

    /**
     * @brief Cấu tử tạo một CMDU mới.
     * @param type Loại thông điệp (ví dụ: DISCOVERY, TOPOLOGY_QUERY).
     * @param id ID giao dịch duy nhất.
     * @param flags Cờ thông điệp (ví dụ: bit FragmentID).
     */
    Cmdu(MessageType type, MessageId id, MessageFlags flags = 0);

    // Mặc định hóa các toán tử
    Cmdu(const Cmdu&) = default;
    Cmdu& operator=(const Cmdu&) = default;
    Cmdu(Cmdu&&) = default;
    Cmdu& operator=(Cmdu&&) = default;
    ~Cmdu() = default;

    // --- Các hàm truy cập Header ---
    MessageType GetType() const { return type_; }
    MessageId GetId() const { return id_; }
    MessageFlags GetFlags() const { return flags_; }
    
    // --- Các hàm quản lý TLV ---

    /**
     * @brief Thêm một TLV vào cuối danh sách.
     * @param tlv Đối tượng Tlv (sẽ được di chuyển).
     */
    void AddTlv(Tlv&& tlv);

    /**
     * @brief Lấy tham chiếu hằng đến danh sách các TLV.
     */
    const std::vector<Tlv>& GetTlvs() const { return tlvs_; }

    /**
     * @brief Tìm kiếm và trả về con trỏ (pointer) tới TLV đầu tiên có Type khớp.
     * @param type Loại TLV cần tìm.
     * @return Con trỏ tới Tlv hoặc nullptr nếu không tìm thấy.
     */
    const Tlv* FindTlv(TlvType type) const;

private:
    MessageType type_;
    MessageId id_;
    MessageFlags flags_;
    uint8_t reserved_ = 0; // Luôn là 0 theo tiêu chuẩn
    
    std::vector<Tlv> tlvs_;
};

// Định nghĩa các hằng số Message Type (chỉ là ví dụ)
namespace CmduTypes {
    const MessageType LINK_METRIC_REQUEST = 0x8001;
    const MessageType TOPOLOGY_DISCOVERY = 0x0001;
    // ...
}

} // namespace ieee1905_1

#endif // IEEE1905_1_CMDU_H