/**
 * simple_controller.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/agent.h"
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
int main(int argc, char* argv[]) {
    
    // --- 1. Set interface parameters ---
    std::string interface_name = "eth0"; 
    if (argc > 1) {
        interface_name = argv[1]; // Allows specifying interface via command line arguments
    }
    
    std::cout << "=================================================" << std::endl;
    std::cout << " IEEE 1905.1 Simple Controller Example" << std::endl;
    std::cout << "=================================================" << std::endl;
    std::cout << "Using network interface: " << interface_name << std::endl;
    
    try {
        // --- 2. Initialize Agent ---
        // Ieee1905Agent will initialize InterfaceManager internally
        Ieee1905Agent agent(interface_name);
        
        // --- 3. Perform protocol action ---
        std::cout << "\n[ACTION] Attempting to send Topology Discovery Message..." << std::endl;
        
        // Send network discovery packet. This logic includes:
        // - Create Cmdu (Topology Discovery)
        // - Add necessary TLVs
        // - Encapsulate (Serialization)
        // - Send via InterfaceManager (Raw Socket)
        agent.SendTopologyDiscovery();
        
        std::cout << "[ACTION] Topology Discovery Sent. Check network traffic for frame 0x893a." << std::endl;

        // --- 4. Simulate the main loop (Listening) ---
        std::cout << "\n[AGENT] Agent is now running (or would be running main loop)..." << std::endl;
        // agent.Start(); // When fully implemented, this function will block and handle I/O

        // In real world, we will need to wait for response or exit
        // Example: std::this_thread::sleep_for(std::chrono::seconds(5));

    } catch (const std::exception& e) {
        std::cerr << "\n[ERROR] Fatal error during execution: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n[ERROR] An unknown error occurred." << std::endl;
        return 1;
    }

    std::cout << "\n=================================================" << std::endl;
    std::cout << "Example finished successfully (simulated)." << std::endl;
    return 0;
}