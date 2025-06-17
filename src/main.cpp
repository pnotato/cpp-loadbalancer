#include "tcp.h"
#include "loadbalancer.h"
#include <thread>


int main()
{
    // TCPServer server(8080);
    // server.start();
    // return 0;

    std::vector<std::string> res = {"http://localhost:5000", "http://localhost:5001", "http://localhost:5002"};
    LoadBalancer lb(res);

    
    
    return 0;
}