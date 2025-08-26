#include "GatewayApp.hpp"
#include <chrono>

int main(int argc, char** argv)
{
    Arguments args;
    args.participantName = "Gateway";
    args.duration = 100ms; // periodic DoWork invocations (not strictly necessary)

    GatewayApp app{args};
    app.SetupCommandLineArgs(argc, argv, "SIL Kit CAN -> SOME/IP Gateway");
    return app.Run();
}
