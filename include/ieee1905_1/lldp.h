/**
 * lldp.h: lldp definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef IEEE1905_1_LLDP_H
#define IEEE1905_1_LLDP_H

#include "tlv.h" 

#include <vector>
#include <cstdint>
#include <stdexcept>

namespace ieee1905_1 {

/**
 * @brief Defines the mandatory and common TLV types for LLDP (IEEE 802.1AB).
 * LLDP TLV Type uses only 7 bits.
 */
namespace LldpTlvTypes {
    // Required TLV type
    const uint8_t END_OF_LLDPDU = 0;        // Required: Mark the end of the LLDP PDU
    const uint8_t CHASSIS_ID    = 1;        // Required: Chassis identification of the sending device
    const uint8_t PORT_ID       = 2;        // Required: Port identification of the sending device
    const uint8_t TTL           = 3;        // Required: Time-to-Live
    
    // Optional TLV type
    const uint8_t PORT_DESC     = 4;        // Port Description
    const uint8_t SYSTEM_NAME   = 5;        // System name
    const uint8_t SYSTEM_DESC   = 6;        // System description
    const uint8_t CAPABILITIES  = 7;        // System capabilities (Bridge, Router, etc.)
    const uint8_t MGMT_ADDR     = 8;        // Management Address

    // Organizational Specific TLV
    const uint8_t ORGANIZATION_SPECIFIC = 127; 
}

/**
 * @brief Represents an LLDP PDU (Protocol Data Unit) packet.
 * An LLDP PDU is a sequence of LLDP TLVs, ending with the END_OF_LLDPDU TLV.
 */
class LldpPdu {
public:
    // LLDP TLV header size is 2 bytes (Type 7-bit, Length 9-bit)
    static constexpr size_t LLDP_TLV_HEADER_SIZE = 2;

    /**
     * @brief Initialize a new LLDP PDU, automatically adding END_OF_LLDPDU.
     */
    LldpPdu();
    
    // Default the operators
    LldpPdu(const LldpPdu&) = default;
    LldpPdu& operator=(const LldpPdu&) = default;
    LldpPdu(LldpPdu&&) = default;
    LldpPdu& operator=(LldpPdu&&) = default;
    ~LldpPdu() = default;

    /**
     * @brief Add an LLDP TLV to the PDU.
     * @param tlv The Tlv object (to be moved).
     */
    void AddTlv(Tlv&& tlv);

    /**
     * @brief Get a constant reference to the list of TLVs.
     */
    const std::vector<Tlv>& GetTlvs() const { return m_tlvs; }

    /**
     * @brief Calculates the total encapsulated size of the PDU (excluding Ethernet Header).
     * Note: This calculation must use LLDP_TLV_HEADER_SIZE = 2 bytes.
     */
    size_t GetPackedSize() const;

private:
    std::vector<Tlv> m_tlvs;
};

} // namespace ieee1905_1

#endif // IEEE1905_1_LLDP_H