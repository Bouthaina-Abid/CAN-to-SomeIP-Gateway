#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <set>

#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID     0x1234
#define SAMPLE_INSTANCE_ID    0x5678
#define SAMPLE_EVENTGROUP_ID  0x4465
#define SAMPLE_EVENT_ID       0x8778

std::shared_ptr<vsomeip::application> app;
std::shared_ptr<vsomeip::payload> payload;

int main() {
    app = vsomeip::runtime::get()->create_application("World");
    app->init();

    // Offer service
    app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);

    // Prepare payload
    payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_data = {0x10, 0x20, 0x30};
    payload->set_data(its_data);

    // Offer event + group
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(SAMPLE_EVENTGROUP_ID);
    app->offer_event(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, its_groups);

    // Thread to send notifications every 2 seconds
    std::thread notifier([] {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            app->notify(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, payload);
            std::cout << "[SERVER] Sent event notification" << std::endl;
        }
    });
    notifier.detach();

    app->start();
}
