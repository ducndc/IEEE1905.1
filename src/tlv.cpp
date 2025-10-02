/**
 * tlv.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/tlv.h"
#include "ieee1905_1/logger.h"

using namespace ieee1905_1;

Tlv::Tlv(TlvType type, const std::vector<uint8_t>& value_data)
	: m_type(type), m_value(value_data) 
{
	if (value_data.size() > 0xFFFF) {
		EM_ERROR("TLV value length exceeds maximum 16-bit limit (65535 bytes).");
	}
}

Tlv::Tlv(TlvType type, std::vector<uint8_t>&& value_data)
	: m_type(type), m_value(std::move(value_data))
{
	if (m_value.size() > 0xFFFF) {
		EM_ERROR("TLV value length exceeds maximum 16-bit limit (65535 bytes).");
	}
}

TlvType Tlv::GetType() const 
{ 
	return m_type; 
}

TlvLength Tlv::GetLength() const 
{ 
	return static_cast<TlvLength>(m_value.size()); 
}

const std::vector<uint8_t>& Tlv::GetValue() const 
{ 
	return m_value; 
}

size_t Tlv::GetPacketSize() const 
{ 
	return TLV_HEADER_SIZE + m_value.size(); 
}