#include "../headerFiles/portScanner.h"

// #pragma comment(lib, "ws2_32.lib") // Link Winsock library
//  https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect

void portScanner::createSocket(int port, const std::string &ipAddress, int result)
{
    // we will add to the txt
    std::wofstream appendFile("example.txt", std::ios_base::app);

    // Create a SOCKET for connecting to server
    SOCKET ConnectSocket;
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET)
    {
        if (!appendFile.is_open())
        {
            wprintf(L"Failed to open file\n");
        }
        else
        {
            // print error to txt
            std::wstringstream ss;
            ss << L"socket function failed with error: " << WSAGetLastError() << L"\n";
            appendFile << ss.str();
        }

        WSACleanup();
        result = 1;
        return;
    }

    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &target.sin_addr);

    // Connect to server.
    result = connect(ConnectSocket, (sockaddr *)&target, sizeof(target));
    wprintf(L"Connected to server.\n");

    if (result == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        std::cout << "Port " << port << " CLOSED (Error: " << error << ")\n";
    }
    else
    {
        std::cout << "Port " << port << " OPEN\n";
    }

    result = closesocket(ConnectSocket);
    if (result == SOCKET_ERROR)
    {
        if (!appendFile.is_open())
        {
            wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        }
        else
        {
            // print error to txt
            std::wstringstream ss;
            ss << L"closesocket function failed with error: %ld\n"
               << WSAGetLastError() << L"\n";
            appendFile << ss.str();
        }

        appendFile.close();
        WSACleanup();
        result = 1;
    }
}

void portScanner::getTime(char *buffer, size_t bufferSize)
{
    // 1. Get the current time as a time_t object
    std::time_t now = std::time(0);

    // 2. Convert to a local time structure (struct tm)
    std::tm *local_time = std::localtime(&now); // Note: Use localtime_s on Windows for safety

    // Format as "YYYY-MM-DD Hour-Minute-Second"
    std::strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", local_time);
}

void portScanner::createTXT(const char *buffer, const std::string &ipAddress)
{
    // txt creation
    std::ofstream outputFile;
    outputFile.open("example.txt");

    // Did it open?
    if (!outputFile.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    outputFile << "Ports opened or closed" << std::endl;
    outputFile << "Ip Address" << ipAddress << std::endl;
    outputFile << "Time Scanned: " << buffer << std::endl;
    outputFile << "" << std::endl;

    outputFile.close(); // close file
}

int main()
{
    portScanner scanner; // headerfile stuff

    // Set an ip - main.cpp will handle this later
    std::string const ipAddress = "192.168.0.61";

    // Get time
    char dateBuffer[80]; // buffer to hold the formatted date
    scanner.getTime(dateBuffer, sizeof(dateBuffer));

    // Create txt
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
    for (int port = 1; port <= 1024; port++)
    {
        scanner.createSocket(port, ipAddress, result); // create connect
    }

    std::cout << "Text has been written to example.txt" << std::endl;
    WSACleanup();
    return 0;
}