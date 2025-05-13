#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

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
    hint.sin_port = 54000;

    // Mark the socket for listening in 
    // Accept a call
    // Close the listening socket
    // While recieving -> display message, echo message.
    return 0;
}