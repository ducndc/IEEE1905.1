/**
 * simple_controller.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/agent.h"
#include "ieee1905_1/logger.h"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace ieee1905_1;

/**
* @brief Simple example program demonstrating the use of Ieee1905Agent.
*
* This program will:
* 1. Initialize Agent 1905.1 on a network interface (e.g., "eth0").
* 2. Send a Topology Discovery message.
* 3. Simulate listening for a response (although the Start() loop is not fully implemented).
*/
int main(int argc, char* argv[]) 
{
    
    // --- 1. Set interface parameters ---
    std::string interface_name = "eth0"; 

    if (argc > 1) {
        interface_name = argv[1]; // Allows specifying interface via command line arguments
    }
    
    EM_DEBUG("=================================================");
    //std::cout << "=================================================" << std::endl;
    EM_DEBUG(" IEEE 1905.1 Simple Controller Example");
    EM_DEBUG("=================================================");
    EM_DEBUG("Using network interface: {}", interface_name);
    
    try {
        // --- 2. Initialize Agent ---
        // Ieee1905Agent will initialize InterfaceManager internally
        Ieee1905Agent agent(interface_name);
        
        // --- 3. Perform protocol action ---
        EM_DEBUG("[ACTION] Attempting to send Topology Discovery Message...");
        
        // Send network discovery packet. This logic includes:
        // - Create Cmdu (Topology Discovery)
        // - Add necessary TLVs
        // - Encapsulate (Serialization)
        // - Send via InterfaceManager (Raw Socket)
        agent.SendTopologyDiscovery();
        
        EM_DEBUG("[ACTION] Topology Discovery Sent. Check network traffic for frame 0x893a.");

        // --- 4. Simulate the main loop (Listening) ---
        EM_DEBUG("[AGENT] Agent is now running (or would be running main loop)...");
        // agent.Start(); // When fully implemented, this function will block and handle I/O

        // In real world, we will need to wait for response or exit
        // Example: std::this_thread::sleep_for(std::chrono::seconds(5));

    } catch (const std::exception& e) {
        EM_ERROR("[ERROR] Fatal error during execution: {}", e.what());
        return 1;
    } catch (...) {
        EM_ERROR("[ERROR] An unknown error occurred.");
        return 1;
    }

    EM_DEBUG("=================================================");
    EM_DEBUG("Example finished successfully (simulated).");

    return 0;
}