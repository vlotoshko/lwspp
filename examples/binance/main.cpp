/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Aug, 2023
 */

#include <future>
#include <iostream>

#include "easywebsocket/client/EventHandlerBase.hpp"
#include "easywebsocket/client/IMessageSender.hpp"
#include "easywebsocket/client/ClientBuilder.hpp"
#include "easywebsocket/client/SslSettingsBuilder.hpp"

using namespace ews;

const cli::Port PORT = 9443;
const cli::Address ADDRESS = "stream.binance.com";
const std::string SUBSCRIBE = R"({"method": "SUBSCRIBE","params":["btcusdt@trade"],"id": 3})";
const int MESSAGES_LIMIT = 5;

class ClientEventHandler : public cli::EventHandlerBase
{
public:
    void onConnect(cli::ISessionInfoPtr) noexcept override
    {
        _messageSender->sendMessage(SUBSCRIBE);
    }

    void onMessageReceive(const std::string& message) noexcept override
    {
        if (_messageCounter < MESSAGES_LIMIT)
        {
            std::cout << "client received the message: " << message << std::endl;
        }

        if (++_messageCounter == MESSAGES_LIMIT)
        {
            _promise.set_value();
        }
    }

    auto getFuture() -> std::shared_future<void>
    {
        return _promise.get_future();
    }

private:
    std::promise<void> _promise;
    int _messageCounter = 0;
};

auto main() -> int
{
    // Configure and build client
    auto clientEventHandler = std::make_shared<ClientEventHandler>();
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setAddress(ADDRESS)
        .setPath("/ws")
        .setPort(PORT)
        .setVersion(cli::ClientVersion::v1_Amsterdam)
        .setEventHandler(clientEventHandler)
        .setMessageSenderAcceptor(clientEventHandler)
        .setSslSettings(cli::SslSettingsBuilder{}.build())
        .setLwsLogLevel(0)
        ;
    auto client = clientBuilder.build();

    auto waitForMessages = clientEventHandler->getFuture();
    waitForMessages.wait();

    return 0;
}
