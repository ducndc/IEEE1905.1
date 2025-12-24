/**
 * cmdu.h: cmdu definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef IEEE1905_1_CMDU_H
#define IEEE1905_1_CMDU_H

#include "tlv.h" 

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>

namespace ieee1905_1 {

// Define data types for Header
using MessageType = uint16_t;
using MessageId = uint16_t;
using MessageFlags = uint8_t; // Use only 1 byte for Flags/FragmentID

/**
 * @brief Represents a complete IEEE 1905.1 CMDU Control packet.
 */
class Cmdu {
public:
    // Fixed size of CMDU Header (not including Ethernet Header)
    static constexpr size_t CMDU_HEADER_SIZE = 
        sizeof(MessageType) + 
        sizeof(MessageId) + 
        sizeof(MessageFlags) + 
        1; // Reserved Byte

    /**
     * @brief Constructs a new CMDU.
     * @param type Message type (e.g., DISCOVERY, TOPOLOGY QUERY).
     * @param id Unique transaction ID.
     * @param flags Message flags (e.g., FragmentID bit).
     */
    Cmdu(MessageType type, MessageId id, MessageFlags flags = 0);

    // Defaulting operators
    Cmdu(const Cmdu&) = default;
    Cmdu& operator=(const Cmdu&) = default;
    Cmdu(Cmdu&&) = default;
    Cmdu& operator=(Cmdu&&) = default;
    ~Cmdu() = default;

    // --- Header access functions ---
    MessageType GetType() const { return m_type; }
    MessageId GetId() const { return m_id; }
    MessageFlags GetFlags() const { return m_flags; }
    
    // --- TLV management functions ---

    /**
     * @brief Add a TLV to the end of the list.
     * @param tlv Tlv object (will be moved).
     */
    void AddTlv(Tlv&& tlv);

    /**
     * @brief Get a constant reference to a list of TLVs.
     */
    const std::vector<Tlv>& GetTlvs() const { return m_tlvs; }

    /**
     * @brief Search and return a pointer to the first TLV with a matching Type.
     * @param type Type of TLV to search.
     * @return Pointer to Tlv or nullptr if not found.
     */
    const Tlv* FindTlv(TlvType type) const;

private:
    MessageType m_type;
    MessageId m_id;
    MessageFlags m_flags;
    uint8_t m_reserved = 0; // Always 0 by standard
    
    std::vector<Tlv> m_tlvs;
};

