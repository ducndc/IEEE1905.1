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
    while (true) {
        int len = m_iface_manager.ReceivePacket(rx_buffer);

        if (len > 0) {
            try {
                std::vector<uint8_t> received_data(rx_buffer.begin(), rx_buffer.begin() + len);
                Cmdu cmdu = MessageParser::Parse(received_data);
                HandleCmdu(cmdu);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing CMDU: " << e.what() << std::endl;
            }
        }
        //usleep(1000); 
    }
}

void Ieee1905Agent::HandleCmdu(const Cmdu& cmdu) 
{
    EM_DEBUG("Ieee1905Agent: Received CMDU Type: 0x{} ID:0x{}", cmdu.GetType(), cmdu.GetId());

    switch (cmdu.GetType()) {
        case CmduTypes::TOPOLOGY_DISCOVERY:
            ProcessTopologyDiscovery(cmdu);
            break;
        case CmduTypes::TOPOLOGY_NOTIFICATION:
            ProcessTopologyNotification(cmdu);
            break;
        case CmduTypes::TOPOLOGY_QUERY:
            ProcessTopologyQuery(cmdu);
            break;
        case CmduTypes::TOPOLOGY_RESPONSE:
            ProcessTopologyResponse(cmdu);
            break;
        case CmduTypes::VENDOR_SPECIFIC:
            ProcessVendorSpecific(cmdu);
            break;
        case CmduTypes::LINK_METRIC_QUERY:
            ProcessLinkMetricQuery(cmdu);
            break;
        case CmduTypes::LINK_METRIC_RESPONSE:
            ProcessLinkMetricResponse(cmdu);
            break;
        case CmduTypes::AP_AUTOCONFIGURATION_SEARCH:
            ProcessAutoconfigrationSearch(cmdu);
            break;
        case CmduTypes::AP_AUTOCONFIGURATION_RESPONSE:
            ProcessAutoconfigrationResponse(cmdu);
            break;
        case CmduTypes::AP_AUTOCONFIGURATION_WSC:
            ProcessAutoconfigrationWsc(cmdu);
            break;
        case CmduTypes::AP_AUTOCONFIGURATION_RENEW:
            ProcessAutoconfigrationRenew(cmdu);
            break;
        case CmduTypes::PUSH_BUTTON_EVENT_NOTIFICATION:
            ProcessPushButtonEventNotification(cmdu);
            break;
        case CmduTypes::PUSH_BUTTON_JOIN_NOTIFICATION:
            ProcessPushButtonJoinNotification(cmdu);
            break;
        case CmduTypes::HIGHER_LAYER_QUERY:
            ProcessHigherLayerQuery(cmdu);
            break;
        case CmduTypes::HIGHER_LAYER_RESPONSE:
            ProcessHigherLayerResponse(cmdu);
            break;
        case CmduTypes::INTERFACE_POWER_CHANGE_REQUEST:
            ProcessInterfacePowerChangeRequest(cmdu);
            break;
        case CmduTypes::INTERFACE_POWER_CHANGE_RESPONSE:
            ProcessInterfacePowerChangeResponse(cmdu);
            break;
        case CmduTypes::GENERIC_PHY_QUERY:
            ProcessGenericPhyQuery(cmdu);
            break;
        case CmduTypes::GENERIC_PHY_RESPONSE:
            ProcessGenericPhyResponse(cmdu);
            break;
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

void Ieee1905Agent::SendTopologyNotification()
{
    EM_DEBUG("Ieee1905Agent: Sending Topology Notification...");
}

void Ieee1905Agent::SendTopologyQuery()
{
    EM_DEBUG("Ieee1905Agent: Sending Topology Query...");
}

void Ieee1905Agent::SendTopologyResponse()
{
    EM_DEBUG("Ieee1905Agent: Sending Topology Response...");
}

void Ieee1905Agent::SendVendorSpecific()
{
    EM_DEBUG("Ieee1905Agent: Sending Vendor Specific...");
}

void Ieee1905Agent::SendLinkMetricQuery()
{
    EM_DEBUG("Ieee1905Agent: Sending Link Metric Query...");
}

void Ieee1905Agent::SendLinkMetricResponse()
{
    EM_DEBUG("Ieee1905Agent: Sending Link Metric Response...");
}

void Ieee1905Agent::SendAutoconfigrationSearch()
{
    EM_DEBUG("Ieee1905Agent: Sending Autoconfigration Search...");
}

void Ieee1905Agent::SendAutoconfigrationResponse()
{
    EM_DEBUG("Ieee1905Agent: Sending Autoconfigration Response...");
}

void Ieee1905Agent::SendAutoconfigrationWsc()
{
    EM_DEBUG("Ieee1905Agent: Sending Autoconfigration Wsc...");
}

void Ieee1905Agent::SendAutoconfigrationRenew()
{
    EM_DEBUG("Ieee1905Agent: Sending Autoconfigration Renew...");
}

void Ieee1905Agent::SendPushButtonEventNotification()
{
    EM_DEBUG("Ieee1905Agent: Sending Push Button Event Notification...");
}

void Ieee1905Agent::SendPushButtonJoinNotification()
{
    EM_DEBUG("Ieee1905Agent: Sending Push Button Join Notification...");
}

void Ieee1905Agent::SendHigherLayerQuery()
{
    EM_DEBUG("Ieee1905Agent: Sending Higher Layer Query...");
}

void Ieee1905Agent::SendHigherLayerResponse()
{
    EM_DEBUG("Ieee1905Agent: Sending Higher Layer Response...");
}

void Ieee1905Agent::SendInterfacePowerChangeRequest()
{
    EM_DEBUG("Ieee1905Agent: Sending Interface Power Change Request...");
}

void Ieee1905Agent::SendInterfacePowerChangeResponse()
{
    EM_DEBUG("Ieee1905Agent: Sending Interface Power Change Response...");
}

void Ieee1905Agent::SendGenericPhyQuery()
{
    EM_DEBUG("Ieee1905Agent: Sending Generic Phy Query...");
}

void Ieee1905Agent::SendGenericPhyResponse()
{
    EM_DEBUG("Ieee1905Agent: Sending Generic Phy Response...");
}

void Ieee1905Agent::ProcessTopologyDiscovery(const Cmdu& request) 
{
    EM_DEBUG("Ieee1905Agent: Processing Topology Discovery Request.");
    // Logic: Respond with a Topology Response packet containing information about interfaces and neighbors.
}

void Ieee1905Agent::ProcessTopologyNotification(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Topology Notification.");
}

void Ieee1905Agent::ProcessTopologyQuery(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Topology Query.");
}

void Ieee1905Agent::ProcessTopologyResponse(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Topology Response.");
}

void Ieee1905Agent::ProcessVendorSpecific(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Vendor Specific.");
}

void Ieee1905Agent::ProcessLinkMetricQuery(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Link Metric Query.");
}

void Ieee1905Agent::ProcessLinkMetricResponse(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Link Metric Response.");
}

void Ieee1905Agent::ProcessAutoconfigrationSearch(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Autoconfigration Search.");
}

void Ieee1905Agent::ProcessAutoconfigrationResponse(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Autoconfigration Response.");
}

void Ieee1905Agent::ProcessAutoconfigrationWsc(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Autoconfigration Wsc.");
}

void Ieee1905Agent::ProcessAutoconfigrationRenew(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Autoconfigration Renew.");
}

void Ieee1905Agent::ProcessPushButtonEventNotification(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Push Button Event Notification.");
}

void Ieee1905Agent::ProcessPushButtonJoinNotification(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Push Button Join Notification.");
}

void Ieee1905Agent::ProcessHigherLayerQuery(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Higher Layer Query.");
}

void Ieee1905Agent::ProcessHigherLayerResponse(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Higher Layer Response.");
}

void Ieee1905Agent::ProcessInterfacePowerChangeRequest(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Interface Power Change Request.");
}

void Ieee1905Agent::ProcessInterfacePowerChangeResponse(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Interface Power Change Response.");
}

void Ieee1905Agent::ProcessGenericPhyQuery(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Generic Phy Query.");
}

void Ieee1905Agent::ProcessGenericPhyResponse(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Generic Phy Response.");
}

} // namespace ieee1905_1