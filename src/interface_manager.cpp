/**
 * interface_manager.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/interface_manager.h"
#include "ieee1905_1/logger.h"

#include <iostream>

// In Linux environment, socket headers need to be included
// #include <sys/socket.h> 
// #include <net/if.h>
// #include <sys/ioctl.h>
// #include <arpa/inet.h>
// #include <unistd.h> 

namespace ieee1905_1 {

InterfaceManager::InterfaceManager(const std::string& iface_name) 
    : m_interface_name(iface_name) 
{
    // Initialize Raw Socket
    // m_socket_fd = socket(AF_PACKET, SOCK_RAW, htons(0x893a)); // 0x893a là EtherType cho 1905.1
    // if (m_socket_fd < 0) { ... throw error ... }
    // ... bind socket to m_interface_name
    EM_DEBUG("InterfaceManager: Initialized for interface {}", m_interface_name);
}

InterfaceManager::~InterfaceManager() 
{
    // if (m_socket_fd >= 0) { close(m_socket_fd); }
    EM_DEBUG("InterfaceManager: Shutdown complete.");
}

bool InterfaceManager::SendPacket(const std::vector<uint8_t>& buffer, const std::vector<uint8_t>& dest_mac) 
{
    // Logic for sending packets over Raw Socket
    // sendto(m_socket_fd, buffer.data(), buffer.size(), 0, ...)
    EM_DEBUG("InterfaceManager: Sending packet of size {}", buffer.size());

    return true;
}

int InterfaceManager::ReceivePacket(std::vector<uint8_t>& buffer) 
{
    // Logic to receive packets from Raw Socket
    // int len = recv(m_socket_fd, buffer.data(), buffer.size(), 0);
    // return len;
    return 0; // Assume received 0 byte
}

} // namespace ieee1905_1