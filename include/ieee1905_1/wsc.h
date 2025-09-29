// include/ieee1905_1/wsc.h
#ifndef IEEE1905_1_WSC_H
#define IEEE1905_1_WSC_H

#include "tlv.h" // Tận dụng lớp Tlv cơ sở
#include <vector>
#include <cstdint>
#include <string>
#include <map>

namespace ieee1905_1 {

// Loại TLV 1905.1 được sử dụng để mang dữ liệu WSC
// (Type này là một ví dụ, trong thực tế cần tra cứu chuẩn 1905.1 cho WSC)
namespace WscTlvType {
    // Thường là Vendor Specific TLV (0x0002) hoặc một Type cụ thể
    // Giả định chúng ta dùng một Type cụ thể trong phạm vi 1905.1
    const TlvType WSC_MESSAGING_TLV = 0x8004; // Ví dụ: WSC TLV
}

/**
 * @brief Định nghĩa các WSC Attributes thường gặp (2 byte Type, 2 byte Length, Value).
 */
namespace WscAttributes {
    const uint16_t WSC_VERSION          = 0x104A; // WSC Version
    const uint16_t WSC_STATE            = 0x104A; // Current state
    const uint16_t WSC_AUTHENTICATION   = 0x1004; // Authentication Type
    const uint16_t WSC_SSID             = 0x1045; // Network SSID
    // ... và nhiều loại Attribute khác
}

/**
 * @brief Đại diện cho dữ liệu WSC được đóng gói trong trường Value của TLV 1905.1.
 * * Lớp này xử lý việc tạo và phân tích cú pháp các WSC Attributes.
 */
class WscMessage {
public:
    /**
     * @brief Cấu tử mặc định.
     */
    WscMessage() = default;

    /**
     * @brief Khởi tạo WscMessage từ dữ liệu thô (sau khi Parse TLV Value).
     * @param raw_wsc_data Dữ liệu WSC thô (vector byte) từ TLV Value.
     */
    explicit WscMessage(const std::vector<uint8_t>& raw_wsc_data);

    /**
     * @brief Thêm một WSC Attribute vào thông điệp.
     * @param attribute_type Loại Attribute (2 byte).
     * @param value_data Dữ liệu thô của Attribute Value.
     */
    void AddAttribute(uint16_t attribute_type, const std::vector<uint8_t>& value_data);

    /**
     * @brief Lấy dữ liệu WSC hoàn chỉnh dưới dạng TLV 1905.1.
     * @return Đối tượng Tlv 1905.1 chứa thông điệp WSC này trong trường Value.
     */
    Tlv ToTlv() const;

    /**
     * @brief Lấy dữ liệu WSC Attribute Value dưới dạng vector byte.
     * @param attribute_type Loại Attribute cần tìm.
     * @return vector byte của Attribute Value, hoặc vector rỗng nếu không tìm thấy.
     */
    std::vector<uint8_t> GetAttributeValue(uint16_t attribute_type) const;

private:
    // Map lưu trữ WSC Attributes: Key=Type, Value=Value Data
    std::map<uint16_t, std::vector<uint8_t>> attributes_;

    // Phương thức nội bộ để đóng gói các Attribute thành buffer byte
    std::vector<uint8_t> PackAttributes() const;
    
    // Phương thức nội bộ để phân tích cú pháp buffer byte thành Attributes
    void ParseAttributes(const std::vector<uint8_t>& raw_wsc_data);
};

} // namespace ieee1905_1

#endif // IEEE1905_1_WSC_H