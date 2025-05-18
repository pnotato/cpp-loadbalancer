#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

// Basic TCP Server First

int main() {
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET specifies the IPv4 address family
    if (listening == -1) 
    {
        std::cerr << "Can't create a socket!";
        return -1;
    }

    // Bind the socket to IP / Port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8080); // host to network short
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        std::cerr <<  "Can't bind to port";
        return -2;
    }

    // Mark the socket for listening in 
    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Can't listen too many connections";
        return -3;
    }

    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr*)&client,  &clientSize);

    if (clientSocket == -1)
    {
        std::cerr << "Problem with client connecting";
        return -4;
    }


    // Close the listening socket
    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV); // clears the memory

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if (result)
    {
        std::cout << host << " connected on " << svc << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
    }

    // While recieving -> display message, echo message.
    char buf[4096];
    while (true) 
    {
        // Clear the buffer
        memset(buf, 0, 4096);

        // Wait for a message
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1) {
            std::cerr << "There was a connection issue" << std::endl;
        }
        if (bytesRecv == 0) {
            std::cout << "The client disconnected" << std::endl;
            break;
        }
       
        // Display message
        std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;


       std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 7\r\n"
        "\r\n"
        "Hello!\n";
        
        // Resend message
        send(clientSocket, response.c_str(), response.size(), 0);
    }
    
    return 0;
}