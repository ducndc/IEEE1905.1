/**
 * cmdu.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/cmdu.h"

#include <iostream>

namespace ieee1905_1 {

Cmdu::Cmdu(MessageType type, MessageId id, MessageFlags flags, FragmentId fragment_id)
    : m_type(type), m_id(id), m_flags(flags), m_fragment_id(fragment_id) 
{
    // No complex logic needed in this constructor
}

void Cmdu::AddTlv(Tlv&& tlv)
{
    // According to the 1905.1 standard, TLV_END_OF_MESSAGE must be the last TLV
    // We could add a check here to ensure the logic TLV_END_OF_MESSAGE
    // However, for simplicity, we just add it at the end:

    // Check if there is already an END_OF_MESSAGE (if there is, remove it first)
    if (!m_tlvs.empty() && m_tlvs.back().GetType() == TlvTypes::END_OF_MESSAGE) {
        // If there is already a TLV_END_OF_MESSAGE, push it out to add the new TLV in the middle
        Tlv end_tlv = std::move(m_tlvs.back());
        m_tlvs.pop_back();
        
        m_tlvs.push_back(std::move(tlv));
        m_tlvs.push_back(std::move(end_tlv));
    } else {
        // Add new TLV at the end
        m_tlvs.push_back(std::move(tlv));
    }
}

const Tlv* Cmdu::FindTlv(TlvType type) const 
{
    // Search the vector for TLVs
    auto it = std::find_if(m_tlvs.begin(), m_tlvs.end(), 
        [type](const Tlv& tlv) {
            return tlv.GetType() == type;
        });

    if (it != m_tlvs.end()) {
        // Returns a pointer to the found TLV
        return &(*it);
    }
    
    return nullptr;
}

// ---------------------------------------------
// Header Notes
// ---------------------------------------------
// Order of fields in Header 1905.1 (8 bytes):
// 1. Message Version (1 byte, 0x00)
// 2. Reserved (1 byte, always 0)
// 3. Message Type (2 bytes)
// 4. Message ID (2 bytes)
// 5. Fragment ID (1 byte)
// 6. Flags (1 byte: bit 7 last fragment, bit 6 relay, rest reserved)

} // namespace ieee1905_1