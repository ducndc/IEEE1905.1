/**
 * lldp.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/lldp.h"
#include <iostream>

namespace ieee1905_1 {

LldpPdu::LldpPdu() 
{
    // According to LLDP standard, PDU must end with TLV_END_OF_LLDPDU (Type 0).
    // We will add it at the end when Pack, or make sure it is always the last element.
    // For simplicity, we just add it at the end when packing.
}

void LldpPdu::AddTlv(Tlv&& tlv) 
{
    // Check and prevent premature TLV_END_OF_LLDPDU addition
    if (tlv.GetType() == LldpTlvTypes::END_OF_LLDPDU) {
        throw std::invalid_argument("END_OF_LLDPDU must not be added manually. It is managed by LldpPdu.");
    }

    // Check LLDP TLV Type limit (7 bit)
    if (tlv.GetType() > LldpTlvTypes::ORGANIZATION_SPECIFIC) {
        throw std::invalid_argument("LLDP TLV Type exceeds 7-bit limit (max 127).");
    }

    // Check LLDP TLV Length limit (9 bits)
    if (tlv.GetLength() > 0x1FF) { // 2^9 - 1 = 511 bytes
        throw std::length_error("LLDP TLV Value length exceeds 9-bit limit (max 511 bytes).");
    }

    // Add TLV to the list
    m_tlvs.push_back(std::move(tlv));
}

size_t LldpPdu::GetPackedSize() const 
{
    size_t total_size = 0;

    // Calculate the size for each TLV (Header 2 bytes + Value Length)
    for (const auto& tlv : m_tlvs) {
        // Check again, in case TLV is added with too large Length
        if (tlv.GetLength() > 0x1FF) {
             throw std::runtime_error("Cannot pack: A TLV length exceeds 511 bytes.");
        }
        total_size += LLDP_TLV_HEADER_SIZE + tlv.GetLength();
    }

    // Required to add the size of TLV_END_OF_LLDPDU
    // END_OF_LLDPDU (Type 0, Length 0) has a size of 2 bytes header
    total_size += LLDP_TLV_HEADER_SIZE; 
    
    return total_size;
}

// ---------------------------------------------
// Notes on LLDP Serializer/Parser
// ---------------------------------------------
/**
 * Important Note: To Pack or Parse LldpPdu,
 * you will need a separate MessageSerializer/Parser for LLDP.

 * The LLDP packing function will need to perform the following bitwise operation:
 * uint16_t packed_header = (tlv.GetType() << 9) | tlv.GetLength();
 * Then write this packed_header in Network Byte Order (Big-Endian) to the buffer.
 */

} // namespace ieee1905_1