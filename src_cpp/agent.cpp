// src/agent.cpp
#include "ieee1905_1/agent.h"
#include "ieee1905_1/cmdu.h"
#include <iostream>

namespace ieee1905_1 {

Ieee1905Agent::Ieee1905Agent(const std::string& interface_name) 
    : iface_manager_(interface_name) {
    std::cout << "Ieee1905Agent: Agent initialized on " << interface_name << std::endl;
}

void Ieee1905Agent::Start() {
    std::cout << "Ieee1905Agent: Starting main loop..." << std::endl;
    std::vector<uint8_t> rx_buffer(2048); // Buffer nhận 2KB
    
    // Vòng lặp chính (ví dụ đơn giản)
    // while (true) {
    //     int len = iface_manager_.ReceivePacket(rx_buffer);
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

void Ieee1905Agent::HandleCmdu(const Cmdu& cmdu) {
    std::cout << "Ieee1905Agent: Received CMDU Type: 0x" << std::hex << cmdu.GetType() << " ID: 0x" << cmdu.GetId() << std::endl;

    switch (cmdu.GetType()) {
        case CmduTypes::TOPOLOGY_DISCOVERY:
            ProcessTopologyDiscovery(cmdu);
            break;
        // ... Thêm các case khác
        default:
            std::cout << "Ieee1905Agent: Unknown CMDU type." << std::endl;
            break;
    }
}

void Ieee1905Agent::SendTopologyDiscovery() {
    std::cout << "Ieee1905Agent: Sending Topology Discovery..." << std::endl;
    
    // 1. Tạo CMDU (Type, ID)
    Cmdu cmdu(CmduTypes::TOPOLOGY_DISCOVERY, next_message_id_++);
    
    // 2. Thêm các TLV cần thiết (ví dụ: MAC Address của thiết bị)
    std::vector<uint8_t> mac_address = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    cmdu.AddTlv(Tlv(TlvTypes::DEVICE_INFO, std::move(mac_address))); // Giả định DEVICE_INFO chứa MAC

    // 3. Serializer thành byte buffer
    std::vector<uint8_t> packet = MessageSerializer::Pack(cmdu);
    
    // 4. Gửi qua InterfaceManager (địa chỉ multicast 1905.1 là 01:80:C2:00:00:13)
    std::vector<uint8_t> multicast_mac = {0x01, 0x80, 0xC2, 0x00, 0x00, 0x13};
    iface_manager_.SendPacket(packet, multicast_mac);
}

void Ieee1905Agent::ProcessTopologyDiscovery(const Cmdu& request) {
    std::cout << "Ieee1905Agent: Processing Topology Discovery Request." << std::endl;
    // Logic: Trả lời bằng gói Topology Response chứa thông tin về các giao diện và các lân cận.
}

} // namespace ieee1905_1