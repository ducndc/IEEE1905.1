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

Cmdu::Cmdu(MessageType type, MessageId id, MessageFlags flags)
    : type_(type), id_(id), flags_(flags) 
{
    // No complex logic needed in this constructor
}

void Cmdu::AddTlv(Tlv&& tlv) 
{
    // According to the 1905.1 standard, TLV_END_OF_MESSAGE must be the last TLV
    // We could add a check here to ensure the logic TLV_END_OF_MESSAGE
    // However, for simplicity, we just add it at the end:

    // Check if there is already an END_OF_MESSAGE (if there is, remove it first)
    if (!tlvs_.empty() && tlvs_.back().GetType() == TlvTypes::END_OF_MESSAGE) {
        // If there is already a TLV_END_OF_MESSAGE, push it out to add the new TLV in the middle
        Tlv end_tlv = std::move(tlvs_.back());
        tlvs_.pop_back();
        
        tlvs_.push_back(std::move(tlv));
        tlvs_.push_back(std::move(end_tlv));
    } else {
        // Add new TLV at the end
        tlvs_.push_back(std::move(tlv));
    }
}

const Tlv* Cmdu::FindTlv(TlvType type) const 
{
    // Search the vector for TLVs
    auto it = std::find_if(tlvs_.begin(), tlvs_.end(), 
        [type](const Tlv& tlv) {
            return tlv.GetType() == type;
        });

    if (it != tlvs_.end()) {
        // Returns a pointer to the found TLV
        return &(*it);
    }
    
    return nullptr;
}

// ---------------------------------------------
// Header Notes
// ---------------------------------------------
// Order of fields in Header 1905.1:
// 1. Message Type (2 bytes)
// 2. Message ID (2 bytes)
// 3. Fragment ID and Flags (1 byte)
// 4. Reserved (1 byte, always 0)
// 5. Reserved (2 bytes, always 0)
// Total size is 6 bytes + 2 Reserved bytes, or 4 bytes depending on the variant.
// We are using: 2+2+1+1 = 6 bytes (and 2 more Reserved bytes in practice if following the standard).
// For now, keep CMDU_HEADER_SIZE = 6 bytes for simplicity.

} // namespace ieee1905_1