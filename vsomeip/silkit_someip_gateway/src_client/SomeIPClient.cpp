#include "SomeIPClient.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <iomanip>

#define SERVICE_ID      0x1234
#define INSTANCE_ID     0x5678
#define EVENT_ID        0x8778
#define EVENT_GROUP_ID  0x4465

SomeIPClient::SomeIPClient()
{
    // Name must match your client JSON "name" field (e.g. "Client")
    app_ = vsomeip::runtime::get()->create_application("Client");
}

void SomeIPClient::init()
{
    if (!app_->init()) {
        std::cerr << "[CLIENT] Init failed" << std::endl;
        return;
    }

    // Availability handler: subscribe to event once service is available
    app_->register_availability_handler(SERVICE_ID, INSTANCE_ID,
        [this](vsomeip::service_t, vsomeip::instance_t, bool is_available) {
            if (is_available) {
                std::set<vsomeip::eventgroup_t> groups;
                groups.insert(EVENT_GROUP_ID);

                app_->request_event(SERVICE_ID, INSTANCE_ID, EVENT_ID, groups);
                app_->subscribe(SERVICE_ID, INSTANCE_ID, EVENT_GROUP_ID);

                std::cout << "[CLIENT] Subscribed to event group." << std::endl;
            }
        });

    // Message handler: receive notifications
    app_->register_message_handler(vsomeip::ANY_SERVICE, vsomeip::ANY_INSTANCE, vsomeip::ANY_METHOD,
        [](const std::shared_ptr<vsomeip::message>& msg) {
            auto pl = msg->get_payload();
            if (!pl) return;

            auto data_ptr = pl->get_data();
            auto len = pl->get_length();

            std::cout << "[CLIENT] Received notification: ";
            for (size_t i = 0; i < len; ++i)
                printf("%02X ", data_ptr[i]);
            std::cout << std::endl;
        });

    // Request the service to trigger availability
    app_->request_service(SERVICE_ID, INSTANCE_ID);

    // Start the vsomeip application
    app_->start();
}
