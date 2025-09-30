/**
 * agent.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/agent.h"
#include "ieee1905_1/cmdu.h"
#include "ieee1905_1/logger.h"

#include <iostream>

namespace ieee1905_1 {

Ieee1905Agent::Ieee1905Agent(const std::string& interface_name) 
    : m_iface_manager(interface_name) 
{
    EM_DEBUG("Ieee1905Agent: Agent initialized on {}", interface_name);
}

void Ieee1905Agent::Start() 
{
    EM_DEBUG("Ieee1905Agent: Starting main loop...");
    std::vector<uint8_t> rx_buffer(2048); // Buffer nhận 2KB
    
    // Main loop (simple example)
    // while (true) {
    //     int len = m_iface_manager.ReceivePacket(rx_buffer);
    //     if (len > 0) {
    //         try {
    //             std::vector<uint8_t> received_data(rx_buffer.begin(), rx_buffer.begin() + len);
    //             Cmdu cmdu = MessageParser::Parse(received_data);
    //             HandleCmdu(cmdu);
    //         } catch (const std::exception& e) {
    //             std::cerr << "Error parsing CMDU: " << e.what() << std::endl;
    //         }
    //     }
    //     // usleep(1000); // Ngủ 1ms
    // }
}

void Ieee1905Agent::HandleCmdu(const Cmdu& cmdu) 
{
    EM_DEBUG("Ieee1905Agent: Received CMDU Type: 0x{} ID:0x{}", cmdu.GetType(), cmdu.GetId());

    switch (cmdu.GetType()) {
        case CmduTypes::TOPOLOGY_DISCOVERY:
            ProcessTopologyDiscovery(cmdu);
            break;
        // ... Add more cases
        default:
            std::cout << "Ieee1905Agent: Unknown CMDU type." << std::endl;
            break;
    }
}

void Ieee1905Agent::SendTopologyDiscovery() 
{
    EM_DEBUG("Ieee1905Agent: Sending Topology Discovery...");
    
    // 1. Create CMDU (Type, ID)
    Cmdu cmdu(CmduTypes::TOPOLOGY_DISCOVERY, m_next_message_id++);
    
    // 2. Add the necessary TLVs (e.g., MAC Address of the device)
    std::vector<uint8_t> mac_address = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    cmdu.AddTlv(Tlv(TlvTypes::DEVICE_INFORMATION_TYPE, std::move(mac_address))); // Assume DEVICE_INFO contains MAC

    // 3. Serialize to byte buffer
    std::vector<uint8_t> packet = MessageSerializer::Pack(cmdu);
    
    // 4. Send via InterfaceManager (multicast address 1905.1 is 01:80:C2:00:00:13)
    std::vector<uint8_t> multicast_mac = {0x01, 0x80, 0xC2, 0x00, 0x00, 0x13};
    m_iface_manager.SendPacket(packet, multicast_mac);
}

void Ieee1905Agent::ProcessTopologyDiscovery(const Cmdu& request) 
{
    EM_DEBUG("Ieee1905Agent: Processing Topology Discovery Request.");
    // Logic: Respond with a Topology Response packet containing information about interfaces and neighbors.
}

} // namespace ieee1905_1