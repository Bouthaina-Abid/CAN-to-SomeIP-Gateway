#pragma once

#include <vsomeip/vsomeip.hpp>
#include <memory>

class SomeIPClient
{
public:
    SomeIPClient();
    void init();

private:
    std::shared_ptr<vsomeip::application> app_;
};
