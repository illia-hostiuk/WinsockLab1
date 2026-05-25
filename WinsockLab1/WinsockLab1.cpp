#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void printError(const char* msg) {
    cout << msg << " Error code: " << WSAGetLastError() << endl;
}

int main() {

    WSADATA wsaData;

    // WINSOCK INITIALIZATION

    int initResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (initResult != 0) {
        cout << "WSAStartup failed!" << endl;
        return 1;
    }

    cout << "WinSock initialized successfully\n\n";

    cout << "Description: " << wsaData.szDescription << endl;
    cout << "System status: " << wsaData.szSystemStatus << endl;
    cout << "Max sockets: " << wsaData.iMaxSockets << endl;
    cout << "Max UDP datagram size: " << wsaData.iMaxUdpDg << endl;

    cout << "\n====================================\n";

    // CREATE SOCKETS

    SOCKET sock1, sock2, sock3, sock4;

    // UDP sockets
    sock1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // TCP sockets
    sock3 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sock4 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock1 == INVALID_SOCKET ||
        sock2 == INVALID_SOCKET ||
        sock3 == INVALID_SOCKET ||
        sock4 == INVALID_SOCKET) {

        printError("Socket creation failed!");
        WSACleanup();
        return 1;
    }

    cout << "4 sockets created successfully\n";

    // SOCKET ADDRESSES

    sockaddr_in addr1, addr2, addr3, addr4;

    // 1. Broadcast UDP socket
    addr1.sin_family = AF_INET;
    addr1.sin_port = htons(644);
    addr1.sin_addr.s_addr = INADDR_ANY;

    // 2. Fixed IP socket
    addr2.sin_family = AF_INET;
    addr2.sin_port = htons(645);
    addr2.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Fixed IP socket
    addr3.sin_family = AF_INET;
    addr3.sin_port = htons(646);
    addr3.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 4. Any local address
    addr4.sin_family = AF_INET;
    addr4.sin_port = htons(647);
    addr4.sin_addr.s_addr = INADDR_ANY;

    // ENABLE BROADCAST MODE

    BOOL broadcastEnable = TRUE;

    setsockopt(
        sock1,
        SOL_SOCKET,
        SO_BROADCAST,
        (char*)&broadcastEnable,
        sizeof(broadcastEnable)
    );

    // SET UDP DATAGRAM SIZE = 644

    int udpSize = 644;

    setsockopt(
        sock1,
        SOL_SOCKET,
        SO_SNDBUF,
        (char*)&udpSize,
        sizeof(udpSize)
    );

    setsockopt(
        sock2,
        SOL_SOCKET,
        SO_SNDBUF,
        (char*)&udpSize,
        sizeof(udpSize)
    );

    // BIND SOCKETS

    if (bind(sock1, (sockaddr*)&addr1, sizeof(addr1)) == SOCKET_ERROR)
        printError("sock1 bind failed");
    else
        cout << "sock1 bind success\n";

    if (bind(sock2, (sockaddr*)&addr2, sizeof(addr2)) == SOCKET_ERROR)
        printError("sock2 bind failed");
    else
        cout << "sock2 bind success\n";

    if (bind(sock3, (sockaddr*)&addr3, sizeof(addr3)) == SOCKET_ERROR)
        printError("sock3 bind failed");
    else
        cout << "sock3 bind success\n";

    if (bind(sock4, (sockaddr*)&addr4, sizeof(addr4)) == SOCKET_ERROR)
        printError("sock4 bind failed");
    else
        cout << "sock4 bind success\n";

    cout << "\n====================================\n";

    // ERROR SIMULATION

    // WSAEINVAL
    if (bind(sock4, (sockaddr*)&addr4, sizeof(addr4)) == SOCKET_ERROR)
        printError("WSAEINVAL simulated");

    // WSAENOTSOCK
    if (bind(9999, (sockaddr*)&addr4, sizeof(addr4)) == SOCKET_ERROR)
        printError("WSAENOTSOCK simulated");

    // WSAEFAULT
    if (bind(sock1, (sockaddr*)0x1234, sizeof(addr1)) == SOCKET_ERROR)
        printError("WSAEFAULT simulated");

    // WSAEADDRINUSE
    SOCKET tempSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (bind(tempSock, (sockaddr*)&addr4, sizeof(addr4)) == SOCKET_ERROR)
        printError("WSAEADDRINUSE simulated");

    closesocket(tempSock);

    // WSAEAFNOSUPPORT
    SOCKET wrongSocket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (wrongSocket == INVALID_SOCKET)
        printError("WSAEAFNOSUPPORT simulated");


    closesocket(sock1);
    closesocket(sock2);
    closesocket(sock3);
    closesocket(sock4);

    WSACleanup();

    cout << "\nSockets closed successfully\n";

    return 0;
}