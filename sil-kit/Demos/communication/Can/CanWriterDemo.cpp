#include "../include/ApplicationBase.hpp"
#include "CanDemoCommon.hpp"
#include <vector>
#include <string>

class CanWriter : public ApplicationBase
{
public:
    using ApplicationBase::ApplicationBase;

private:
    ICanController* _canController{nullptr};
    std::string _networkName = "CAN1";
    bool _printHex{true};  // You can toggle with command line args if you want
    uint8_t speedValue = 100;
    uint8_t _buffer[1];  // Persistent buffer for frame data

    void AddCommandLineArgs() override
    {
        GetCommandLineParser()->Add<CommandlineParser::Option>(
            "network", "N", _networkName, "-N, --network <name>", 
            std::vector<std::string>{"Name of the CAN network to use."});

        GetCommandLineParser()->Add<CommandlineParser::Flag>(
            "hex", "H", "-H, --hex", 
            std::vector<std::string>{"Print CAN payload as hex."});
    }

    void EvaluateCommandLineArgs() override
    {
        _networkName = GetCommandLineParser()->Get<CommandlineParser::Option>("network").Value();
        _printHex = GetCommandLineParser()->Get<CommandlineParser::Flag>("hex").Value();
    }

    void CreateControllers() override
    {
        _canController = GetParticipant()->CreateCanController("CanController1", _networkName);
        if (!_canController)
        {
            GetLogger()->Error("Failed to create CAN controller");
            throw std::runtime_error("Failed to create CAN controller");
        }
        _canController->AddFrameTransmitHandler([this](ICanController*, const CanFrameTransmitEvent& ack) {
            CanDemoCommon::FrameTransmitHandler(ack, GetLogger());
        });
        _canController->AddFrameHandler([this](ICanController*, const CanFrameEvent& frameEvent) {
            CanDemoCommon::FrameHandler(frameEvent, GetLogger(), _printHex);
        });
    }

    void InitControllers() override
    {
        _canController->SetBaudRate(500'000, 2'000'000, 4'000'000);
        _canController->Start();
    }

    void SendFrame()
    {
        CanFrame frame{};
        frame.canId = 0x12; 
        frame.flags = static_cast<CanFrameFlagMask>(CanFrameFlag::Fdf)
                    | static_cast<CanFrameFlagMask>(CanFrameFlag::Brs);

        _buffer[0] = speedValue;
        frame.dataField = SilKit::Util::Span<const unsigned char>{_buffer, 1};
        frame.dlc = 1;

        GetLogger()->Info("Sending CAN FD frame with speed=" + std::to_string(speedValue));
        _canController->SendFrame(frame);

        if (speedValue >= 250)
            speedValue = 0;
        else
            speedValue += 10;
    }

    void DoWorkSync(std::chrono::nanoseconds) override { SendFrame(); }
    void DoWorkAsync() override { SendFrame(); }
};

int main(int argc, char** argv)
{
    Arguments args;
    args.participantName = "CanWriter";
    args.duration = 100ms; // Adjust as needed
    CanWriter app{args};
    app.SetupCommandLineArgs(argc, argv, "CAN Writer for Gateway");
    return app.Run();
}
