/**
 * Simple echo winsock application
 * Information and comments on the server: http://thisthread.blogspot.com/2012/05/hello-windows-sockets-2-server.html
 *
 * Loosely based on http://msdn.microsoft.com/en-us/library/windows/desktop/ms738545(v=vs.85).aspx
 */

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <algorithm>

// #pragma comment(lib, "Ws2_32.lib") // or: Properties - Linker - Input - Additional Dependecies: Ws2_32.lib

namespace wsa
{
    void set(ADDRINFO& that, int flags =0, int family =0, int type =0, int protocol =0,
        size_t addrlen =0, char* name =0, SOCKADDR* addr =0, ADDRINFO* next =0)
    {
        that.ai_flags = flags;
        that.ai_family = family;
        that.ai_socktype = type;
        that.ai_protocol = protocol;
        that.ai_addrlen = addrlen;
        that.ai_canonname = name;
        that.ai_addr = addr;
        that.ai_next = next;
    }

    void list(ADDRINFO* ai)
    {
        std::cout << "list available socket address info:" << std::endl;
        while(ai)
        {
            std::cout << ai->ai_family << ' ' << ai->ai_socktype << ' ' << ai->ai_protocol << std::endl;
            ai = ai->ai_next;
        }
        std::cout << "---" << std::endl;
    }

    ADDRINFO* get(const char* host, const char* port, ADDRINFO* hints)
    {
        std::cout << "get socket address information" << std::endl;
        ADDRINFO* ai = NULL;
        int failCode = getaddrinfo(host, port, hints, &ai);
        if(failCode)
        {
            std::cout << "getaddrinfo failed: " << failCode << std::endl;
            return NULL;
        }
        return ai;
    }

    SOCKET createSocket(ADDRINFO* ai)
    {
        std::cout << "create a socket" << std::endl;

        SOCKET sk = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if(sk == INVALID_SOCKET)
        {
            std::cout << "error creating socket: " << WSAGetLastError() << std::endl;
            return INVALID_SOCKET;
        }
        return sk;
    }

    SOCKET createClientSocket(ADDRINFO* ai)
    {
        SOCKET sk = createSocket(ai);

        std::cout << "connect to server" << std::endl;
        if(sk != INVALID_SOCKET && connect(sk, ai->ai_addr, ai->ai_addrlen) == SOCKET_ERROR)
        {
            closesocket(sk);

            std::cout << "can't connect to server." << std::endl;
            return INVALID_SOCKET;
        }
        return sk;
    }

    SOCKET createServerSocket(ADDRINFO* ai)
    {
        SOCKET sk = createSocket(ai);

        std::cout << "bind socket" << std::endl;
        if(sk != INVALID_SOCKET && bind(sk, ai->ai_addr, ai->ai_addrlen) == SOCKET_ERROR)
        {
            closesocket(sk);

            std::cout << "Unable to bind: " << WSAGetLastError() << std::endl;
            return INVALID_SOCKET;
        }
        return sk;
    }
}

namespace
{
    const int BUFLEN = 512;

    void coreClient(SOCKET sk)
    {
        char* message = "Hello Winsock 2!";

        std::cout << "sending message: '" << message << '\'' << std::endl;
        int size = send(sk, message, strlen(message), 0);
        if(size == SOCKET_ERROR)
        {
            std::cout << "send failed: " << WSAGetLastError() << std::endl;
            return;
        }

        std::cout << size << " bytes sent" << std::endl;

        std::cout << "socket send component shutdown" << std::endl;
        if(shutdown(sk, SD_SEND) == SOCKET_ERROR)
        {
            std::cout << "shutdown failed: " << WSAGetLastError() << std::endl;
            return;
        }

        // Receive data until the server closes the connection
        do {
            char buffer[BUFLEN];
            size = recv(sk, buffer, BUFLEN, 0);
            if(size > 0)
            {
                std::cout << "Buffer received: '";
                std::for_each(buffer, buffer + size, [](char c){ std::cout << c; });
                std::cout << '\'' << std::endl;
            }
            else if(!size)
                std::cout << "Connection closed." << std::endl;
            else
                std::cout << "recv failed: " << WSAGetLastError() << std::endl;
        } while(size > 0);
    }

    void echoLoop(SOCKET skClient)
    {
        char buffer[BUFLEN];

        int rSize;
        do {
            rSize = recv(skClient, buffer, BUFLEN, 0);
            if(rSize > 0)
            {
                std::cout << "Buffer received: '";
                std::for_each(buffer, buffer + rSize, [](char c){ std::cout << c; });
                std::cout << '\'' << std::endl;

                // Echo the buffer back to the sender
                int sSize = send(skClient, buffer, rSize, 0);
                if(sSize == SOCKET_ERROR)
                {
                    std::cout << "send failed: " << WSAGetLastError() << std::endl;
                    return;
                }
                std::cout << sSize << " bytes sent back" << std::endl;
            }
            else if(rSize == 0)
                std::cout << "closing connection ..." << std::endl;
            else
            {
                std::cout << "Error receiving: " << WSAGetLastError() << std::endl;
                return;
            }
        } while(rSize > 0);
    }

    void coreServer(SOCKET skServer)
    {
        if(listen(skServer, SOMAXCONN) == SOCKET_ERROR)
        {
            std::cout << "Listen failed with error: " << WSAGetLastError() << std::endl;
            return;
        }

        // Accept a client socket
        SOCKET skClient = accept(skServer, NULL, NULL);
        if(skClient == INVALID_SOCKET)
        {
            std::cout << "accept failed: " << WSAGetLastError() << std::endl;
            return;
        }

        echoLoop(skClient);
        closesocket(skClient);
    }

    void client(const char* host, const char* port)
    {
        std::cout << "Client socket" << std::endl;

        std::cout << "Specifying the socket characteristic" << std::endl;
        ADDRINFO hints;
        wsa::set(hints, 0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP); // both TCP IP v4/v6 are accepted

        ADDRINFO* ai = wsa::get(host, port, &hints);
        if(ai)
        {
            wsa::list(ai);

            SOCKET sk = wsa::createClientSocket(ai);
            freeaddrinfo(ai);

            if(sk != INVALID_SOCKET)
            {
                coreClient(sk);
                closesocket(sk);
            }
        }
    }

    void server(const char* port)
    {
        std::cout << "Server socket" << std::endl;

        std::cout << "Specifing the socket characteristic" << std::endl;
        ADDRINFO hints;
        wsa::set(hints, AI_PASSIVE, AF_INET, SOCK_STREAM, IPPROTO_TCP); // server (passive) TCP v4

        ADDRINFO* ai = wsa::get(NULL, port, &hints);
        if(ai)
        {
            SOCKET sk = wsa::createServerSocket(ai);
            freeaddrinfo(ai);

            if(sk != INVALID_SOCKET)
            {
                coreServer(sk);
                closesocket(sk);
            }
        }
    }
}

void simple(bool isServer, const char* host, const char* port)
{
    std::cout << "Initializing Winsock" << std::endl;
    WSADATA wsaData;
    int failCode = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(failCode)
    {
        std::cout << "WSAStartup failed: " << failCode << std::endl;
        return;
    }

    if(isServer)
        server(port);
    else
        client(host, port);

    WSACleanup();
}
