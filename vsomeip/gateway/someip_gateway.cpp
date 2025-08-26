#include "someip_gateway.hpp"
#include <iostream>
#include <iterator> 

#define CLIENT_ID       0x1000
#define SERVICE_ID      0x1234
#define INSTANCE_ID     0x5678
#define METHOD_ID       0x0421

SomeIPGateway::SomeIPGateway()
{
    app_ = vsomeip::runtime::get()->create_application("GatewayClient");
}

void SomeIPGateway::init()
{
    app_->init();
    app_->register_message_handler(SERVICE_ID, INSTANCE_ID, METHOD_ID,
        std::bind(&SomeIPGateway::on_message, this, std::placeholders::_1));
    app_->start();
}

void SomeIPGateway::send_request(const uint8_t* data, size_t size)
{
    std::lock_guard<std::mutex> lock(send_mutex_);

    std::cout << "[SOME/IP] Sending request with payload: ";
    for (size_t i = 0; i < size; ++i)
        printf("%02X ", data[i]);
    std::cout << std::endl;

    auto request = vsomeip::runtime::get()->create_request();

    request->set_service(SERVICE_ID);
    request->set_instance(INSTANCE_ID);
    request->set_method(METHOD_ID);
    request->set_client(CLIENT_ID);

    auto payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> pl_data(data, data + size);
    payload->set_data(pl_data);

    request->set_payload(payload);

    app_->send(request);
}

void SomeIPGateway::on_message(const std::shared_ptr<vsomeip::message>& response)
{
    auto payload = response->get_payload();
    if (!payload)
    {
        std::cout << "[SOME/IP] Received response with empty payload" << std::endl;
        return;
    }
    auto data = payload->get_data();           // pointeur vers unsigned char
    auto size = payload->get_length();         // taille du payload

    std::cout << "[SOME/IP] Received response payload: ";
    for (size_t i = 0; i < size; ++i)
        printf("%02X ", data[i]);
    std::cout << std::endl;
}


