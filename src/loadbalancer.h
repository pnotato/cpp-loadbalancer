#pragma once
#include <vector>
#include <string>
#include <regex>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

class LoadBalancer {
    public:
        LoadBalancer(std::vector<std::string> servers);

        void handle_connection(int client_socket);
        

    private:
        std::vector<std::string> servers; // keeps a list of servers
                                          // maybe write a better way to keep track later. for now I only have 3 servers
        int index;
        std::string get_next_server();
        void proxy_data(int client_socket, int backend_socket);


};