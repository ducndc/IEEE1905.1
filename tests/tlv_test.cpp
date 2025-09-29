// tlv_test.cpp
#include "gtest/gtest.h"
#include "ieee1905_1/tlv.h"
#include "ieee1905_1/cmdu.h"
#include "ieee1905_1/serializer.h"
#include "ieee1905_1/parser.h"

using namespace ieee1905_1;

// =======================================================
// 1. TLV CLASS TESTS
// =======================================================

/**
 * @brief Kiểm tra các thuộc tính cơ bản của TLV sau khi khởi tạo.
 */
TEST(TlvTest, BasicInitialization) {
    TlvType type = 0x0102;
    std::vector<uint8_t> value_data = {0xAA, 0xBB, 0xCC, 0xDD};

    Tlv tlv(type, value_data);

    // Kiểm tra Type, Length và Kích thước đóng gói
    ASSERT_EQ(tlv.GetType(), type);
    ASSERT_EQ(tlv.GetLength(), 4); // 4 bytes trong value_data
    ASSERT_EQ(tlv.GetPackedSize(), Tlv::TLV_HEADER_SIZE + 4); // 4 + 4 = 8 bytes
    
    // Kiểm tra nội dung Value
    ASSERT_EQ(tlv.GetValue().size(), 4);
    ASSERT_EQ(tlv.GetValue(), value_data);
}

/**
 * @brief Kiểm tra cấu tử Move và việc chuyển quyền sở hữu dữ liệu.
 */
TEST(TlvTest, MoveConstructor) {
    TlvType type = 0x0001;
    std::vector<uint8_t> original_data = {0x01, 0x02, 0x03};
    size_t original_size = original_data.size();
    
    // Tạo TLV bằng cách Move data
    Tlv tlv(type, std::move(original_data));

    // Kiểm tra TLV mới
    ASSERT_EQ(tlv.GetType(), type);
    ASSERT_EQ(tlv.GetLength(), original_size);
    
    // Trong C++11 trở lên, vector đã move có thể có size() là 0 
    // (tùy thuộc vào implementation), nhưng không nên dùng original_data nữa.
    // Kiểm tra dữ liệu trong TLV mới
    ASSERT_EQ(tlv.GetValue()[0], 0x01);
}

/**
 * @brief Kiểm tra việc tạo TLV rỗng (như TLV_END_OF_MESSAGE).
 */
TEST(TlvTest, EmptyTlv) {
    TlvType type = TlvTypes::END_OF_MESSAGE;
    std::vector<uint8_t> empty_data = {};

    Tlv tlv(type, empty_data);

    ASSERT_EQ(tlv.GetType(), type);
    ASSERT_EQ(tlv.GetLength(), 0);
    ASSERT_EQ(tlv.GetPackedSize(), Tlv::TLV_HEADER_SIZE); // Chỉ có 4 bytes Header
}

/**
 * @brief Kiểm tra ngoại lệ khi độ dài Value vượt quá giới hạn 16-bit.
 */
TEST(TlvTest, LengthLimitCheck) {
    TlvType type = 0xFFFF;
    // Tạo vector có kích thước 0x10000 (65536) bytes, vượt quá 0xFFFF
    std::vector<uint8_t> oversized_data(0x10000); 

    // Kiểm tra xem cấu tử có ném ra std::length_error không
    ASSERT_THROW({
        Tlv tlv(type, oversized_data);
    }, std::length_error);
}

// =======================================================
// 2. SERIALIZATION/PARSING TESTS
// =======================================================

/**
 * @brief Kiểm tra việc đóng gói (Pack) và phân tích cú pháp (Parse) TLV.
 * Đây là bài kiểm tra tích hợp giữa Tlv, Cmdu, Serializer và Parser.
 */
