# 🔍 Network Scanner

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Linux-green.svg)](https://www.linux.org/)
[![Build System](https://img.shields.io/badge/Build%20System-CMake-red.svg)](https://cmake.org/)

A powerful network scanning tool written in C++ for educational purposes. This tool demonstrates basic network scanning techniques similar to nmap. 🚀

## ✨ Features
- 🔎 TCP port scanning
- 🔍 Service detection
- 🌐 Multiple IP address scanning support
- ⚙️ Configurable port ranges

## 🛠️ Building
```bash
mkdir build
cd build
cmake ..
make
```

## 🚀 Usage
```bash
./network_scanner <target_ip> [start_port] [end_port]
```

### Example:
```bash
./network_scanner 192.168.1.1 1 1000
```

## ⚠️ Note
This tool is created for educational purposes only. Always ensure you have permission to scan networks and systems.

## 📋 Dependencies
- C++17 or higher
- CMake 3.10 or higher
- POSIX-compliant system

## 📜 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
