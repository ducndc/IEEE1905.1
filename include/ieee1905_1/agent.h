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
    
    void SendTopologyNotification();
    void SendTopologyQuery();
    void SendTopologyResponse();
    void SendVendorSpecific();
    void SendLinkMetricQuery();
    void SendLinkMetricResponse();
    void SendAutoconfigrationSearch();
    void SendAutoconfigrationResponse();
    void SendAutoconfigrationWsc();
    void SendAutoconfigrationRenew();
    void SendPushButtonEventNotification();
    void SendPushButtonJoinNotification();
    void SendHigherLayerQuery();
    void SendHigherLayerResponse();
    void SendInterfacePowerChangeRequest();
    void SendInterfacePowerChangeResponse();
    void SendGenericPhyQuery();
    void SendGenericPhyResponse();

private:
    InterfaceManager m_iface_manager;
    MessageId m_next_message_id = 1;

    // --- CMDU Processing Logic ---
    void ProcessTopologyDiscovery(const Cmdu& request);
    void ProcessTopologyNotification(const Cmdu& request);
    void ProcessTopologyQuery(const Cmdu& request);
    void ProcessTopologyResponse(const Cmdu& request);
    void ProcessVendorSpecific(const Cmdu& request);
    void ProcessLinkMetricQuery(const Cmdu& request);
    void ProcessLinkMetricResponse(const Cmdu& request);
    void ProcessAutoconfigrationSearch(const Cmdu& request);
    void ProcessAutoconfigrationResponse(const Cmdu& request);
    void ProcessAutoconfigrationWsc(const Cmdu& request);
    void ProcessAutoconfigrationRenew(const Cmdu& request);
    void ProcessPushButtonEventNotification(const Cmdu& request);
    void ProcessPushButtonJoinNotification(const Cmdu& request);
    void ProcessHigherLayerQuery(const Cmdu& request);
    void ProcessHigherLayerResponse(const Cmdu& request);
    void ProcessInterfacePowerChangeRequest(const Cmdu& request);
    void ProcessInterfacePowerChangeResponse(const Cmdu& request);
    void ProcessGenericPhyQuery(const Cmdu& request);
    void ProcessGenericPhyResponse(const Cmdu& request);
};

} // namespace ieee1905_1

#endif // IEEE1905_1_AGENT_H