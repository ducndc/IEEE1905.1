/**
 * interface_manager.h: interface_manager definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef IEEE1905_1_INTERFACE_MANAGER_H
#define IEEE1905_1_INTERFACE_MANAGER_H

#include <vector>
#include <cstdint>
#include <string>

namespace ieee1905_1 {

/**
 * @brief Low-level network I/O management (Raw Sockets) for the 1905.1 protocol.
 * This class needs to be OS dependent (Linux, Windows, etc.).
 */
class InterfaceManager {
public:
    InterfaceManager(const std::string& iface_name);
    ~InterfaceManager();

    /**
     * @brief Send packet 1905.1 to Multicast or Unicast address.
     * @param buffer Binary packet data.
     * @param dest_mac Destination MAC address (e.g. 01:80:C2:00:00:13 for Multicast).
     * @return true if sending is successful.
     */
    bool SendPacket(const std::vector<uint8_t>& buffer, const std::vector<uint8_t>& dest_mac);

    /**
     * @brief Wait for and receive 1905.1 packet.
     * @param buffer Buffer to store received data.
     * @return Length of received packet or -1 if error.
     */
    int ReceivePacket(std::vector<uint8_t>& buffer);

private:
    std::string m_interface_name;
    int m_socket_fd = -1; // Socket file descriptor
    // ... Need more logic to handle MAC addresses, Bind, etc.
};

} // namespace ieee1905_1

#endif // IEEE1905_1_INTERFACE_MANAGER_H