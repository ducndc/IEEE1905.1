// include/ieee1905_1/agent.h
#ifndef IEEE1905_1_AGENT_H
#define IEEE1905_1_AGENT_H

#include "cmdu.h"
#include "interface_manager.h"
#include "serializer.h"
#include "parser.h"
#include <string>

namespace ieee1905_1 {

/**
 * @brief Lớp lõi thực thi logic của Agent IEEE 1905.1.
 * * Lớp này xử lý các sự kiện mạng, gửi và nhận các thông điệp CMDU.
 */
class Ieee1905Agent {
public:
    Ieee1905Agent(const std::string& interface_name);

    /**
     * @brief Bắt đầu vòng lặp xử lý chính (listening và responding).
     */
    void Start();

    /**
     * @brief Xử lý một CMDU nhận được và quyết định phản hồi.
     * @param cmdu Thông điệp nhận được.
     */
    void HandleCmdu(const Cmdu& cmdu);

    /**
     * @brief Gửi thông điệp Topology Discovery để khám phá mạng.
     */
    void SendTopologyDiscovery();
    
private:
    InterfaceManager iface_manager_;
    MessageId next_message_id_ = 1;

    // --- Logic Xử lý CMDU ---
    void ProcessTopologyDiscovery(const Cmdu& request);
    // ... Thêm các hàm process khác
};

} // namespace ieee1905_1

#endif // IEEE1905_1_AGENT_H