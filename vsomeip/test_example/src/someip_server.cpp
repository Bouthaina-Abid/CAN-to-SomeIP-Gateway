#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>

#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678
#define SAMPLE_METHOD_ID 0x0421

class Service {
public:
    Service()
        : app_(vsomeip::runtime::get()->create_application("Service")) {}

    void init() {
        if (!app_->init()) {
            std::cerr << "[SERVER] Failed to initialize application." << std::endl;
            return;
        }

        app_->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID,
                                       std::bind(&Service::on_request, this, std::placeholders::_1));

        app_->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
        std::cout << "[SERVER] Service offered." << std::endl;

        app_->start();
    }

    void on_request(const std::shared_ptr<vsomeip::message> &request) {
        auto payload = request->get_payload();
        std::vector<vsomeip::byte_t> data(payload->get_data(),
                                          payload->get_data() + payload->get_length());

        std::cout << "[SERVER] Received request with data: ";
        for (auto byte : data)
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
        std::cout << std::dec << std::endl;

        // Process data: just add 1 to each byte
        for (auto &b : data) b += 1;

        // Create payload using factory
        auto response_payload = vsomeip::runtime::get()->create_payload();
        response_payload->set_data(data);

        auto response = vsomeip::runtime::get()->create_response(request);
        response->set_payload(response_payload);

        app_->send(response);
        std::cout << "[SERVER] Response sent." << std::endl;
    }

private:
    std::shared_ptr<vsomeip::application> app_;
};

int main() {
    Service server;
    server.init();
    return 0;
}
