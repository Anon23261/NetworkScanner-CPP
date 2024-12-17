#include "scanner.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>

void print_banner() {
    std::cout << "\033[1;36m" // Cyan color
              << R"(
 _   _      _                      _     
| \ | | ___| |___      _____  _ __| | __ 
|  \| |/ _ \ __\ \ /\ / / _ \| '__| |/ / 
| |\  |  __/ |_ \ V  V / (_) | |  |   <  
|_| \_|\___|\__| \_/\_/ \___/|_|  |_|\_\ 
                                         
   ___                                  
  / __| __ __ _ _ _  _ _  ___ _ _      
  \__ \/ _/ _` | ' \| ' \/ -_) '_|     
  |___/\__\__,_|_||_|_||_\___|_|       
)" << "\033[0m" << std::endl;
}

void print_help() {
    std::cout << "Usage: ./network_scanner <target_ip> [start_port] [end_port]\n"
              << "Example: ./network_scanner 192.168.1.1 1 1000\n";
}

void print_results(const std::vector<NetworkScanner::ScanResult>& results) {
    std::cout << "\n\033[1;32m" // Green color
              << "Scan Results:\n"
              << "\033[0m"
              << std::setw(15) << "PORT"
              << std::setw(15) << "STATE"
              << std::setw(20) << "SERVICE"
              << std::setw(15) << "RESPONSE TIME\n"
              << std::string(65, '-') << "\n";

    for (const auto& result : results) {
        if (result.is_open) {
            std::cout << std::setw(15) << result.port
                      << std::setw(15) << "\033[1;32mOPEN\033[0m"
                      << std::setw(20) << result.service_name
                      << std::setw(15) << result.response_time.count() << "ms\n";
        }
    }
}

int main(int argc, char* argv[]) {
    print_banner();

    if (argc < 2 || argc > 4) {
        print_help();
        return 1;
    }

    try {
        std::string target_ip = argv[1];
        int start_port = (argc >= 3) ? std::stoi(argv[2]) : 1;
        int end_port = (argc >= 4) ? std::stoi(argv[3]) : 1024;

        std::cout << "\033[1;34m" // Blue color
                  << "Starting scan on " << target_ip
                  << " (ports " << start_port << "-" << end_port << ")\n"
                  << "\033[0m";

        NetworkScanner scanner(target_ip, start_port, end_port);
        
        // Configure scanner
        scanner.set_timeout(1000);  // 1 second timeout
        scanner.set_threads(50);    // 50 concurrent threads

        auto start_time = std::chrono::high_resolution_clock::now();
        auto results = scanner.scan();
        auto end_time = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

        print_results(results);

        std::cout << "\n\033[1;34m" // Blue color
                  << "Scan completed in " << duration.count() << " seconds\n"
                  << "\033[0m";

    } catch (const std::exception& e) {
        std::cerr << "\033[1;31m" // Red color
                  << "Error: " << e.what() << "\n"
                  << "\033[0m";
        return 1;
    }

    return 0;
}
