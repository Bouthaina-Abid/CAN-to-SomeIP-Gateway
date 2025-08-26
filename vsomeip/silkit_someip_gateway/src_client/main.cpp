#include "SomeIPClient.hpp"
#include <thread>


int main()
{
    SomeIPClient client;
    client.init();

    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
