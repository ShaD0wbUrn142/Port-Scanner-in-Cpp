#pragma once

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

class portScanner
{
public:
    portScanner() = default;
    ~portScanner() = default;

    void createSocket(int port, const std::string &ipAddress, int result);
    void getTime(char *buffer, size_t bufferSize);
    void createTXT(const char *buffer, const std::string &ipAddress);
};
