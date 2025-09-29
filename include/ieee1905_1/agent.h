/**
 * agent.h: agent definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef IEEE1905_1_AGENT_H
#define IEEE1905_1_AGENT_H

#include "cmdu.h"
#include "interface_manager.h"
#include "serializer.h"
#include "parser.h"

#include <string>

namespace ieee1905_1 {

/**
 * @brief Class Agent IEEE 1905.1.
 * * Process event and transport CMDU messages.
 */
class Ieee1905Agent {
public:
    Ieee1905Agent(const std::string& interface_name);

    /**
     * @brief Starting loop (listening and responding).
     */
    void Start();

    /**
     * @brief Process a received CMDU and decide on a response.
     * @param cmdu Message received.
     */
    void HandleCmdu(const Cmdu& cmdu);

    /**
     * @brief Send Topology Discovery messages to discover the network.
     */
    void SendTopologyDiscovery();
    
private:
    InterfaceManager iface_manager_;
    MessageId next_message_id_ = 1;

    // --- CMDU Processing Logic ---
    void ProcessTopologyDiscovery(const Cmdu& request);
};

} // namespace ieee1905_1

#endif // IEEE1905_1_AGENT_H