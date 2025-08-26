#include "can_socket.hpp"
#include "someip_gateway.hpp"  // pour SomeIPGateway

#include <iostream>
#include <cstring>      // memset
#include <unistd.h>     // read, close
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <linux/can/raw.h>

CanSocket::CanSocket(const std::string& interface_name, SomeIPGateway* gateway)
    : interface_name_(interface_name),
      someip_gateway_(gateway),
      socket_fd_(-1),
      running_(false)
{}

CanSocket::~CanSocket() {
    stop();
    close();
}

bool CanSocket::open() {
    if (socket_fd_ < 0) {
    std::cerr << "Erreur ouverture socket CAN" << std::endl;
    return false;
}

    if ((socket_fd_ = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        std::cerr << "Error while opening socket" << std::endl;
        return false;
    }

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, interface_name_.c_str(), IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    if (ioctl(socket_fd_, SIOCGIFINDEX, &ifr) < 0) {
        std::cerr << "Error getting interface index for " << interface_name_ << std::endl;
        close();
        return false;
    }

    struct sockaddr_can addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd_, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error in socket bind" << std::endl;
        close();
        return false;
    }
    

    std::cout << "Socket CAN ouvert sur " << interface_name_ << std::endl;
    return true;
}

void CanSocket::close() {
    if (socket_fd_ >= 0) {
        ::close(socket_fd_);
        socket_fd_ = -1;
    }
}

void CanSocket::start() {
    running_ = true;
    read_thread_ = std::thread(&CanSocket::read_loop, this);
}

void CanSocket::stop() {
    running_ = false;
    if (read_thread_.joinable())
        read_thread_.join();
}

void CanSocket::read_loop() {
    std::cout << "[DEBUG] read_loop started." << std::endl;

    struct can_frame frame;

    while (running_) {
        int nbytes = read(socket_fd_, &frame, sizeof(frame));
        if (nbytes < 0) {
            std::cerr << "Erreur lecture socket CAN" << std::endl;
            break;
        } else if (nbytes < static_cast<int>(sizeof(struct can_frame))) {
            std::cerr << "Lecture incomplète CAN frame" << std::endl;
            continue;
        }

        std::cout << "Trame CAN reçue ID=0x" << std::hex << frame.can_id << std::dec
                  << " DLC=" << static_cast<int>(frame.can_dlc) << " Data=";

        for (int i = 0; i < frame.can_dlc; i++)
            std::cout << std::hex << (int)frame.data[i] << " ";
        std::cout << std::dec << std::endl;

        // Envoi des données CAN à la passerelle SOME/IP
        if (someip_gateway_) {
            someip_gateway_->send_request(frame.data, frame.can_dlc);
        }
    }
}
