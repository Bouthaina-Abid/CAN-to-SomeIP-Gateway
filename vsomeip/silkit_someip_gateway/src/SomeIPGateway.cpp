#include "SomeIPGateway.hpp"
#include <iostream>
#include <thread>
#include <chrono>

//#define CLIENT_ID   0x333   
#define SERVICE_ID  0x1234
#define INSTANCE_ID 0x5678
#define EVENT_ID 0x8778
#define EVENT_GROUP_ID 0x4465

std::shared_ptr<vsomeip::application> g_app; 

SomeIPGateway::SomeIPGateway()
{
    // Name must match client JSON "name", e.g. "Gateway"
    app_ = vsomeip::runtime::get()->create_application("GatewayNotifier");
    g_app = app_;
}

void SomeIPGateway::init()
{
    if (!app_->init())
    {
        std::cerr << "[SOME/IP] vsomeip init failed" << std::endl;
        return;
    }

    

    // Register availability handler so we request the service and send requests once offered
    app_->register_availability_handler(SERVICE_ID, INSTANCE_ID,
        [](vsomeip::service_t service, vsomeip::instance_t instance, bool is_available) {
            std::cout << "[SOME/IP] Availability: service "
                      << std::hex << service << "." << instance
                      << (is_available ? " available" : " not available") << std::dec << std::endl;

            if (is_available)
            {
                // small delay then optionally take action (we keep CAN -> SOME/IP sending on frame receipts)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
    });

     // Offer service + event for notify/subscribe
    app_->offer_service(SERVICE_ID, INSTANCE_ID);

    std::set<vsomeip::eventgroup_t> groups;
    groups.insert(EVENT_GROUP_ID);
    app_->offer_event(SERVICE_ID, INSTANCE_ID, EVENT_ID, groups);

    app_->start();
    std::cout << "[SOME/IP] vsomeip started (GatewayClient)" << std::endl;
}



void SomeIPGateway::notify(const uint8_t* data, size_t size)
{
    std::lock_guard<std::mutex> lock(send_mutex_);

    auto payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> pl_data(data, data + size);
    payload->set_data(pl_data);

    app_->notify(SERVICE_ID, INSTANCE_ID, EVENT_ID, payload);
    std::cout << "[SERVER] Notified subscribers with payload: ";
    for (size_t i = 0; i < size; ++i) printf("%02X ", data[i]);
    std::cout << std::endl;
}
