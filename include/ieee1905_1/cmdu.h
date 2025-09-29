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
    const MessageType TOPOLOGY_DISCOVERY               = 0x0000;
    const MessageType TOPOLOGY_NOTIFICATION            = 0x0001;
    const MessageType TOPOLOGY_QUERY                   = 0x0002;
    const MessageType TOPOLOGY_RESPONSE                = 0x0003;
    const MessageType VENDOR_SPECIFIC                  = 0x0004;
    const MessageType LINK_METRIC_QUERY                = 0x0005;
    const MessageType LINK_METRIC_RESPONSE             = 0x0006;
    const MessageType AP_AUTOCONFIGURATION_SEARCH      = 0x0007;
    const MessageType AP_AUTOCONFIGURATION_RESPONSE    = 0x0008;
    const MessageType AP_AUTOCONFIGURATION_WSC         = 0x0009;
    const MessageType AP_AUTOCONFIGURATION_RENEW       = 0x000a;
    const MessageType PUSH_BUTTON_EVENT_NOTIFICATION   = 0x000b;
    const MessageType PUSH_BUTTON_JOIN_NOTIFICATION    = 0x000c;
    const MessageType HIGHER_LAYER_QUERY               = 0x000d;
    const MessageType HIGHER_LAYER_RESPONSE            = 0x000e;
    const MessageType INTERFACE_POWER_CHANGE_REQUEST   = 0x000f;
    const MessageType INTERFACE_POWER_CHANGE_RESPONSE  = 0x0010;
    const MessageType GENERIC_PHY_QUERY                = 0x0011;
    const MessageType GENERIC_PHY_RESPONSE             = 0x0012;
}


} // namespace ieee1905_1

#endif // IEEE1905_1_CMDU_H