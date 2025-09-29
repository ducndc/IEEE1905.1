#include "ieee1905_1/tlv.h"

using namespace ieee1905_1;

Tlv::Tlv(TlvType type, const std::vector<uint8_t>& value_data)
	: type_(type), value_(value_data) 
{
	if (value_data.size() > 0xFFFF) {
		throw std::length_error("TLV value length exceeds maximum 16-bit limit (65535 bytes).");
	}
}

Tlv::Tlv(TlvType type, std::vector<uint8_t>&& value_data)
	: type_(type), value_(std::move(value_data))
{
	if (value_.size() > 0xFFFF) {
		throw std::length_error("TLV value length exceeds maximum 16-bit limit (65535 bytes).");
	}
}

TlvType Tlv::GetType() const 
{ 
	return type_; 
}

TlvLength Tlv::GetLength() const 
{ 
	return static_cast<TlvLength>(value_.size()); 
}

const std::vector<uint8_t>& Tlv::GetValue() const 
{ 
	return value_; 
}

size_t Tlv::GetPacketSize() const 
{ 
	return TLV_HEADER_SIZE + value_.size(); 
}