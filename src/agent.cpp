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

const char *CmduTypeToString(uint16_t type)
{
    switch (type) {

    /* ===== IEEE 1905.1 core ===== */
    case 0x0000: return "TOPOLOGY_DISCOVERY";
    case 0x0001: return "TOPOLOGY_NOTIFICATION";
    case 0x0002: return "TOPOLOGY_QUERY";
    case 0x0003: return "TOPOLOGY_RESPONSE";
    case 0x0004: return "VENDOR_SPECIFIC";
    case 0x0005: return "LINK_METRIC_QUERY";
    case 0x0006: return "LINK_METRIC_RESPONSE";
    case 0x0007: return "AP_AUTOCONFIGURATION_SEARCH";
    case 0x0008: return "AP_AUTOCONFIGURATION_RESPONSE";
    case 0x0009: return "AP_AUTOCONFIGURATION_WSC";
    case 0x000a: return "AP_AUTOCONFIGURATION_RENEW";
    case 0x000b: return "PUSH_BUTTON_EVENT_NOTIFICATION";
    case 0x000c: return "PUSH_BUTTON_JOIN_NOTIFICATION";
    case 0x000d: return "HIGHER_LAYER_QUERY";
    case 0x000e: return "HIGHER_LAYER_RESPONSE";
    case 0x000f: return "INTERFACE_POWER_CHANGE_REQUEST";
    case 0x0010: return "INTERFACE_POWER_CHANGE_RESPONSE";
    case 0x0011: return "GENERIC_PHY_QUERY";
    case 0x0012: return "GENERIC_PHY_RESPONSE";

    /* ===== EasyMesh / Multi-AP ===== */
    case 0x8000: return "ACK_1905";
    case 0x8001: return "AP_CAPABILITY_QUERY";
    case 0x8002: return "AP_CAPABILITY_REPORT";
    case 0x8003: return "MAP_POLICY_CONFIG_REQUEST";
    case 0x8004: return "CHANNEL_PREFERENCE_QUERY";
    case 0x8005: return "CHANNEL_PREFERENCE_REPORT";
    case 0x8006: return "CHANNEL_SELECTION_REQUEST";
    case 0x8007: return "CHANNEL_SELECTION_RESPONSE";
    case 0x8008: return "OPERATING_CHANNEL_REPORT";
    case 0x8009: return "CLIENT_CAPABILITY_QUERY";
    case 0x800a: return "CLIENT_CAPABILITY_REPORT";
    case 0x800b: return "AP_LINK_METRICS_QUERY";
    case 0x800c: return "AP_LINK_METRICS_RESPONSE";
    case 0x800d: return "ASSOC_STA_LINK_METRICS_QUERY";
    case 0x800e: return "ASSOC_STA_LINK_METRICS_RESPONSE";
    case 0x800f: return "UNASSOC_STA_LINK_METRICS_QUERY";
    case 0x8010: return "UNASSOC_STA_LINK_METRICS_RESPONSE";
    case 0x8011: return "BEACON_METRICS_QUERY";
    case 0x8012: return "BEACON_METRICS_RESPONSE";
    case 0x8013: return "COMBINED_INFRASTRUCTURE_METRICS";
    case 0x8014: return "CLIENT_STEERING_REQUEST";
    case 0x8015: return "CLIENT_STEERING_BTM_REQUEST";
    case 0x8016: return "CLIENT_ASSOC_CONTROL_REQUEST";
    case 0x8017: return "CLIENT_STEERING_COMPLETE";
    case 0x8018: return "HIGHER_LAYER_DATA_MESSAGE";
    case 0x8019: return "BACKHAUL_STEERING_REQUEST";
    case 0x801a: return "BACKHAUL_STEERING_RESPONSE";
    case 0x801b: return "CHANNEL_SCAN_REQUEST";
    case 0x801c: return "CHANNEL_SCAN_RESPONSE";
    case 0x801d: return "DPP_CCE_INDICATION_MESSAGE";
    case 0x801e: return "_1905_REKEY_REQUEST";
    case 0x801f: return "_1905_DESCRIPTION_FAILURE";
    case 0x8020: return "CAC_REQUEST";
    case 0x8021: return "CAC_TERMINATION";
    case 0x8022: return "CLIENT_DISASSOCIATION_STATS";
    case 0x8023: return "SERVICE_PRIORITIZATION_STATS";
    case 0x8024: return "ERROR_RESPONSE";
    case 0x8025: return "ASSOCIATION_STATUS_NOTIFICATION";
    case 0x8026: return "TUNNELED_MESSAGE";
    case 0x8027: return "BACKHAUL_STA_CAP_QUERY_MESSAGE";
    case 0x8028: return "BACKHAUL_STA_CAP_REPORT_MESSAGE";
    case 0x8029: return "PROXIED_ENCAP_DPP_MESSAGE";
    case 0x802a: return "DIRECT_ENCAP_DPP_MESSAGE";
    case 0x802b: return "BSS_RECONFIGURATION_TRIGGER_MESSAGE";
    case 0x802c: return "BSS_CONFIGURATION_REQUEST_MESSAGE";
    case 0x802d: return "BSS_CONFIGURATION_RESPONSE_MESSAGE";
    case 0x802e: return "BSS_CONFIGURATION_RESULT_MESSAGE";
    case 0x802f: return "CHIRP_NOTIFICATION_MESSAGE";
    case 0x8030: return "_1905_ENCAP_EAPOL";
    case 0x8031: return "DPP_BOOTSTRAPING_URI_NOTIFICATION";
    case 0x8032: return "DPP_BOOTSTRAPING_URI_QUERY";
    case 0x8033: return "FAILED_CONNECTION_MESSAGE";
    case 0x8034: return "DPP_URI_NOTIFICATION_MESSAGE";
    case 0x8035: return "AGENT_LIST_MESSAGE";
    case 0x8037: return "QOS_MANAGEMENT_NOTIFICATION";
    case 0x8043: return "EARLY_AP_CAP_REPORT_MESSAGE";
    case 0x8044: return "AP_MLD_CONFIGURATION_REQUEST_MESSAGE";
    case 0x8045: return "AP_MLD_CONFIGURATION_RESPONSE_MESSAGE";
    case 0x8046: return "BSTA_MLD_CONFIGURATION_REQUEST_MESSAGE";
    case 0x8047: return "BSTA_MLD_CONFIGURATION_RESPONSE_MESSAGE";
    case 0x8048: return "AFC_SPECTRUM_INQUIRY_MESSAGE";

    /* ===== Internal / debug ===== */
    case 0x9000: return "DEV_SEND_1905_REQUEST";

    default:
        return "UNKNOWN_CMDU_TYPE";
    }
}

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
    const uint16_t type = cmdu.GetType();
    const uint16_t mid  = cmdu.GetId();

    EM_DEBUG("Ieee1905Agent: Received CMDU {} (0x{:04x}), MID=0x{:04x}",
             CmduTypeToString(type), type, mid);

    switch (type) {

    /* =========================================================
     * 1905.1 – TOPOLOGY
     * ========================================================= */
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

    /* =========================================================
     * 1905.1 – LINK METRICS
     * ========================================================= */
    case CmduTypes::LINK_METRIC_QUERY:
        ProcessLinkMetricQuery(cmdu);
        break;

    case CmduTypes::LINK_METRIC_RESPONSE:
        ProcessLinkMetricResponse(cmdu);
        break;

    /* =========================================================
     * 1905.1 – AP AUTOCONFIG
     * ========================================================= */
    case CmduTypes::AP_AUTOCONFIGURATION_SEARCH:
        ProcessAutoconfigurationSearch(cmdu);
        break;

    case CmduTypes::AP_AUTOCONFIGURATION_RESPONSE:
        ProcessAutoconfigurationResponse(cmdu);
        break;

    case CmduTypes::AP_AUTOCONFIGURATION_WSC:
        ProcessAutoconfigurationWsc(cmdu);
        break;

    case CmduTypes::AP_AUTOCONFIGURATION_RENEW:
        ProcessAutoconfigurationRenew(cmdu);
        break;

    /* =========================================================
     * 1905.1 – GENERIC / MISC
     * ========================================================= */
    case CmduTypes::VENDOR_SPECIFIC:
        ProcessVendorSpecific(cmdu);
        break;

    case CmduTypes::HIGHER_LAYER_QUERY:
        ProcessHigherLayerQuery(cmdu);
        break;

    case CmduTypes::HIGHER_LAYER_RESPONSE:
        ProcessHigherLayerResponse(cmdu);
        break;

    case CmduTypes::GENERIC_PHY_QUERY:
        ProcessGenericPhyQuery(cmdu);
        break;

    case CmduTypes::GENERIC_PHY_RESPONSE:
        ProcessGenericPhyResponse(cmdu);
        break;

    /* =========================================================
     * EasyMesh – AP / RADIO CAPABILITY
     * ========================================================= */
    case CmduTypes::AP_CAPABILITY_QUERY:
        ProcessApCapabilityQuery(cmdu);
        break;

    case CmduTypes::AP_CAPABILITY_REPORT:
        ProcessApCapabilityReport(cmdu);
        break;

    /* =========================================================
     * EasyMesh – CHANNEL MANAGEMENT
     * ========================================================= */
    case CmduTypes::CHANNEL_PREFERENCE_QUERY:
        ProcessChannelPreferenceQuery(cmdu);
        break;

    case CmduTypes::CHANNEL_PREFERENCE_REPORT:
        ProcessChannelPreferenceReport(cmdu);
        break;

    case CmduTypes::CHANNEL_SELECTION_REQUEST:
        ProcessChannelSelectionRequest(cmdu);
        break;

    case CmduTypes::CHANNEL_SELECTION_RESPONSE:
        ProcessChannelSelectionResponse(cmdu);
        break;

    case CmduTypes::OPERATING_CHANNEL_REPORT:
        ProcessOperatingChannelReport(cmdu);
        break;

    /* =========================================================
     * EasyMesh – CLIENT CAPABILITY & METRICS
     * ========================================================= */
    case CmduTypes::CLIENT_CAPABILITY_QUERY:
        ProcessClientCapabilityQuery(cmdu);
        break;

    case CmduTypes::CLIENT_CAPABILITY_REPORT:
        ProcessClientCapabilityReport(cmdu);
        break;

    case CmduTypes::ASSOC_STA_LINK_METRICS_QUERY:
        ProcessAssocStaLinkMetricsQuery(cmdu);
        break;

    case CmduTypes::ASSOC_STA_LINK_METRICS_RESPONSE:
        ProcessAssocStaLinkMetricsResponse(cmdu);
        break;

    case CmduTypes::UNASSOC_STA_LINK_METRICS_QUERY:
        ProcessUnassocStaLinkMetricsQuery(cmdu);
        break;

    case CmduTypes::UNASSOC_STA_LINK_METRICS_RESPONSE:
        ProcessUnassocStaLinkMetricsResponse(cmdu);
        break;

    /* =========================================================
     * EasyMesh – CLIENT STEERING / ROAMING (CORE KPI)
     * ========================================================= */
    case CmduTypes::CLIENT_STEERING_REQUEST:
        ProcessClientSteeringRequest(cmdu);
        break;

    case CmduTypes::CLIENT_STEERING_BTM_REQUEST:
        ProcessClientSteeringBtmRequest(cmdu);
        break;

    case CmduTypes::CLIENT_ASSOC_CONTROL_REQUEST:
        ProcessClientAssocControlRequest(cmdu);
        break;

    case CmduTypes::CLIENT_STEERING_COMPLETE:
        ProcessClientSteeringComplete(cmdu);
        break;

    /* =========================================================
     * EasyMesh – BACKHAUL
     * ========================================================= */
    case CmduTypes::BACKHAUL_STEERING_REQUEST:
        ProcessBackhaulSteeringRequest(cmdu);
        break;

    case CmduTypes::BACKHAUL_STEERING_RESPONSE:
        ProcessBackhaulSteeringResponse(cmdu);
        break;

    /* =========================================================
     * EasyMesh – CHANNEL SCAN
     * ========================================================= */
    case CmduTypes::CHANNEL_SCAN_REQUEST:
        ProcessChannelScanRequest(cmdu);
        break;

    case CmduTypes::CHANNEL_SCAN_RESPONSE:
        ProcessChannelScanResponse(cmdu);
        break;

    /* =========================================================
     * EasyMesh – Wi-Fi 6 / 7 / MLD
     * ========================================================= */
    case CmduTypes::AP_MLD_CONFIGURATION_REQUEST_MESSAGE:
        ProcessApMldConfigurationRequest(cmdu);
        break;

    case CmduTypes::AP_MLD_CONFIGURATION_RESPONSE_MESSAGE:
        ProcessApMldConfigurationResponse(cmdu);
        break;

    case CmduTypes::BSTA_MLD_CONFIGURATION_REQUEST_MESSAGE:
        ProcessBstaMldConfigurationRequest(cmdu);
        break;

    case CmduTypes::BSTA_MLD_CONFIGURATION_RESPONSE_MESSAGE:
        ProcessBstaMldConfigurationResponse(cmdu);
        break;

    /* =========================================================
     * DEFAULT
     * ========================================================= */
    default:
        EM_WARN("Ieee1905Agent: Unhandled CMDU {} (0x{:04x}), MID=0x{:04x}",
                CmduTypeToString(type), type, mid);
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

/* =========================================================
 * LINK METRICS
 * ========================================================= */
void Ieee1905Agent::ProcessLinkMetricQuery(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Link Metric Query.");
}

void Ieee1905Agent::ProcessLinkMetricResponse(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Link Metric Response.");
}

/* =========================================================
 * AP AUTOCONFIGURATION (FIXED NAME)
 * ========================================================= */
void Ieee1905Agent::ProcessAutoconfigurationSearch(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Autoconfiguration Search.");
}

void Ieee1905Agent::ProcessAutoconfigurationResponse(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Autoconfiguration Response.");
}

void Ieee1905Agent::ProcessAutoconfigurationWsc(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Autoconfiguration WSC.");
}

void Ieee1905Agent::ProcessAutoconfigurationRenew(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Autoconfiguration Renew.");
}

/* =========================================================
 * HIGHER LAYER / GENERIC PHY
 * ========================================================= */
void Ieee1905Agent::ProcessHigherLayerQuery(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Higher Layer Query.");
}

void Ieee1905Agent::ProcessHigherLayerResponse(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Higher Layer Response.");
}

void Ieee1905Agent::ProcessGenericPhyQuery(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Generic PHY Query.");
}

void Ieee1905Agent::ProcessGenericPhyResponse(const Cmdu& request)
{
    EM_DEBUG("Ieee1905Agent: Processing Generic PHY Response.");
}

/* ================= Client Capability ================= */

void Ieee1905Agent::ProcessClientCapabilityQuery(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessClientCapabilityQuery MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessClientCapabilityReport(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessClientCapabilityReport MID={}", cmdu.mid());
}

/* ================= AP Capability ================= */

void Ieee1905Agent::ProcessApCapabilityQuery(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessApCapabilityQuery MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessApCapabilityReport(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessApCapabilityReport MID={}", cmdu.mid());
}

/* ================= Channel Preference ================= */

void Ieee1905Agent::ProcessChannelPreferenceQuery(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessChannelPreferenceQuery MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessChannelPreferenceReport(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessChannelPreferenceReport MID={}", cmdu.mid());
}

/* ================= Channel Selection ================= */

void Ieee1905Agent::ProcessChannelSelectionRequest(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessChannelSelectionRequest MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessChannelSelectionResponse(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessChannelSelectionResponse MID={}", cmdu.mid());
}

/* ================= Channel Scan ================= */

void Ieee1905Agent::ProcessChannelScanRequest(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessChannelScanRequest MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessChannelScanResponse(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessChannelScanResponse MID={}", cmdu.mid());
}

/* ================= Operating Channel ================= */

void Ieee1905Agent::ProcessOperatingChannelReport(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessOperatingChannelReport MID={}", cmdu.mid());
}

/* ================= STA Link Metrics ================= */

void Ieee1905Agent::ProcessAssocStaLinkMetricsQuery(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessAssocStaLinkMetricsQuery MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessAssocStaLinkMetricsResponse(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessAssocStaLinkMetricsResponse MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessUnassocStaLinkMetricsQuery(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessUnassocStaLinkMetricsQuery MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessUnassocStaLinkMetricsResponse(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessUnassocStaLinkMetricsResponse MID={}", cmdu.mid());
}

/* ================= Client Steering ================= */

void Ieee1905Agent::ProcessClientSteeringRequest(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessClientSteeringRequest MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessClientSteeringBtmRequest(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessClientSteeringBtmRequest MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessClientSteeringComplete(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessClientSteeringComplete MID={}", cmdu.mid());
}

/* ================= Client Assoc Control ================= */

void Ieee1905Agent::ProcessClientAssocControlRequest(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessClientAssocControlRequest MID={}", cmdu.mid());
}

/* ================= Backhaul Steering ================= */

void Ieee1905Agent::ProcessBackhaulSteeringRequest(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessBackhaulSteeringRequest MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessBackhaulSteeringResponse(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessBackhaulSteeringResponse MID={}", cmdu.mid());
}

/* ================= MLD / Wi-Fi 7 ================= */

void Ieee1905Agent::ProcessApMldConfigurationRequest(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessApMldConfigurationRequest MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessApMldConfigurationResponse(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessApMldConfigurationResponse MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessBstaMldConfigurationRequest(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessBstaMldConfigurationRequest MID={}", cmdu.mid());
}

void Ieee1905Agent::ProcessBstaMldConfigurationResponse(const Cmdu &cmdu)
{
    SPDLOG_DEBUG("ProcessBstaMldConfigurationResponse MID={}", cmdu.mid());
}


} // namespace ieee1905_1