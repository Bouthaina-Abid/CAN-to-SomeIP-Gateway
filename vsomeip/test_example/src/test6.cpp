#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <set>

#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID     0x1234
#define SAMPLE_INSTANCE_ID    0x5678
#define SAMPLE_EVENTGROUP_ID  0x4465
#define SAMPLE_EVENT_ID       0x8778

std::shared_ptr<vsomeip::application> app;

void on_message(const std::shared_ptr<vsomeip::message> &_response) {
    std::stringstream its_message;
    its_message << "CLIENT: received notification ["
                << std::hex << std::setw(4) << std::setfill('0')
                << _response->get_service() << "."
                << std::setw(4) << _response->get_instance() << "."
                << std::setw(4) << _response->get_method()
                << "] Payload = ";

    auto its_payload = _response->get_payload();
    for (uint32_t i = 0; i < its_payload->get_length(); ++i) {
        its_message << std::hex << std::setw(2) << std::setfill('0')
                    << (int)its_payload->get_data()[i] << " ";
    }

    std::cout << its_message.str() << std::endl;
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "CLIENT: Service ["
              << std::hex << std::setw(4) << _service << "."
              << std::setw(4) << _instance
              << "] is " << (_is_available ? "available." : "NOT available.")
              << std::endl;

    if (_is_available) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::set<vsomeip::eventgroup_t> its_groups;
        its_groups.insert(SAMPLE_EVENTGROUP_ID);

        app->request_event(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, its_groups);
        app->subscribe(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENTGROUP_ID);
        std::cout << "CLIENT: Subscribed to event group." << std::endl;
    }
}

int main() {
    app = vsomeip::runtime::get()->create_application("Hello");
    app->init();

    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);

    app->register_message_handler(vsomeip::ANY_SERVICE, vsomeip::ANY_INSTANCE, vsomeip::ANY_METHOD, on_message);

    app->start();
}
