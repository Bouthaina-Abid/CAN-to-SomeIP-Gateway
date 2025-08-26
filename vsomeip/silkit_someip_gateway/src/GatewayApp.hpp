#pragma once

#include "SomeIPGateway.hpp"

#include "ApplicationBase.hpp"          // from SilKit demos
#include "silkit/services/can/all.hpp"

using namespace SilKit::Services::Can;

class GatewayApp : public ApplicationBase
{
public:
    using ApplicationBase::ApplicationBase;
    ~GatewayApp() = default;

private:
    ICanController* _canController{nullptr};
    SomeIPGateway _someip;

    // ApplicationBase overrides
    void AddCommandLineArgs() override;
    void EvaluateCommandLineArgs() override;
    void CreateControllers() override;
    void InitControllers() override;
    void DoWorkSync(std::chrono::nanoseconds) override;
    void DoWorkAsync() override;

    // CAN callback
    void OnCanFrame(ICanController* ctrl, const CanFrameEvent& ev);
    

};
