#include "someip_gateway.hpp"
#include "can_socket.hpp"
#include <iostream>     // std::cout, std::endl
#include <thread>
#include <chrono>

int main() {
    SomeIPGateway someip_gateway;
    someip_gateway.init();

    CanSocket can("vcan0", &someip_gateway);
    if (!can.open()) {
        std::cerr << "Impossible d'ouvrir le socket CAN" << std::endl;
        return 1;
    }

    can.start();

    // Laisse tourner la boucle CAN quelques secondes
    std::this_thread::sleep_for(std::chrono::seconds(30));

    can.stop();
    can.close();

    return 0;
}
