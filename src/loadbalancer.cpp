#include "loadbalancer.h"


LoadBalancer::LoadBalancer(std::vector<std::string> servers) {
    this->servers = servers;
    this->index = 0;
}

void LoadBalancer::handle_connection(int client_socket) {
    std::string server_url = get_next_server();


    std::smatch match;
    std::regex url_regex(R"(http://([^:/]+):(\d+))");
    if (!std::regex_match(server_url, match, url_regex)) {
        std::cerr << "ERROR: Invalid backend format: " << server_url << std::endl;
        close(client_socket);
        return;
    }

    std::string ip = match[1];       
    int port = std::stoi(match[2]);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "ERROR: Socket Creation Failed" << std::endl;
        close(client_socket);
        return;
    }

    sockaddr_in backend_addr{};
    backend_addr.sin_family = AF_INET;
    backend_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &backend_addr.sin_addr);

    if (connect(server_socket, (sockaddr*)&backend_addr, sizeof(backend_addr)) < 0) {
        std::cerr << "ERROR: Failed to connect to server" << std::endl;
        close(client_socket);
        close(server_socket);
        return;
    }

    proxy_data(client_socket, server_socket);

}

// Uses Round-robin!
std::string LoadBalancer::get_next_server() {
    std::string res = this->servers[this->index];
    if (this->index == (servers.size()-1))
    {
        this->index = 0; 
    }
    else {
         this->index++;
    }
    return res;
}   

void LoadBalancer::proxy_data(int client_socket, int backend_socket) {
     char buffer[4096];

    while (true) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(client_socket, &read_fds);
        FD_SET(backend_socket, &read_fds);

        int max_fd = std::max(client_socket, backend_socket) + 1;

        int activity = select(max_fd, &read_fds, nullptr, nullptr, nullptr);
        if (activity < 0) {
            perror("select error");
            break;
        }

        // Client -> Backend
        if (FD_ISSET(client_socket, &read_fds)) {
            ssize_t bytes = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) break; // closed or error
            send(backend_socket, buffer, bytes, 0);
        }

        // Backend -> Client
        if (FD_ISSET(backend_socket, &read_fds)) {
            ssize_t bytes = recv(backend_socket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) break;
            send(client_socket, buffer, bytes, 0);
        }
    }

    close(client_socket);
    close(backend_socket);

}