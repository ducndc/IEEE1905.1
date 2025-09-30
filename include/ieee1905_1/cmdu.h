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
    const MessageType TOPOLOGY_DISCOVERY               = 0x0000;
    const MessageType TOPOLOGY_NOTIFICATION            = 0x0001;
    const MessageType TOPOLOGY_QUERY                   = 0x0002;
    const MessageType TOPOLOGY_RESPONSE                = 0x0003;
    const MessageType VENDOR_SPECIFIC                  = 0x0004;
    const MessageType LINK_METRIC_QUERY                = 0x0005;
    const MessageType LINK_METRIC_RESPONSE             = 0x0006;
    const MessageType AP_AUTOCONFIGURATION_SEARCH      = 0x0007;
    const MessageType AP_AUTOCONFIGURATION_RESPONSE    = 0x0008;
    const MessageType AP_AUTOCONFIGURATION_WSC         = 0x0009;
    const MessageType AP_AUTOCONFIGURATION_RENEW       = 0x000a;
    const MessageType PUSH_BUTTON_EVENT_NOTIFICATION   = 0x000b;
    const MessageType PUSH_BUTTON_JOIN_NOTIFICATION    = 0x000c;
    const MessageType HIGHER_LAYER_QUERY               = 0x000d;
    const MessageType HIGHER_LAYER_RESPONSE            = 0x000e;
    const MessageType INTERFACE_POWER_CHANGE_REQUEST   = 0x000f;
    const MessageType INTERFACE_POWER_CHANGE_RESPONSE  = 0x0010;
    const MessageType GENERIC_PHY_QUERY                = 0x0011;
    const MessageType GENERIC_PHY_RESPONSE             = 0x0012;
}


} // namespace ieee1905_1

#endif // IEEE1905_1_CMDU_H