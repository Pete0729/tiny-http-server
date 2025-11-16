#include <iostream>
#include <string>
#include <unistd.h>
#include "../include/server.hpp"

int main(int argc, char *argv[])
{
    // For now, just confirm everything builds & runs.
    std::cout << "tiny_http_server starting up..." << std::endl;

    HttpServer server(8080);
    server.initialize_server();

    std::cout << "Listening on port 8080..." << std::endl;

    // For now: block and accept ONE connection, then close.
    int client_fd = server.accept_connection();

    if (client_fd < 0)
    {
        std::cout << "Failed to grab client socket" << std::endl;
        return 0;
    }

    std::cout << "Client connected!" << std::endl;

    std::string request_text = server.read_request(client_fd);

    std::cout << "Request Recieved: " + request_text << std::endl;

    std::string responseText = server.ResponseBuilder();

    server.send_response(client_fd, responseText);

    close(client_fd);

    return 0;
}
