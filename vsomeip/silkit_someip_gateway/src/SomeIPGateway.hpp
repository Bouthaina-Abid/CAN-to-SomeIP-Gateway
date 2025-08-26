#pragma once
#include <vsomeip/vsomeip.hpp>
#include <memory>
#include <mutex>
#include <vector>

class SomeIPGateway
{
public:
    SomeIPGateway();
    ~SomeIPGateway() = default;

    void init();
    void notify(const uint8_t* data, size_t size);

private:
    std::shared_ptr<vsomeip::application> app_;
    std::mutex send_mutex_;
};


