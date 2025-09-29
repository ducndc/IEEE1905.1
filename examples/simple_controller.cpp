// examples/simple_controller.cpp
#include "ieee1905_1/agent.h"
#include <iostream>
#include <stdexcept>
#include <string>

using namespace ieee1905_1;

/**
 * @brief Chương trình ví dụ đơn giản minh họa cách sử dụng Ieee1905Agent.
 *
 * Chương trình này sẽ:
 * 1. Khởi tạo Agent 1905.1 trên một giao diện mạng (ví dụ: "eth0").
 * 2. Gửi một thông điệp Topology Discovery.
 * 3. Mô phỏng việc lắng nghe phản hồi (mặc dù vòng lặp Start() chưa được triển khai đầy đủ).
 */
int main(int argc, char* argv[]) {
    
    // --- 1. Thiết lập tham số giao diện ---
    std::string interface_name = "eth0"; 
    if (argc > 1) {
        interface_name = argv[1]; // Cho phép chỉ định giao diện qua đối số dòng lệnh
    }
    
    std::cout << "=================================================" << std::endl;
    std::cout << " IEEE 1905.1 Simple Controller Example" << std::endl;
    std::cout << "=================================================" << std::endl;
    std::cout << "Using network interface: " << interface_name << std::endl;
    
    try {
        // --- 2. Khởi tạo Agent ---
        // Ieee1905Agent sẽ khởi tạo InterfaceManager bên trong
        Ieee1905Agent agent(interface_name);
        
        // --- 3. Thực hiện hành động giao thức ---
        std::cout << "\n[ACTION] Attempting to send Topology Discovery Message..." << std::endl;
        
        // Gửi gói tin khám phá mạng. Logic này bao gồm:
        // - Tạo Cmdu (Topology Discovery)
        // - Thêm các TLV cần thiết
        // - Đóng gói (Serialization)
        // - Gửi qua InterfaceManager (Raw Socket)
        agent.SendTopologyDiscovery();
        
        std::cout << "[ACTION] Topology Discovery Sent. Check network traffic for frame 0x893a." << std::endl;

        // --- 4. Mô phỏng vòng lặp chính (Listening) ---
        std::cout << "\n[AGENT] Agent is now running (or would be running main loop)..." << std::endl;
        // agent.Start(); // Khi triển khai đầy đủ, hàm này sẽ chặn và xử lý I/O

        // Trong môi trường thực, chúng ta sẽ cần chờ phản hồi hoặc thoát
        // Ví dụ: std::this_thread::sleep_for(std::chrono::seconds(5)); 

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