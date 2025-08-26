// ================================
// File: someip_gateway.hpp
// ================================
#ifndef SOMEIP_GATEWAY_HPP
#define SOMEIP_GATEWAY_HPP

#include <vsomeip/vsomeip.hpp>
#include <memory>
#include <mutex>

class SomeIPGateway
{
public:
    SomeIPGateway();
    void init();
    void send_request(const uint8_t* data, size_t size);

private:
    std::shared_ptr<vsomeip::application> app_;
    std::mutex send_mutex_;
    void on_message(const std::shared_ptr<vsomeip::message> &response);
};

#endif // SOMEIP_GATEWAY_HPP
