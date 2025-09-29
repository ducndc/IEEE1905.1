// include/ieee1905_1/parser.h
#ifndef IEEE1905_1_PARSER_H
#define IEEE1905_1_PARSER_H

#include "cmdu.h"
#include <vector>
#include <stdexcept>

namespace ieee1905_1 {

/**
 * @brief Phân tích cú pháp (Parse) luồng byte thô thành đối tượng Cmdu.
 * * Lớp tiện ích tĩnh, chịu trách nhiệm xử lý Endianness và kiểm tra lỗi độ dài.
 */
class MessageParser {
public:
    /**
     * @brief Phân tích cú pháp buffer byte nhị phân thành đối tượng Cmdu.
     * @param buffer Buffer byte chứa gói tin 1905.1.
     * @return Đối tượng Cmdu đã được phân tích.
     * @throws std::runtime_error nếu định dạng gói tin không hợp lệ.
     */
    static Cmdu Parse(const std::vector<uint8_t>& buffer);

private:
    // Các hàm tiện ích để đọc dữ liệu theo Network Byte Order (Big-Endian)
    static uint16_t ReadUint16(const uint8_t*& ptr);
    static uint8_t ReadUint8(const uint8_t*& ptr);
};

} // namespace ieee1905_1

#endif // IEEE1905_1_PARSER_H