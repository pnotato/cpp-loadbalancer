#include "tcp.h"
#include "loadbalancer.h"
#include <thread>


int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: ./loadbalancer <port>" << std::endl;
        return 1;
    }
    TCPServer server(argv[1]);
    std::vector<std::string> res = {"http://localhost:5000", "http://localhost:5001", "http://localhost:5002"};
    LoadBalancer lb(res);
    server.start(lb);
    
    return 0;
}