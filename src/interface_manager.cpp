/**
 * interface_manager.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/interface_manager.h"
#include <iostream>

// In Linux environment, socket headers need to be included
// #include <sys/socket.h> 
// #include <net/if.h>
// #include <sys/ioctl.h>
// #include <arpa/inet.h>
// #include <unistd.h> 

namespace ieee1905_1 {

InterfaceManager::InterfaceManager(const std::string& iface_name) 
    : interface_name_(iface_name) 
{
    // Initialize Raw Socket
    // socket_fd_ = socket(AF_PACKET, SOCK_RAW, htons(0x893a)); // 0x893a là EtherType cho 1905.1
    // if (socket_fd_ < 0) { ... throw error ... }
    // ... bind socket tới interface_name_
    std::cout << "InterfaceManager: Initialized for interface " << interface_name_ << std::endl;
}

InterfaceManager::~InterfaceManager() 
{
    // if (socket_fd_ >= 0) { close(socket_fd_); }
    std::cout << "InterfaceManager: Shutdown complete." << std::endl;
}

bool InterfaceManager::SendPacket(const std::vector<uint8_t>& buffer, const std::vector<uint8_t>& dest_mac) 
{
    // Logic for sending packets over Raw Socket
    // sendto(socket_fd_, buffer.data(), buffer.size(), 0, ...)
    std::cout << "InterfaceManager: Sending packet of size " << buffer.size() << std::endl;
    return true; // Giả sử thành công
}

int InterfaceManager::ReceivePacket(std::vector<uint8_t>& buffer) 
{
    // Logic to receive packets from Raw Socket
    // int len = recv(socket_fd_, buffer.data(), buffer.size(), 0);
    // return len;
    return 0; // Giả sử nhận được 0 byte
}

} // namespace ieee1905_1