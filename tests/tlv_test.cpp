/**
 * tlv_test.cpp
 */

#include "gtest/gtest.h"
#include "ieee1905_1/tlv.h"
#include "ieee1905_1/cmdu.h"
#include "ieee1905_1/serializer.h"
#include "ieee1905_1/parser.h"

using namespace ieee1905_1;

// ==========================================================================================
// 1. TLV CLASS TESTS
// ==============================================================================================

/**
 * @brief Test the basic properties of TLV after initialization.
 */
TEST(TlvTest, BasicInitialization)
{
    TlvType type = TlvTypes::VENDOR_SPECIFIC;
    std::vector<uint8_t> value_data = {0xAA, 0xBB, 0xCC, 0xDD};

    Tlv tlv(type, value_data);

    // Check Type, Length and Packed Size
    ASSERT_EQ(tlv.GetType(), type);
    ASSERT_EQ(tlv.GetLength(), 4);                            // 4 bytes in value_data
    ASSERT_EQ(tlv.GetPacketSize(), Tlv::TLV_HEADER_SIZE + 4); // 3 + 4 = 7 bytes

    // Check Value content
    ASSERT_EQ(tlv.GetValue().size(), 4);
    ASSERT_EQ(tlv.GetValue(), value_data);
}

/**
 * @brief Check Move constructor and transfer of data ownership.
 */
TEST(TlvTest, MoveConstructor)
{
    TlvType type = TlvTypes::AL_MAC_ADDRESS_TYPE;
    std::vector<uint8_t> original_data = {0x01, 0x02, 0x03};
    size_t original_size = original_data.size();

    // Create TLV by Moving data
    Tlv tlv(type, std::move(original_data));

    // Test new TLV
    ASSERT_EQ(tlv.GetType(), type);
    ASSERT_EQ(tlv.GetLength(), original_size);

    // In C++11 and above, moved vectors can have size() of 0
    // (depending on implementation), but original_data should no longer be used.
    // Check data in new TLV
    ASSERT_EQ(tlv.GetValue()[0], 0x01);
}

/**
 * @brief Check for empty TLV generation (like TLV_END_OF_MESSAGE).
 */
TEST(TlvTest, EmptyTlv)
{
    TlvType type = TlvTypes::END_OF_MESSAGE;
    std::vector<uint8_t> empty_data = {};

    Tlv tlv(type, empty_data);

    ASSERT_EQ(tlv.GetType(), type);
    ASSERT_EQ(tlv.GetLength(), 0);
    ASSERT_EQ(tlv.GetPacketSize(), Tlv::TLV_HEADER_SIZE); // Only 3 bytes Header
}

/**
 * @brief Check exception when Value length exceeds 16-bit limit.
 */
TEST(TlvTest, LengthLimitCheck)
{
    TlvType type = 0xFF;
    // Create vector with size 0x10000 (65536) bytes, exceeding 0xFFFF
    std::vector<uint8_t> oversized_data(0x10000);

    // Check if constructor throws std::length_error
    ASSERT_THROW({ Tlv tlv(type, oversized_data); }, std::length_error);
}

// ==========================================
// 2. SERIALIZATION/PARSING TESTS
// ====================================================================

