#include <iostream>
#include <string>
#include <sstream>
#include "../headerFiles/portScanner.h"

int main(int argc, char *argv[])
{
    // Check if exactly two arguments and the program name were provided
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <argument1> <argument2>" << std::endl;
        return 1; // Return an error code
    }

    // Set the arguments as variables
    std::string ipAddress = argv[1]; // 192.168.1.1
    std::string portRange = argv[2]; // 1-1024

    // get the two numbers from range
    std::stringstream ss(portRange);

    std::string t;
    // Delimiter
    char del = '-';

    int startingPort, endingPort;

    // Get the first number
    if (getline(ss, t, del))
    {
        startingPort = std::stoi(t); // convert string to int
    }

    // Get the second number
    if (getline(ss, t, del))
    {
        endingPort = std::stoi(t); // convert string to int
    }

    std::cout << "Ip Address: : " << ipAddress << "\n";
    std::cout << "Starting Port: " << startingPort << "\n";
    std::cout << "Ending Port: " << endingPort << "\n";

    portScanner scanner; // headerfile stuff

    return 0;
}