// Define Message Type constants
namespace CmduTypes {
    const MessageType TOPOLOGY_DISCOVERY = 0x0000;
    const MessageType TOPOLOGY_NOTIFICATION = 0x0001;
    const MessageType TOPOLOGY_QUERY = 0x0002;
    const MessageType TOPOLOGY_RESPONSE = 0x0003;
    const MessageType VENDOR_SPECIFIC = 0x0004;
    const MessageType LINK_METRIC_QUERY = 0x0005;
    const MessageType LINK_METRIC_RESPONSE = 0x0006;
    const MessageType AP_AUTOCONFIGURATION_SEARCH = 0x0007;
    const MessageType AP_AUTOCONFIGURATION_RESPONSE = 0x0008;
    const MessageType AP_AUTOCONFIGURATION_WSC = 0x0009;
    const MessageType AP_AUTOCONFIGURATION_RENEW = 0x000a;
    const MessageType PUSH_BUTTON_EVENT_NOTIFICATION = 0x000b;
    const MessageType PUSH_BUTTON_JOIN_NOTIFICATION = 0x000c;
    const MessageType HIGHER_LAYER_QUERY = 0x000d;
    const MessageType HIGHER_LAYER_RESPONSE = 0x000e;
    const MessageType INTERFACE_POWER_CHANGE_REQUEST = 0x000f;
    const MessageType INTERFACE_POWER_CHANGE_RESPONSE = 0x0010;
    const MessageType GENERIC_PHY_QUERY = 0x0011;
    const MessageType GENERIC_PHY_RESPONSE = 0x0012;
    const MessageType ACK_1905 = 0x8000;
    const MessageType AP_CAPABILITY_QUERY = 0x8001;
    const MessageType AP_CAPABILITY_REPORT = 0x8002;
    const MessageType MAP_POLICY_CONFIG_REQUEST = 0x8003;
    const MessageType CHANNEL_PREFERENCE_QUERY = 0x8004;
    const MessageType CHANNEL_PREFERENCE_REPORT = 0x8005;
    const MessageType CHANNEL_SELECTION_REQUEST = 0x8006;
    const MessageType CHANNEL_SELECTION_RESPONSE = 0x8007;
    const MessageType OPERATING_CHANNEL_REPORT = 0x8008;
    const MessageType CLIENT_CAPABILITY_QUERY = 0x8009;
    const MessageType CLIENT_CAPABILITY_REPORT = 0x800a;
    const MessageType AP_LINK_METRICS_QUERY = 0x800b;
    const MessageType AP_LINK_METRICS_RESPONSE = 0x800c;
    const MessageType ASSOC_STA_LINK_METRICS_QUERY = 0x800d;
    const MessageType ASSOC_STA_LINK_METRICS_RESPONSE = 0x800e;
    const MessageType UNASSOC_STA_LINK_METRICS_QUERY = 0x800f;
    const MessageType UNASSOC_STA_LINK_METRICS_RESPONSE = 0x8010;
    const MessageType BEACON_METRICS_QUERY = 0x8011;
    const MessageType BEACON_METRICS_RESPONSE = 0x8012;
    const MessageType COMBINED_INFRASTRUCTURE_METRICS = 0x8013;
    const MessageType CLIENT_STEERING_REQUEST = 0x8014;
    const MessageType CLIENT_STEERING_BTM_REQUEST = 0x8015;
    const MessageType CLIENT_ASSOC_CONTROL_REQUEST = 0x8016;
    const MessageType CLIENT_STEERING_COMPLETE = 0x8017;
    const MessageType HIGHER_LAYER_DATA_MESSAGE = 0x8018;
    const MessageType BACKHAUL_STEERING_REQUEST = 0x8019;
    const MessageType BACKHAUL_STEERING_RESPONSE = 0x801a;
    const MessageType CHANNEL_SCAN_REQUEST = 0x801b;
    const MessageType CHANNEL_SCAN_RESPONSE = 0x801c;
    const MessageType DPP_CCE_INDICATION_MESSAGE = 0x801d;
    const MessageType _1905_REKEY_REQUEST = 0x801e;
    const MessageType _1905_DESCRIPTION_FAILURE = 0x801f;
    const MessageType CAC_REQUEST = 0x8020;
    const MessageType CAC_TERMINATION = 0x8021;
    const MessageType CLIENT_DISASSOCIATION_STATS = 0x8022;
    const MessageType SERVICE_PRIORITIZATION_STATS = 0x8023;
    const MessageType ERROR_RESPONSE = 0x8024;
    const MessageType ASSOCIATION_STATUS_NOTIFICATION = 0x8025;
    const MessageType TUNNELED_MESSAGE = 0x8026;
    const MessageType BACKHAUL_STA_CAP_QUERY_MESSAGE = 0x8027;
    const MessageType BACKHAUL_STA_CAP_REPORT_MESSAGE = 0x8028;
    const MessageType PROXIED_ENCAP_DPP_MESSAGE = 0x8029;
    const MessageType DIRECT_ENCAP_DPP_MESSAGE = 0x802a;
    const MessageType BSS_RECONFIGURATION_TRIGGER_MESSAGE = 0x802b;
    const MessageType BSS_CONFIGURATION_REQUEST_MESSAGE = 0x802c;
    const MessageType BSS_CONFIGURATION_RESPONSE_MESSAGE = 0x802d;
    const MessageType BSS_CONFIGURATION_RESULT_MESSAGE = 0x802e;
    const MessageType CHIRP_NOTIFICATION_MESSAGE = 0x802f;
    const MessageType _1905_ENCAP_EAPOL = 0x8030;
    const MessageType DPP_BOOTSTRAPING_URI_NOTIFICATION = 0x8031;
    const MessageType DPP_BOOTSTRAPING_URI_QUERY = 0x8032;
    const MessageType FAILED_CONNECTION_MESSAGE = 0x8033;
    const MessageType DPP_URI_NOTIFICATION_MESSAGE = 0x8034;
    const MessageType AGENT_LIST_MESSAGE = 0x8035;
    const MessageType QOS_MANAGEMENT_NOTIFICATION = 0x8037;
    const MessageType EARLY_AP_CAP_REPORT_MESSAGE = 0x8043;
    const MessageType AP_MLD_CONFIGURATION_REQUEST_MESSAGE = 0x8044;
    const MessageType AP_MLD_CONFIGURATION_RESPONSE_MESSAGE = 0x8045;
    const MessageType BSTA_MLD_CONFIGURATION_REQUEST_MESSAGE = 0x8046;
    const MessageType BSTA_MLD_CONFIGURATION_RESPONSE_MESSAGE = 0x8047;
    const MessageType AFC_SPECTRUM_INQUIRY_MESSAGE = 0x8048;
    const MessageType DEV_SEND_1905_REQUEST = 0x9000;
}


} // namespace ieee1905_1

#endif // IEEE1905_1_CMDU_H