TEST(TlvTest, SerializationAndParsingRoundTrip) {
    // --- 1. Tạo CMDU với các TLV tùy chỉnh ---
    MessageType msg_type = CmduTypes::LINK_METRIC_REQUEST;
    MessageId msg_id = 0xABBA;
    MessageFlags msg_flags = 0x01;
    
    Cmdu original_cmdu(msg_type, msg_id, msg_flags);

    // Thêm TLV 1: DEVICE_INFO (6 bytes MAC)
    std::vector<uint8_t> mac_address = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    original_cmdu.AddTlv(Tlv(TlvTypes::DEVICE_INFO, std::move(mac_address)));

    // Thêm TLV 2: LINK_METRIC (1 byte Query)
    std::vector<uint8_t> query_byte = {0x01};
    original_cmdu.AddTlv(Tlv(TlvTypes::LINK_METRIC, std::move(query_byte)));

    // --- 2. Đóng gói (Pack) ---
    std::vector<uint8_t> packed_buffer = MessageSerializer::Pack(original_cmdu);

    // Kiểm tra kích thước buffer đóng gói
    // Header (6) + TLV1 (4+6) + TLV2 (4+1) + END_OF_MESSAGE (4+0) = 6 + 10 + 5 + 4 = 25
    ASSERT_EQ(packed_buffer.size(), 25); 

    // --- 3. Phân tích cú pháp (Parse) ---
    Cmdu parsed_cmdu = MessageParser::Parse(packed_buffer);

    // --- 4. Kiểm tra Header đã được Parse ---
    ASSERT_EQ(parsed_cmdu.GetType(), msg_type);
    ASSERT_EQ(parsed_cmdu.GetId(), msg_id);
    ASSERT_EQ(parsed_cmdu.GetFlags(), msg_flags);

    // --- 5. Kiểm tra các TLV đã được Parse ---
    const auto& parsed_tlvs = parsed_cmdu.GetTlvs();
    ASSERT_EQ(parsed_tlvs.size(), 3); // TLV1 + TLV2 + END_OF_MESSAGE

    // Kiểm tra TLV 1
    const Tlv* tlv1 = parsed_cmdu.FindTlv(TlvTypes::DEVICE_INFO);
    ASSERT_NE(tlv1, nullptr);
    ASSERT_EQ(tlv1->GetLength(), 6);
    
    // Kiểm tra TLV 2
    const Tlv* tlv2 = parsed_cmdu.FindTlv(TlvTypes::LINK_METRIC);
    ASSERT_NE(tlv2, nullptr);
    ASSERT_EQ(tlv2->GetLength(), 1);
    ASSERT_EQ(tlv2->GetValue()[0], 0x01);

    // Kiểm tra TLV cuối cùng (END_OF_MESSAGE)
    ASSERT_EQ(parsed_tlvs.back().GetType(), TlvTypes::END_OF_MESSAGE);
    ASSERT_EQ(parsed_tlvs.back().GetLength(), 0);
}

/**
 * @brief Kiểm tra ngoại lệ khi parser nhận buffer quá ngắn.
 */
TEST(TlvTest, ParserIncompleteHeader) {
    // Buffer chỉ có 5 byte, nhỏ hơn CMDU_HEADER_SIZE (6)
    std::vector<uint8_t> short_buffer = {0x00, 0x01, 0xAA, 0xBB, 0xCC}; 

    ASSERT_THROW({
        MessageParser::Parse(short_buffer);
    }, std::runtime_error);
}

/**
 * @brief Kiểm tra ngoại lệ khi dữ liệu TLV bị cắt ngắn.
 */
TEST(TlvTest, ParserIncompleteTlvValue) {
    // Tạo buffer với Header hợp lệ, nhưng TLV Length sai.
    // Giả lập gói tin Link Metric Request (0x8001), ID=1, Flags=0.
    // Thêm TLV 1: Type=0x0004 (Device Info), Length=10
    // Nhưng chỉ cung cấp 5 byte data.
    std::vector<uint8_t> bad_tlv_buffer = {
        0x80, 0x01, // Type
        0x00, 0x01, // ID
        0x00, 0x00, // Flags + Reserved
        
        0x00, 0x04, // TLV Type: DEVICE_INFO
        0x00, 0x0A, // TLV Length: 10 bytes (Thiếu 5 byte)
        
        0x01, 0x02, 0x03, 0x04, 0x05 // Chỉ có 5 byte data
    }; 

    ASSERT_THROW({
        MessageParser::Parse(bad_tlv_buffer);
    }, std::runtime_error);
}