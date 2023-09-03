/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Aug, 2023
 */

#include <iostream>
#include <thread>

#include "easywebsocket/client/ClientBuilder.hpp"
#include "easywebsocket/client/EventHandlerBase.hpp"
#include "easywebsocket/client/IDataSender.hpp"

#include "easywebsocket/server/EventHandlerBase.hpp"
#include "easywebsocket/server/IDataSender.hpp"
#include "easywebsocket/server/ServerBuilder.hpp"

using namespace ews;

class ClientEventHandler : public cli::EventHandlerBase
{
public:
    void onConnect(cli::ISessionInfoPtr) noexcept override
    {
        _dataSender->sendTextData("hello server!");
    }

    void onTextDataReceive(const std::string& message, size_t /*bytesRemains*/) noexcept override
    {
        std::cout << "client received the message: " << message << std::endl;
    }
};

class ServerEventHandler : public srv::EventHandlerBase
{
public:
    void onTextDataReceive(srv::SessionId, const std::string& message, size_t /*bytesRemains*/) noexcept override
    {
        std::cout << "server received the message: " << message << std::endl;
        _dataSender->sendTextData("hello client!");
    }
};

auto main() -> int
{
    const srv::Port PORT = 9000;

    // Configure and build server
    auto serverEventHandler = std::make_shared<ServerEventHandler>();
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setPort(PORT)
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setEventHandler(serverEventHandler)
        .setDataSenderAcceptor(serverEventHandler)
        .setLwsLogLevel(0)
        ;
    auto server = serverBuilder.build();

    // Configure and build client
    auto clientEventHandler = std::make_shared<ClientEventHandler>();
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setAddress("localhost")
        .setPort(PORT)
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setEventHandler(clientEventHandler)
        .setDataSenderAcceptor(clientEventHandler)
        .setLwsLogLevel(0)
        ;
    auto client = clientBuilder.build();

    // Give some time for the client and server to exchange with the messages
    const auto timeout = 100U;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

    return 0;
}
