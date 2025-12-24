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

const char* CmduTypeToString(uint16_t type);

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

    /* =========================================================
     * 1905.1 – TOPOLOGY
     * ========================================================= */
    void ProcessTopologyDiscovery(const Cmdu& cmdu);
    void ProcessTopologyNotification(const Cmdu& cmdu);
    void ProcessTopologyQuery(const Cmdu& cmdu);
    void ProcessTopologyResponse(const Cmdu& cmdu);

    /* =========================================================
     * 1905.1 – LINK METRICS
     * ========================================================= */
    void ProcessLinkMetricQuery(const Cmdu& cmdu);
    void ProcessLinkMetricResponse(const Cmdu& cmdu);

    /* =========================================================
     * 1905.1 – AP AUTOCONFIGURATION
     * ========================================================= */
    void ProcessAutoconfigurationSearch(const Cmdu& cmdu);
    void ProcessAutoconfigurationResponse(const Cmdu& cmdu);
    void ProcessAutoconfigurationWsc(const Cmdu& cmdu);
    void ProcessAutoconfigurationRenew(const Cmdu& cmdu);

    /* =========================================================
     * 1905.1 – GENERIC / MISC
     * ========================================================= */
    void ProcessVendorSpecific(const Cmdu& cmdu);
    void ProcessHigherLayerQuery(const Cmdu& cmdu);
    void ProcessHigherLayerResponse(const Cmdu& cmdu);
    void ProcessGenericPhyQuery(const Cmdu& cmdu);
    void ProcessGenericPhyResponse(const Cmdu& cmdu);

    /* =========================================================
     * EasyMesh – AP / RADIO CAPABILITY
     * ========================================================= */
    void ProcessApCapabilityQuery(const Cmdu& cmdu);
    void ProcessApCapabilityReport(const Cmdu& cmdu);

    /* =========================================================
     * EasyMesh – CHANNEL MANAGEMENT
     * ========================================================= */
    void ProcessChannelPreferenceQuery(const Cmdu& cmdu);
    void ProcessChannelPreferenceReport(const Cmdu& cmdu);
    void ProcessChannelSelectionRequest(const Cmdu& cmdu);
    void ProcessChannelSelectionResponse(const Cmdu& cmdu);
    void ProcessOperatingChannelReport(const Cmdu& cmdu);

    /* =========================================================
     * EasyMesh – CLIENT CAPABILITY & METRICS
     * ========================================================= */
    void ProcessClientCapabilityQuery(const Cmdu& cmdu);
    void ProcessClientCapabilityReport(const Cmdu& cmdu);

    void ProcessAssocStaLinkMetricsQuery(const Cmdu& cmdu);
    void ProcessAssocStaLinkMetricsResponse(const Cmdu& cmdu);

    void ProcessUnassocStaLinkMetricsQuery(const Cmdu& cmdu);
    void ProcessUnassocStaLinkMetricsResponse(const Cmdu& cmdu);

    /* =========================================================
     * EasyMesh – CLIENT STEERING / ROAMING
     * ========================================================= */
    void ProcessClientSteeringRequest(const Cmdu& cmdu);
    void ProcessClientSteeringBtmRequest(const Cmdu& cmdu);
    void ProcessClientAssocControlRequest(const Cmdu& cmdu);
    void ProcessClientSteeringComplete(const Cmdu& cmdu);

    /* =========================================================
     * EasyMesh – BACKHAUL
     * ========================================================= */
    void ProcessBackhaulSteeringRequest(const Cmdu& cmdu);
    void ProcessBackhaulSteeringResponse(const Cmdu& cmdu);

    /* =========================================================
     * EasyMesh – CHANNEL SCAN
     * ========================================================= */
    void ProcessChannelScanRequest(const Cmdu& cmdu);
    void ProcessChannelScanResponse(const Cmdu& cmdu);

    /* =========================================================
     * EasyMesh – Wi-Fi 6 / 7 / MLD
     * ========================================================= */
    void ProcessApMldConfigurationRequest(const Cmdu& cmdu);
    void ProcessApMldConfigurationResponse(const Cmdu& cmdu);

    void ProcessBstaMldConfigurationRequest(const Cmdu& cmdu);
    void ProcessBstaMldConfigurationResponse(const Cmdu& cmdu);
};

} // namespace ieee1905_1

#endif // IEEE1905_1_AGENT_H