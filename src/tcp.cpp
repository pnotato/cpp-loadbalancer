#include "tcp.h"
#include "loadbalancer.h"
#include <thread>
#define BUF_SIZE 4096

TCPServer::TCPServer(int port) 
{
    this->port = port;
    this->is_running = false;
    this->server_socket = create_socket();
    if (this->server_socket == -1) {
        throw std::runtime_error("Socket creation failed.");
    }
    if (listen(this->server_socket, SOMAXCONN) == -1) {
        std::cerr << "ERROR: Failed to listen on socket." << std::endl;
        close(this->server_socket);
        throw std::runtime_error("Socket listen failed.");
    }
    std::cout << "Server is listening on port " << this->port << std::endl;
}

void TCPServer::start(LoadBalancer &lb) {
    this->is_running = true;
    std::cout << "Starting server..." << std::endl;

    while (is_running) {
        sockaddr_in client;
        socklen_t client_size = sizeof(client);

        int client_socket = accept(this->server_socket, (sockaddr*)&client,  &client_size);
        if (client_socket == -1)
        {
            std::cerr << "ERROR: Failed to accept client connection" << std::endl;
            continue; // Maybe change this to a break later? 
        }
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr, client_ip, client_size);
        std::cout << "Accepting connection from IP: " << client_ip << ". Forwarding to " << lb.get_current_server() << "!" << std::endl; 

        // std::thread t(&TCPServer::handle_client, this, client_socket);
        std::thread t(&LoadBalancer::handle_connection, &lb, client_socket);
        t.detach();
    }
}

void TCPServer::stop() {
    this->is_running = false;
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

void TCPServer::handle_client(int client_socket) {
    char buf[BUF_SIZE]; // Maybe change the size of this later. buf stays as a pointer since it's a char array.
    while(true){
        memset(buf, 0, BUF_SIZE);
        int bytes_recieved = recv(client_socket, &buf, BUF_SIZE, 0);
        if (bytes_recieved == -1) {
            std::cerr << "ERROR: Failed to recieve data" << std::endl;
        }
        if (bytes_recieved == 0) {
            std::cout << "Client disconnected" << std::endl;
            break; // Since this stops the loop, the thread DIES
        }

        std::cout << "Recieved Message: " << std::string(buf, 0, bytes_recieved) << std::endl;

        // This is only so clients don't have a stroke that the data isnt http or smt stupid
        std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 7\r\n"
        "\r\n"
        "Message Recieved!!\n";

        send(client_socket, response.c_str(), response.size(), 0);
    }
}