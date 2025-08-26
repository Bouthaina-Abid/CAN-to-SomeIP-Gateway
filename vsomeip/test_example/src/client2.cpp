#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <vsomeip/vsomeip.hpp>

#define SERVICE_ID     0x5678
#define INSTANCE_ID    0x9abc
#define METHOD_ID      0x0001

std::shared_ptr<vsomeip::application> app;

void on_request(const std::shared_ptr<vsomeip::message> &_request) {
    auto payload = _request->get_payload();
    auto data = payload->get_data();

    uint32_t speedValue = data[0] |
                          (data[1] << 8) |
                          (data[2] << 16) |
                          (data[3] << 24);

    std::cout << "[SERVICE] Received speedValue = " << speedValue << std::endl;

    // Réponse ACK avec la même valeur
    auto response = vsomeip::runtime::get()->create_response(_request);
    auto resp_payload = vsomeip::runtime::get()->create_payload();

    std::vector<vsomeip::byte_t> resp_data(4);
    resp_data[0] = data[0];
    resp_data[1] = data[1];
    resp_data[2] = data[2];
    resp_data[3] = data[3];

    resp_payload->set_data(resp_data);
    response->set_payload(resp_payload);

    app->send(response);
    std::cout << "[SERVICE] Sent ACK = " << speedValue << std::endl;
}

int main() {
    app = vsomeip::runtime::get()->create_application("Service2");
    app->init();

    app->register_message_handler(SERVICE_ID, INSTANCE_ID, METHOD_ID, on_request);
    app->offer_service(SERVICE_ID, INSTANCE_ID);

    std::cout << "[SERVICE] Started SOME/IP service..." << std::endl;
    app->start();
    return 0;
}
