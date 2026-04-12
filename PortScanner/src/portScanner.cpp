#include "../headerFiles/portScanner.h"

// #pragma comment(lib, "ws2_32.lib") // Link Winsock library
//  https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect

void portScanner::createSocket(int port, const std::string &ipAddress, int result)
{
    // Open file in append mode
    std::wofstream appendFile("example.txt", std::ios_base::app);

    // check if file opened successfully
    if (!appendFile.is_open())
    {
        wprintf(L"Failed to open log file\n");
    }

    // Create a SOCKET for connecting to server
    SOCKET ConnectSocket;
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (ConnectSocket == INVALID_SOCKET)
    {
        // log error if file is available
        if (appendFile.is_open())
        {
            std::wstringstream ss;
            ss << L"[ERROR] socket() failed with error: " << WSAGetLastError() << L"\n";
            appendFile << ss.str();
            appendFile.flush(); // ensure it is written immediately
        }

        WSACleanup();
        result = 1;
        return; // exit early
    }

    // Setup target address
    sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &target.sin_addr);

    // Attempt connection
    result = connect(ConnectSocket, (sockaddr *)&target, sizeof(target));

    // log outcomes
    if (result == SOCKET_ERROR)
    {
        int error = WSAGetLastError();

        std::cout << "Port " << port << " CLOSED (Error: " << error << ")\n";

        // Log CLOSED port to file
        if (appendFile.is_open())
        {
            appendFile << L"Port " << port << L" CLOSED (Error: " << error << L")\n";
        }
    }
    else
    {
        std::cout << "Port " << port << " OPEN\n";

        // Log OPEN port to file
        if (appendFile.is_open())
        {
            appendFile << L"Port " << port << L" OPEN\n";
        }
    }

    // Close socket and check result
    result = closesocket(ConnectSocket);

    if (result == SOCKET_ERROR)
    {
        int error = WSAGetLastError();

        // Log close error
        if (appendFile.is_open())
        {
            appendFile << L"[ERROR] closesocket failed: " << error << L"\n";
            appendFile.flush();
        }

        wprintf(L"closesocket failed with error: %ld\n", error);

        WSACleanup();
        result = 1;
    }

    // close file at the end of function
    if (appendFile.is_open())
    {
        appendFile.close();
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
    std::string date(buffer, 10); // extract YYYY-MM-DD
    std::string filename = "PortScannerOutput_" + date + ".txt";

    std::ofstream outputFile(filename);

    // Did it open?
    if (!outputFile.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    outputFile << "Ports opened or closed" << std::endl;
    outputFile << "Ip Address: " << ipAddress << std::endl;
    outputFile << "Time Scanned: " << buffer << std::endl;
    outputFile << "" << std::endl;

    outputFile.close(); // close file
}
