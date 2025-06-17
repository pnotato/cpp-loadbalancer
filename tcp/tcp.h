#pragma once

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <thread>

class TCPServer {
    public:
        // Constructor
        TCPServer(
            int port = 8000
        );
        // Initializing
        bool init();

        // Start Listening
        void start();

        // Stop Listening
        void stop();

    private:
        int port;
        int server_socket;
        bool is_running;
        sockaddr_in server_addr;

        void handle_client(int client_socket);
        int create_socket();
   
}