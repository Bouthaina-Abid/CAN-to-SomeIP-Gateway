#ifndef CAN_SOCKET_HPP
#define CAN_SOCKET_HPP

#include <linux/can.h>
#include <string>
#include <mutex>
#include <thread>

class SomeIPGateway;  // déclaration anticipée

class CanSocket {
public:
    CanSocket(const std::string& interface_name, SomeIPGateway* gateway);
    ~CanSocket();

    bool open();            // ouvre le socket CAN
    void close();           // ferme le socket CAN
    void start();           // lance la boucle de lecture dans un thread
    void stop();            // arrête la boucle

private:
    void read_loop();

    std::string interface_name_;
    int socket_fd_ = -1;
    bool running_ = false;
    SomeIPGateway* someip_gateway_;

    std::thread read_thread_;
    std::mutex mutex_;
};

#endif // CAN_SOCKET_HPP
