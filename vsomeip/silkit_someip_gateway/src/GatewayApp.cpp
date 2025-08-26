#include "GatewayApp.hpp"
#include "CanDemoCommon.hpp" // optional for logging helpers
#include <iostream>
#include <vector>

void GatewayApp::AddCommandLineArgs()
{
    // Add network option if you want to override via CLI
    GetCommandLineParser()->Add<CommandlineParser::Option>(
        "network", "N", std::string("CAN1"), "-N, --network <name>",
        std::vector<std::string>{"Name of the CAN network to use."});
}

void GatewayApp::EvaluateCommandLineArgs()
{
    // nothing required here unless you want to read options
}

void GatewayApp::CreateControllers()
{
    // create CAN controller using ApplicationBase helper (GetParticipant())
    auto networkName = GetCommandLineParser()->Get<CommandlineParser::Option>("network").Value();
    _canController = GetParticipant()->CreateCanController("GatewayCanController", networkName);

    // add rx handler
    _canController->AddFrameHandler([this](ICanController* ctrl, const CanFrameEvent& ev) {
        this->OnCanFrame(ctrl, ev);
    });

    // optional: send ack handler logging
    _canController->AddFrameTransmitHandler([this](ICanController* ctrl, const CanFrameTransmitEvent& ack) {
        CanDemoCommon::FrameTransmitHandler(ack, GetLogger());
    });
}

void GatewayApp::InitControllers()
{
    // 1️⃣ Start CAN controller
    _canController->SetBaudRate(500'000, 2'000'000, 4'000'000);
    _canController->Start();

    // 2️⃣ Initialize SOME/IP in a separate thread to avoid blocking SIL Kit
    std::thread([this]() {
        _someip.init();  // This contains app_->start() internally
    }).detach();  // detach so SIL Kit can continue its simulation loop

    // 3️⃣ SIL Kit simulation loop continues normally
}


// unchanged from your last version — ensure this is compiled in GatewayApp.cpp
void GatewayApp::OnCanFrame(ICanController* /*ctrl*/, const SilKit::Services::Can::CanFrameEvent& frameEvent)
{
    const auto& frame = frameEvent.frame;

    std::cout << "[Gateway] Received CAN frame: canId=0x" << std::hex << frame.canId
              << " dlc=" << std::dec << static_cast<int>(frame.dlc) << " data=[";
    for (size_t i = 0; i < frame.dlc; ++i)
    {
        std::cout << std::hex << std::uppercase << static_cast<int>(frame.dataField[i]);
        if (i + 1 < frame.dlc) std::cout << " ";
    }
    std::cout << "]" << std::dec << std::endl;
    
    

    if (frame.dlc >= 1)
    {
        uint8_t speed = frame.dataField[0];
        std::cout << "[Gateway] Extracted speed value: " << static_cast<int>(speed) << std::endl;
        _someip.notify(&speed, 1);
    }
    else
    {
        std::cout << "[Gateway] Received CAN frame with no data, skipping SOME/IP request." << std::endl;
    }
}




void GatewayApp::DoWorkSync(std::chrono::nanoseconds /*now*/)
{
    // nothing needed; CAN callbacks will call SOME/IP
}

void GatewayApp::DoWorkAsync()
{
    // nothing needed
}
