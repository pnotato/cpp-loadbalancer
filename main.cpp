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
    hint.sin_port = htons(54000); // host to network short
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        std::cerr <<  "Can't bind to port";
        return -2;
    }

    // Mark the socket for listening in 
    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Can't listen";
        return -3;
    }

    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr*)&client,  &clientSize)

    if (clientSocket == -1)
    {
        std::cerr << "Problem with client connecting";
    }

    close(listening);
    memset(host, 0, NI_MAXHOST);

    // Close the listening socket
    // While recieving -> display message, echo message.
    return 0;
}