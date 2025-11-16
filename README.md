# Tiny HTTP Server

This project implements a simple HTTP server in C++. It is designed to be lightweight and easy to use, making it suitable for educational purposes and small applications.

## Project Structure

```
tiny-http-server
├── .vscode
│   ├── launch.json
│   └── settings.json
├── CMakeLists.txt
├── src
│   └── main.cpp
├── include
│   └── server.hpp
└── README.md
```

## Requirements

- CMake (version 3.10 or higher)
- A C++ compiler that supports C++11 or higher

## Building the Project

To build the project, follow these steps:

1. Clone the repository or download the project files.
2. Open a terminal and navigate to the project directory.
3. Create a build directory:
   ```
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```
   cmake ..
   ```
5. Build the project:
   ```
   make
   ```

## Running the Server

After building the project, you can run the server with the following command:

```
./tiny-http-server
```

The server will start listening for incoming HTTP requests on the specified port (default is 8080). You can access it by navigating to `http://localhost:8080` in your web browser.

## Contributing

Feel free to contribute to this project by submitting issues or pull requests. Your feedback and contributions are welcome!

## License

This project is licensed under the MIT License. See the LICENSE file for more details.