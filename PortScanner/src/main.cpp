#include <iostream>
#include <string>
#include <sstream>
#include "../headerFiles/portScanner.h"

int main(int argc, char *argv[])
{
    // headerfile stuff
    portScanner scanner;

    // ARGUEMENTS:
    // Check if exactly two arguments and the program name were provided
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <ip_address> <start-end>\n";
        return 1;
    }

    // Set the arguments as variables
    std::string ipAddress = argv[1]; // 192.168.1.1
    std::string portRange = argv[2]; // 1-1024

    // PORT RANGE
    // Find the dash delimiter
    size_t dashPos = portRange.find('-');

    if (dashPos == std::string::npos)
    {
        std::cerr << "Invalid port range format. Use start-end (e.g., 1-1024)\n";
        return 1;
    }

    // Extract parts
    std::string startStr = portRange.substr(0, dashPos);
    std::string endStr = portRange.substr(dashPos + 1);

    int startingPort, endingPort;

    try
    {
        startingPort = std::stoi(startStr);
        endingPort = std::stoi(endStr);
    }
    catch (...) // change?
    {
        std::cerr << "Port range must contain valid numbers\n";
        return 1;
    }

    // Validate range
    if (startingPort < 1 || endingPort > 65535 || startingPort > endingPort)
    {
        std::cerr << "Invalid port range values\n";
        return 1;
    }

    std::cout << "Ip Address: : " << ipAddress << "\n";
    std::cout << "Starting Port: " << startingPort << "\n";
    std::cout << "Ending Port: " << endingPort << "\n";

    // GET TIME
    char dateBuffer[80]; // buffer to hold the formatted date
    scanner.getTime(dateBuffer, sizeof(dateBuffer));

    // CREATE TXT
    scanner.createTXT(dateBuffer, ipAddress);

    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != NO_ERROR)
    {
        wprintf(L"WSAStartup function failed with error: %d\n", result);
        return 1;
    }

    // loop through all ports
    for (int port = startingPort; port <= endingPort; port++)
    {
        scanner.createSocket(port, ipAddress, result); // create connect
    }

    std::cout << "Text has been written to example.txt" << std::endl;
    WSACleanup();
    return 0;
}