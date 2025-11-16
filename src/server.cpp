#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../include/server.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <cstring>

// this is the main server logic file
// it will implement the functions declared in server.hpp

/*
    1. Create a TCP socket (IPv4, stream)
    2. If that fails, log and return error
    3. Optionally set socket options (SO_REUSEADDR, maybe SO_REUSEPORT)
    4. Build an address structure: "listen on all interfaces at my configured port"
    5. Bind the socket to that address/port
    6. If bind fails, close the socket, log, return error
    7. Mark it as a listening socket (listen())
    8. If listen fails, close the socket, log, return error
    9. On success, return the socket file descriptor
*/

int HttpServer::create_listening_socket()
{
    sockaddr_in serverAddress = {0};                    // Struct to hold server address info
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); //    AF_INET : IPv4 protocol - SOCK_STREAM: TCP socket

    if (serverSocket < 0)
    { // Check for socket creation error
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    serverAddress.sin_family = AF_INET;                // IPv4
    serverAddress.sin_port = htons(port);              // Port number (network byte order)
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on all interfaces

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0)
    { // Check for bind error 0 = success -- -1 = error
        std::cerr << "Error binding socket" << std::endl;
        close(serverSocket);
        return -1;
    } // Bind the socket to the address and port

    if (listen(serverSocket, 1) != 0)
    { // Check for listen error 0 = success -- -1 = error
        std::cerr << "Error listening on socket" << std::endl;
        close(serverSocket);
        return -1;
    } // Mark the socket as a listening socket with a backlog of 1

    return serverSocket;
}
int HttpServer::accept_connection()
{
    if (running == false)
    {
        std::cerr << "Error: Server not running" << std::endl;
        return -1;
    }

    if (server_fd < 0)
    {
        std::cerr << "Error: invalid file descriptor" << std::endl;
        return -1;
    }

    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int client_fd = accept(server_fd, (struct sockaddr *)&clientAddr, &addrLen);

    if (client_fd < 0)
    {
        perror("Error accepting connection");
        return -1;
    }
    char client_ip[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &clientAddr.sin_addr, client_ip, sizeof(client_ip));
    int client_port = ntohs(clientAddr.sin_port);
    std::cout << "Accepted client_fd=" << client_fd << " from " << client_ip << ":" << client_port << std::endl;

    // Log local address (server side of the accepted socket)
    sockaddr_in localAddr;
    socklen_t localLen = sizeof(localAddr);
    if (getsockname(client_fd, (struct sockaddr *)&localAddr, &localLen) == 0)
    {
        char local_ip[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET, &localAddr.sin_addr, local_ip, sizeof(local_ip));
        std::cout << "Local socket " << local_ip << ":" << ntohs(localAddr.sin_port) << std::endl;
    }

    return client_fd;
}

std::string HttpServer::read_request(int client_fd)
{

    // Temporary buffer on the stack
    constexpr size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    // Try to read up to BUFFER_SIZE bytes from the socket
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

    if (bytes_received < 0)
    {
        // Error condition
        perror("Error reading from client socket");
        return {};
    }

    if (bytes_received == 0)
    {
        // Client closed the connection gracefully
        std::cout << "Client closed the connection." << std::endl;
        return {};
    }

    // buffer = the string we want to return, static_cast<size_t>(bytes_received) = number of bytes we actually received
    return std::string(buffer, static_cast<size_t>(bytes_received));

    return "";
}

std::string HttpServer::ResponseBuilder()
{
    std::string response = "";
    std::string body = "Hello from my C++ server\n";
    size_t byteCount = body.size();
    std::cout << "Content-Length: " << byteCount << std::endl;

    response += "HTTP/1.1 200 OK\r\n";
    response += "Content-Length: " + std::to_string(byteCount) + "\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += body;

    return response;
}

void HttpServer::send_response(int client_fd, const std::string &response)
{
    size_t len = response.size();

    const char *resPtr = response.c_str();

    ssize_t bytes_sent;

    bytes_sent = send(client_fd, resPtr, len, 0);

    if (bytes_sent < 0)
    {
        perror("Error sending response");
        return;
    }

    if (bytes_sent < len)
    {
        std::cout << "Partial response sent (";
        std::cout << bytes_sent;
        std::cout << ") of total bytes to send: ";
        std::cout << len << std::endl;
        return;
    }

    std::cout << "Successfully responded!" << std::endl;

    return;
}

void HttpServer::initialize_server()
{

    if (running)
    {
        std::cerr << "Server is already running." << std::endl;
        return;
    }

    if (server_fd > 0)
    {
        std::cerr << "Server socket already created." << std::endl;
        return;
    }

    server_fd = create_listening_socket();
    if (server_fd < 0)
    {
        std::cerr << "Failed to create listening socket." << std::endl;
        return;
    }

    running = true;
    return;
}

HttpServer::HttpServer(int port)
{
    this->running = false;
    this->port = port;
    this->server_fd = -1;
}

HttpServer::~HttpServer()
{
    if (server_fd >= 0)
    {
        close(server_fd);
    }
}

void HttpServer::shutdown_server()
{
    // Implementation for shutting down server resources
    // Placeholder implementation
}
