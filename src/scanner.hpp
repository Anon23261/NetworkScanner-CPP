#pragma once

#include <string>
#include <vector>
#include <future>
#include <chrono>

class NetworkScanner {
public:
    struct ScanResult {
        std::string ip;
        int port;
        bool is_open;
        std::string service_name;
        std::chrono::milliseconds response_time;
    };

    NetworkScanner(const std::string& target_ip, int start_port = 1, int end_port = 1024);
    
    // Main scanning functions
    std::vector<ScanResult> scan();
    ScanResult scan_port(const std::string& ip, int port);
    
    // Setters for configuration
    void set_timeout(int milliseconds) { timeout_ms = milliseconds; }
    void set_threads(int num) { thread_count = num; }

private:
    std::string target_ip;
    int start_port;
    int end_port;
    int timeout_ms = 1000;  // Default 1 second timeout
    int thread_count = 50;  // Default number of threads

    // Helper functions
    bool is_valid_ip(const std::string& ip);
    std::string get_service_name(int port);
    bool connect_to_port(const std::string& ip, int port, std::chrono::milliseconds& response_time);
};
