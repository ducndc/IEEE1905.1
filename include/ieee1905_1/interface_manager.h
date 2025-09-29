// include/ieee1905_1/interface_manager.h
#ifndef IEEE1905_1_INTERFACE_MANAGER_H
#define IEEE1905_1_INTERFACE_MANAGER_H

#include <vector>
#include <cstdint>
#include <string>

namespace ieee1905_1 {

/**
 * @brief Quản lý I/O mạng cấp thấp (Raw Sockets) cho giao thức 1905.1.
 * * Lớp này cần phụ thuộc vào hệ điều hành (Linux, Windows, v.v.).
 */
class InterfaceManager {
public:
    InterfaceManager(const std::string& iface_name);
    ~InterfaceManager();

    /**
     * @brief Gửi gói tin 1905.1 đến địa chỉ Multicast hoặc Unicast.
     * @param buffer Dữ liệu gói tin nhị phân.
     * @param dest_mac Địa chỉ MAC đích (ví dụ: 01:80:C2:00:00:13 cho Multicast).
     * @return true nếu gửi thành công.
     */
    bool SendPacket(const std::vector<uint8_t>& buffer, const std::vector<uint8_t>& dest_mac);

    /**
     * @brief Chờ và nhận gói tin 1905.1.
     * @param buffer Buffer để lưu dữ liệu nhận được.
     * @return Độ dài gói tin đã nhận hoặc -1 nếu lỗi.
     */
    int ReceivePacket(std::vector<uint8_t>& buffer);

private:
    std::string interface_name_;
    int socket_fd_ = -1; // File descriptor của socket
    // ... Cần thêm logic để xử lý địa chỉ MAC, Bind, v.v.
};

} // namespace ieee1905_1

#endif // IEEE1905_1_INTERFACE_MANAGER_H