/**
* @brief Test the TLV packaging and parsing.

* This is an integration test between Tlv, Cmdu, Serializer and Parser.
*/
TEST(TlvTest, SerializationAndParsingRoundTrip)
{
    // --- 1. Create CMDU with custom TLVs ---
    MessageType msg_type = CmduTypes::LINK_METRIC_QUERY;
    MessageId msg_id = 0xABBA;
    MessageFlags msg_flags = CmduFlags::LAST_FRAGMENT | CmduFlags::RELAY;

    Cmdu original_cmdu(msg_type, msg_id, msg_flags);

    // Add TLV 1: DEVICE_INFO (6 bytes MAC)
    std::vector<uint8_t> mac_address = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    original_cmdu.AddTlv(Tlv(TlvTypes::DEVICE_INFORMATION_TYPE, std::move(mac_address)));

    // Add TLV 2: LINK_METRIC_QUERY (1 byte Query)
    std::vector<uint8_t> query_byte = {0x01};
    original_cmdu.AddTlv(Tlv(TlvTypes::LINK_METRIC_QUERY, std::move(query_byte)));

    // --- 2. Pack ---
    std::vector<uint8_t> packed_buffer = MessageSerializer::Pack(original_cmdu);

    // Check packed buffer size
    // Header (8) + TLV1 (3+6) + TLV2 (3+1) + END_OF_MESSAGE (3+0) = 8 + 9 + 4 + 3 = 24
    ASSERT_EQ(packed_buffer.size(), 24);

    // --- 3. Parse ---
    Cmdu parsed_cmdu = MessageParser::Parse(packed_buffer);

    // --- 4. Check Parsed Header ---
    ASSERT_EQ(parsed_cmdu.GetType(), msg_type);
    ASSERT_EQ(parsed_cmdu.GetId(), msg_id);
    ASSERT_EQ(parsed_cmdu.GetFragmentId(), 0);
    ASSERT_EQ(parsed_cmdu.GetFlags(), msg_flags);

    // --- 5. Check the Parsed TLVs ---
    const auto &parsed_tlvs = parsed_cmdu.GetTlvs();
    ASSERT_EQ(parsed_tlvs.size(), 3); // TLV1 + TLV2 + END_OF_MESSAGE

    // Check TLV 1
    const Tlv *tlv1 = parsed_cmdu.FindTlv(TlvTypes::DEVICE_INFORMATION_TYPE);
    ASSERT_NE(tlv1, nullptr);
    ASSERT_EQ(tlv1->GetLength(), 6);

    // Check TLV 2
    const Tlv *tlv2 = parsed_cmdu.FindTlv(TlvTypes::LINK_METRIC_QUERY);
    ASSERT_NE(tlv2, nullptr);
    ASSERT_EQ(tlv2->GetLength(), 1);
    ASSERT_EQ(tlv2->GetValue()[0], 0x01);

    // Check the last TLV (END_OF_MESSAGE)
    ASSERT_EQ(parsed_tlvs.back().GetType(), TlvTypes::END_OF_MESSAGE);
    ASSERT_EQ(parsed_tlvs.back().GetLength(), 0);
}

/**
 * @brief Checks for an exception when the parser receives a buffer that is too short.
 */
TEST(TlvTest, ParserIncompleteHeader)
{
    // Buffer is only 5 bytes, smaller than CMDU_HEADER_SIZE (8)
    std::vector<uint8_t> short_buffer = {0x00, 0x01, 0xAA, 0xBB, 0xCC};

    ASSERT_THROW({ MessageParser::Parse(short_buffer); }, std::runtime_error);
}
/**
 * @brief Checks the exact wire layout of the CMDU header and TLV headers (IEEE 1905.1-2013, 6.2/6.3).
 */
TEST(TlvTest, SerializationWireFormat)
{
    Cmdu cmdu(CmduTypes::TOPOLOGY_QUERY, 0x1234);
    cmdu.AddTlv(Tlv(TlvTypes::AL_MAC_ADDRESS_TYPE, {0x02, 0x00, 0x00, 0x00, 0x00, 0x01}));

    std::vector<uint8_t> expected = {
        0x00, 0x00,                         // Version, Reserved
        0x00, 0x02,                         // Message Type (TOPOLOGY_QUERY)
        0x12, 0x34,                         // Message ID
        0x00,                               // Fragment ID
        0x80,                               // Flags (last fragment)
        0x01, 0x00, 0x06,                   // TLV Type (AL MAC), Length
        0x02, 0x00, 0x00, 0x00, 0x00, 0x01, // TLV Value
        0x00, 0x00, 0x00,                   // END_OF_MESSAGE
    };

    ASSERT_EQ(MessageSerializer::Pack(cmdu), expected);
}

/**
 * @brief Checks that Ethernet padding after END_OF_MESSAGE is ignored by the parser.
 */
TEST(TlvTest, ParserIgnoresPaddingAfterEndOfMessage)
{
    Cmdu cmdu(CmduTypes::TOPOLOGY_QUERY, 0x0042);
    std::vector<uint8_t> buffer = MessageSerializer::Pack(cmdu);

    // Pad to the minimum Ethernet payload size (46 bytes)
    buffer.resize(46, 0x00);

    Cmdu parsed_cmdu = MessageParser::Parse(buffer);

    ASSERT_EQ(parsed_cmdu.GetType(), CmduTypes::TOPOLOGY_QUERY);
    ASSERT_EQ(parsed_cmdu.GetId(), 0x0042);
    ASSERT_EQ(parsed_cmdu.GetTlvs().size(), 1); // END_OF_MESSAGE only
    ASSERT_EQ(parsed_cmdu.GetTlvs().back().GetType(), TlvTypes::END_OF_MESSAGE);
}
