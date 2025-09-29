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
    TlvType type_;
    std::vector<uint8_t> value_;	
};

namespace TlvTypes {
    constexpr TlvType END_OF_MESSAGE  = 0x0000;
    constexpr TlvType DEVICE_INFO     = 0x0004;
    constexpr TlvType LINK_METRIC     = 0x000A;
}

} // namespace ieee1905_1

#endif // IEEE1905_1_TLV_H