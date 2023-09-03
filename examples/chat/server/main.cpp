/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include <csignal>
#include <future>

#include "easywebsocket/server/ServerBuilder.hpp"
#include "easywebsocket/server/Types.hpp"
#include "easywebsocket/server/TypesFwd.hpp"

#include "EventHandler.hpp"

std::promise<void> promise;

void signalHandler(int)
{
    promise.set_value();
}

void waitForSignal()
{
    promise.get_future().wait();
}

auto main() -> int
{
    signal(SIGINT, signalHandler);

    using namespace ews;
    const srv::Port PORT = 9000;

    auto eventHandler = std::make_shared<chat::EventHandler>();
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setPort(PORT)
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setEventHandler(eventHandler)
        .setDataSenderAcceptor(eventHandler)
        ;
    auto server = serverBuilder.build();

    waitForSignal(); // Ctrl+C

    return 0;
}
