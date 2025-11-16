#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

class HttpServer
{
public:
    HttpServer(int port);
    ~HttpServer();
    void initialize_server(); // Function to initialize server resources -> maybe want to add config params later
    void shutdown_server();
    int create_listening_socket();
    int accept_connection();
    std::string read_request(int client_fd); // Reads the HTTP request from the client -> may need to update for multiple reads, reading until double CRLF, reading body separately, content-length, etc.
    std::string ResponseBuilder();
    void send_response(int client_fd, const std::string &response); // Sends an HTTP response to the client --> may want to add overloads for different response types later, status codes, headers, etc.

private:
    int port;
    bool running;
    int server_fd;
};

#endif // SERVER_HPP