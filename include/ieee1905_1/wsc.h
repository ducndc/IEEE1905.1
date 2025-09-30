/**
 * wsc.h: wsc definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef IEEE1905_1_WSC_H
#define IEEE1905_1_WSC_H

#include "tlv.h"

#include <vector>
#include <cstdint>
#include <string>
#include <map>

namespace ieee1905_1 {

// TLV type 1905.1 is used to carry WSC data
namespace WscTlvType {
// Usually Vendor Specific TLV (0x0002) or a specific Type
// Assuming we use a specific Type in the 1905.1 range
    const TlvType WSC_MESSAGING_TLV = 0x8004; // WSC TLV
}

/**
 * @brief Defines common WSC Attributes (2 byte Type, 2 byte Length, Value).
 */
namespace WscAttributes {
    const uint16_t WSC_VERSION          = 0x104A; // WSC Version
    const uint16_t WSC_STATE            = 0x104A; // Current state
    const uint16_t WSC_AUTHENTICATION   = 0x1004; // Authentication Type
    const uint16_t WSC_SSID             = 0x1045; // Network SSID
    // ... and many other Attribute types
}

/**
 * @brief Represents the WSC data encapsulated in the Value field of TLV 1905.1.
 * This class handles the creation and parsing of WSC Attributes.
 */
class WscMessage {
public:
    /**
     * @brief Default constructor.
     */
    WscMessage() = default;

    /**
     * @brief Initialize WscMessage from raw data (after Parse TLV Value).
     * @param raw_wsc_data Raw WSC data (byte vector) from TLV Value.
     */
    explicit WscMessage(const std::vector<uint8_t>& raw_wsc_data);

    /**
     * @brief Adds a WSC Attribute to the message.
     * @param attribute_type Attribute type (2 bytes).
     * @param value_data Raw data of the Attribute Value.
     */
    void AddAttribute(uint16_t attribute_type, const std::vector<uint8_t>& value_data);

    /**
     * @brief Get the complete WSC data as TLV 1905.1.
     * @return A Tlv 1905.1 object containing this WSC message in the Value field.
     */
    Tlv ToTlv() const;

    /**
     * @brief Get the WSC Attribute Value data as a byte vector.
     * @param attribute_type The type of Attribute to look for.
     * @return the byte vector of the Attribute Value, or an empty vector if not found.
     */
    std::vector<uint8_t> GetAttributeValue(uint16_t attribute_type) const;

private:
    // Map stores WSC Attributes: Key=Type, Value=Value Data
    std::map<uint16_t, std::vector<uint8_t>> m_attributes;

    // Internal method to pack Attributes into byte buffer
    std::vector<uint8_t> PackAttributes() const;
    
    // Internal method to parse buffer bytes into Attributes
    void ParseAttributes(const std::vector<uint8_t>& raw_wsc_data);
};

} // namespace ieee1905_1

#endif // IEEE1905_1_WSC_H