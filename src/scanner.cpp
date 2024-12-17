#include "scanner.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <thread>
#include <algorithm>
#include <cstring>

NetworkScanner::NetworkScanner(const std::string& ip, int start, int end)
    : target_ip(ip), start_port(start), end_port(end) {
    if (!is_valid_ip(ip)) {
        throw std::runtime_error("Invalid IP address");
    }
    if (start_port > end_port || start_port < 1 || end_port > 65535) {
        throw std::runtime_error("Invalid port range");
    }
}

std::vector<NetworkScanner::ScanResult> NetworkScanner::scan() {
    std::vector<ScanResult> results;
    std::vector<std::future<ScanResult>> futures;

    // Create a thread pool
    for (int port = start_port; port <= end_port; ++port) {
        if (futures.size() >= thread_count) {
            // Wait for some threads to complete
            auto it = std::find_if(futures.begin(), futures.end(),
                [](const std::future<ScanResult>& f) {
                    return f.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
                });
            
            if (it != futures.end()) {
                results.push_back(it->get());
                futures.erase(it);
            }
        }
        
        // Launch new scan
        futures.push_back(std::async(std::launch::async,
            &NetworkScanner::scan_port, this, target_ip, port));
    }

    // Wait for remaining threads
    for (auto& f : futures) {
        results.push_back(f.get());
    }

    // Sort results by port number
    std::sort(results.begin(), results.end(),
        [](const ScanResult& a, const ScanResult& b) { return a.port < b.port; });

    return results;
}

NetworkScanner::ScanResult NetworkScanner::scan_port(const std::string& ip, int port) {
    ScanResult result{ip, port, false, "", std::chrono::milliseconds(0)};
    
    std::chrono::milliseconds response_time;
    result.is_open = connect_to_port(ip, port, response_time);
    result.response_time = response_time;
    
    if (result.is_open) {
        result.service_name = get_service_name(port);
    }
    
    return result;
}

bool NetworkScanner::connect_to_port(const std::string& ip, int port, std::chrono::milliseconds& response_time) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    // Set non-blocking
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    auto start = std::chrono::high_resolution_clock::now();
    
    // Attempt connection
    int result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    
    if (result < 0) {
        if (errno == EINPROGRESS) {
            fd_set fdset;
            struct timeval tv;
            
            FD_ZERO(&fdset);
            FD_SET(sock, &fdset);
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;

            result = select(sock + 1, NULL, &fdset, NULL, &tv);
            
            if (result == 1) {
                int so_error;
                socklen_t len = sizeof(so_error);
                getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
                if (so_error == 0) {
                    auto end = std::chrono::high_resolution_clock::now();
                    response_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                    close(sock);
                    return true;
                }
            }
        }
    }

    close(sock);
    return false;
}

bool NetworkScanner::is_valid_ip(const std::string& ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) != 0;
}

std::string NetworkScanner::get_service_name(int port) {
    // Common ports and their services
    switch (port) {
        case 21: return "FTP";
        case 22: return "SSH";
        case 23: return "Telnet";
        case 25: return "SMTP";
        case 53: return "DNS";
        case 80: return "HTTP";
        case 110: return "POP3";
        case 143: return "IMAP";
        case 443: return "HTTPS";
        case 445: return "SMB";
        case 3306: return "MySQL";
        case 3389: return "RDP";
        case 5432: return "PostgreSQL";
        case 8080: return "HTTP-Proxy";
        default: return "Unknown";
    }
}
