/**
 * tlv.h: tlv definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef IEEE1905_1_TLV_H
#define IEEE1905_1_TLV_H 

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <utility>

namespace ieee1905_1 {

using TlvType = uint16_t;
using TlvLength = uint16_t;

class Tlv {
public:

	static constexpr size_t TLV_HEADER_SIZE = sizeof(TlvType) + sizeof(TlvLength);

	Tlv(TlvType type, std::vector<uint8_t>&& value_data);
	Tlv(TlvType type, const std::vector<uint8_t>& value_data);

	Tlv& operator=(const Tlv&) = default;
	Tlv(Tlv&&) = default;
	Tlv& operator=(Tlv&&) = default;
	~Tlv() = default;

	/**
	 * @brief Get type of TLV
	 * @return TlvType.
	 */
	TlvType GetType() const;

	/**
	 * @brief Get length of TLV
	 * @return TlvLength
	 */
	TlvLength GetLength() const;

	/**
	 * @brief Get data of Value 
	 * @return Reference to vector byte data
	 */
	const std::vector<uint8_t>& GetValue() const;

	/**
	 * @brief Get size of TLV
	 * @return Bytes (Header 4 bytes + Length)
	 */
	size_t GetPacketSize() const;
private:
    TlvType m_type;
    std::vector<uint8_t> m_value;	
};

namespace TlvTypes {
    constexpr TlvType END_OF_MESSAGE                      = 0;
	constexpr TlvType AL_MAC_ADDRESS_TYPE                 = 1;
	constexpr TlvType MAC_ADDRESS_TYPE                    = 2;
	constexpr TlvType DEVICE_INFORMATION_TYPE             = 3;
	constexpr TlvType DEVICE_BRIDGING_CAPABILITIES        = 4;
	constexpr TlvType NON_1905_NEIGHBOR_DEVICE_LIST       = 6;
	constexpr TlvType NEIGHBOR_DEVICE_LIST                = 7;
	constexpr TlvType LINK_METRIC_QUERY                   = 8;
	constexpr TlvType TRANSMITTER_LINK_METRIC             = 9;
	constexpr TlvType RECEIVER_LINK_METRIC                = 10;
	constexpr TlvType VENDOR_SPECIFIC                     = 11;
	constexpr TlvType LINK_METRIC_RESULT_CODE             = 12;
	constexpr TlvType SEARCHED_ROLE                       = 13;
	constexpr TlvType AUTOCONFIG_FREQ_BAND                = 14;
	constexpr TlvType SUPPORTED_ROLE                      = 15;
	constexpr TlvType SUPPORTED_FREQ_BAND                 = 16;
	constexpr TlvType WSC                                 = 17;
	constexpr TlvType PUSH_BUTTON_EVENT_NOTIFICATION      = 18;
	constexpr TlvType PUSH_BUTTON_JOIN_NOTIFICATION       = 19;
	constexpr TlvType GENERIC_PHY_DEVICE_INFORMATION      = 20;
	constexpr TlvType DEVICE_IDENTIFICATION               = 21;
	constexpr TlvType CONTROL_URL                         = 22;
	constexpr TlvType IPV4                                = 23;
	constexpr TlvType IPV6                                = 24;
	constexpr TlvType GENERIC_PHY_EVENT_NOTIFICATION      = 25;
	constexpr TlvType _1905_PROFILE_VERSION               = 26;
	constexpr TlvType POWER_OFF_INTERFACE                 = 27;
	constexpr TlvType INTERFACE_POWER_CHANGE_INFORMATION  = 28;
	constexpr TlvType INTERFACE_POWER_CHANGE_STATUS       = 29;
	constexpr TlvType L2_NEIGHBOR_DEVICE                  = 30;
}

} // namespace ieee1905_1

#endif // IEEE1905_1_TLV_H