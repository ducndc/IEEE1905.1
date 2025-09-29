// include/ieee1905_1/serializer.h
#ifndef IEEE1905_1_SERIALIZER_H
#define IEEE1905_1_SERIALIZER_H

#include "cmdu.h"
#include <vector>

namespace ieee1905_1 {

/**
 * @brief Chuyển đổi đối tượng Cmdu thành luồng byte sẵn sàng gửi qua mạng.
 * * Lớp tiện ích tĩnh, chịu trách nhiệm xử lý Endianness (Thứ tự Byte).
 */
class MessageSerializer {
public:
    /**
     * @brief Đóng gói Cmdu và các TLV thành một buffer byte hoàn chỉnh.
     * @param cmdu Đối tượng Cmdu cần đóng gói.
     * @return std::vector<uint8_t> chứa gói tin nhị phân.
     */
    static std::vector<uint8_t> Pack(const Cmdu& cmdu);

private:
    // Các hàm tiện ích để ghi dữ liệu theo Network Byte Order (Big-Endian)
    static void WriteUint16(uint8_t*& ptr, uint16_t value);
    static void WriteUint8(uint8_t*& ptr, uint8_t value);
};

} // namespace ieee1905_1

#endif // IEEE1905_1_SERIALIZER_H