// include/ieee1905_1/lldp.h
#ifndef IEEE1905_1_LLDP_H
#define IEEE1905_1_LLDP_H

#include "tlv.h" // Tận dụng lớp Tlv cơ sở để lưu trữ dữ liệu
#include <vector>
#include <cstdint>
#include <stdexcept>

namespace ieee1905_1 {

/**
 * @brief Định nghĩa các loại TLV bắt buộc và phổ biến cho LLDP (IEEE 802.1AB).
 * * LLDP TLV Type chỉ sử dụng 7 bit.
 */
namespace LldpTlvTypes {
    // Loại TLV bắt buộc
    const uint8_t END_OF_LLDPDU = 0;        // Bắt buộc: Đánh dấu kết thúc LLDP PDU
    const uint8_t CHASSIS_ID    = 1;        // Bắt buộc: Nhận dạng khung (Chassis) của thiết bị gửi
    const uint8_t PORT_ID       = 2;        // Bắt buộc: Nhận dạng cổng của thiết bị gửi
    const uint8_t TTL           = 3;        // Bắt buộc: Thời gian sống (Time-to-Live)
    
    // Loại TLV tùy chọn
    const uint8_t PORT_DESC     = 4;        // Mô tả cổng
    const uint8_t SYSTEM_NAME   = 5;        // Tên hệ thống
    const uint8_t SYSTEM_DESC   = 6;        // Mô tả hệ thống
    const uint8_t CAPABILITIES  = 7;        // Khả năng của hệ thống (Bridge, Router, v.v.)
    const uint8_t MGMT_ADDR     = 8;        // Địa chỉ quản lý (Management Address)

    // Loại TLV riêng của tổ chức (Organizational Specific TLV)
    const uint8_t ORGANIZATION_SPECIFIC = 127; 
}

/**
 * @brief Đại diện cho một gói LLDP PDU (Protocol Data Unit).
 * * Một LLDP PDU là một chuỗi các LLDP TLV, kết thúc bằng END_OF_LLDPDU TLV.
 */
class LldpPdu {
public:
    // Kích thước header TLV của LLDP là 2 byte (Type 7-bit, Length 9-bit)
    static constexpr size_t LLDP_TLV_HEADER_SIZE = 2;

    /**
     * @brief Khởi tạo một LLDP PDU mới, tự động thêm END_OF_LLDPDU.
     */
    LldpPdu();
    
    // Mặc định hóa các toán tử
    LldpPdu(const LldpPdu&) = default;
    LldpPdu& operator=(const LldpPdu&) = default;
    LldpPdu(LldpPdu&&) = default;
    LldpPdu& operator=(LldpPdu&&) = default;
    ~LldpPdu() = default;

    /**
     * @brief Thêm một LLDP TLV vào PDU.
     * @param tlv Đối tượng Tlv (sẽ được di chuyển).
     */
    void AddTlv(Tlv&& tlv);

    /**
     * @brief Lấy tham chiếu hằng đến danh sách các TLV.
     */
    const std::vector<Tlv>& GetTlvs() const { return tlvs_; }

    /**
     * @brief Tính tổng kích thước đóng gói của PDU (không bao gồm Ethernet Header).
     * * Lưu ý: Việc tính toán này phải dùng LLDP_TLV_HEADER_SIZE = 2 byte.
     */
    size_t GetPackedSize() const;

private:
    std::vector<Tlv> tlvs_;
};

} // namespace ieee1905_1

#endif // IEEE1905_1_LLDP_H