#include "socket.hh"
#include "util.hh"

#include <cstdlib>
#include <iostream>

using namespace std;

void get_URL(const string &host, const string &path) {
    // Your code here.

    // You will need to connect to the "http" service on
    // the computer whose name is in the "host" string,
    // then request the URL path given in the "path" string.

    // Then you'll need to print out everything the server sends back,
    // (not just one call to read() -- everything) until you reach
    // the "eof" (end of file).
    const std::string http_service = "http";
    const std::string http_version = "HTTP/1.1";
    const std::string crlf = "\r\n";

    try {
        // 创建并连接套接字
        TCPSocket socket;
        socket.connect(Address(host, http_service));

        // 构建HTTP请求
        std::string request = "GET " + path + " " + http_version + crlf +
                              "Host: " + host + crlf + 
                              "Connection: close" + crlf + crlf;
        socket.write(request);
        socket.shutdown(SHUT_WR); // 停止写操作

        // 读取并输出服务器响应
        while (!socket.eof()) {
            std::cout << socket.read();
        }

        // 关闭套接字
        socket.close();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    try {
        if (argc <= 0) {
            abort();  // For sticklers: don't try to access argv[0] if argc <= 0.
        }

        // The program takes two command-line arguments: the hostname and "path" part of the URL.
        // Print the usage message unless there are these two arguments (plus the program name
        // itself, so arg count = 3 in total).
        if (argc != 3) {
            cerr << "Usage: " << argv[0] << " HOST PATH\n";
            cerr << "\tExample: " << argv[0] << " stanford.edu /class/cs144\n";
            return EXIT_FAILURE;
        }

        // Get the command-line arguments.
        const string host = argv[1];
        const string path = argv[2];

        // Call the student-written function.
        get_URL(host, path);
    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
