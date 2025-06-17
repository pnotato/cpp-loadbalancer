<pre>
                                   +--------------------+
                                   |   Client Request   |
                                   |    (e.g. curl)     |
                                   +---------+----------+
                                             |
                                             v
                                   +--------------------+
                                   | TCP Load Balancer! |
                                   +----------+---------+
                                              |
                     +------------------------+------------------------+
                     |                        |                        |
                     v                        v                        v
              +-------------+          +-------------+          +-------------+
              |  Server #1  |          |  Server #2  |          |  Server #3  |
              +-------------+          +-------------+          +-------------+
</pre>




# A Load Balancer Implementation in C++

> Load balancing is the method of distributing network traffic equally across a pool of resources that support an application. [What is Load Balancing? - AWS](https://aws.amazon.com/what-is/load-balancing/)


A lightweight implementation of a loadbalancer built in C++. The program listens for client connections on a specified local port and forwards incoming traffic to a pool of backend TCP servers using a round-robin distribution strategy. The implementation uses standard C++ threading and sockets, without any external dependencies.

## Features

- Simple round-robin load balancing
- Multi-threaded handling of client connections
- Clean separation of server setup and connection handling
- Designed to work with local TCP echo servers or other TCP-based services

## Example

![loadbalancer video](./docs/example.gif)

## Usage
> Requires the following: CMake 3.10+ and C++17 or newer. For the sample backend servers, you will also need Python 3.10.12+ with Flask 3.11+ installed.

Clone the repository and build the project:
```bash
git clone https://github.com/yourusername/tcp-loadbalancer.git
cd tcp-loadbalancer
mkdir build && cd build
cmake ..
make
```

Run the loadbalancer with a port number as an argument. By default the program runs on port 8000.
```
build/ $ ./loadbalancer 8080
Server is listening on port 8080
Starting server...
```

In seperate terminals, start up the Flask servers:
```
servers/ $ python server_1.py
servers/ $ python server_2.py
servers/ $ python server_3.py 
```

You should now be able to send requests (e.g. curl) to the TCP server, which should be distributed accross the 3 backend servers accordingly.
```
curl http:/localhost:8080 // change it to the port you've set the loadbalancer to.
```

