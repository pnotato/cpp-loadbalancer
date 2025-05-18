#include "tcp.h"

TCPServer::TCPServer(int port) 
{
    this->port = port;
    this->server_socket = -1;
    this->is_running = false;
}

bool TCPServer::init()
{

}

int TCPServer::create_socket()
{
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) 
    {
        std::cerr << "ERROR: Couldn't create socket." << std::endl;
        return -1;
    }

    // Binding the socket to IP/port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(this->port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    
    int port_bindings = bind(listening, (sockaddr*)&hint, sizeof(hint));
    if (port_bindings == -1)
    {
        std::cerr << "ERROR: Can't bind to port." << std::endl;
        return -1;
    }
    return listening;
}