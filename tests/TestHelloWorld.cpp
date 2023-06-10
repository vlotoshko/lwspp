/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <future>
#include <thread>

#include "catch2/catch.hpp"

#include "easywebsocket/client/ClientContextBuilder.hpp"
#include "easywebsocket/client/ClientFactory.hpp"
#include "easywebsocket/client/IEventHandler.hpp"
#include "easywebsocket/client/IMessageSender.hpp"

#include "easywebsocket/server/IEventHandler.hpp"
#include "easywebsocket/server/IMessageSender.hpp"
#include "easywebsocket/server/ServerFactory.hpp"
#include "easywebsocket/server/ServerContextBuilder.hpp"

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{
namespace
{

const srv::Port PORT = 9000;
const cli::Address ADDRESS = "localhost";

const std::string HELLO_SERVER = "hello server!";
const std::string HELLO_CLIENT = "hello client!";

class ServerEventHandler : public srv::IEventHandler
{
public:
    explicit ServerEventHandler(std::string& i)
        :_incomeMessage(i)
    {}

    void onConnect(srv::SessionId) noexcept override
    {}
    void onDisconnect(srv::SessionId) noexcept override
    {}

    void onMessageReceive(srv::SessionId sessionId, const std::string& message) noexcept override
    {
        _incomeMessage = message;
        _messageSender->sendMessage(sessionId, HELLO_CLIENT);
    }

    void onError(srv::SessionId, const std::string&) noexcept override
    {}
    void onWarning(srv::SessionId, const std::string&) noexcept override
    {}

    void setMessageSender(srv::IMessageSenderPtr ms) override
    {
        _messageSender = ms;
    }

private:
    srv::IMessageSenderPtr _messageSender;
    std::string& _incomeMessage;
};

class ClientEventHandler : public cli::IEventHandler
{
public:
    explicit ClientEventHandler(std::string& i)
        :_incomeMessage(i)
    {}

    void onConnect() noexcept override
    {
        _messageSender->sendMessage(HELLO_SERVER);
    }
    void onDisconnect() noexcept override
    {}
    void onMessageReceive(const std::string& message) noexcept override
    {
        _incomeMessage = message;
    }
    void onError(const std::string&) noexcept override
    {}
    void onWarning(const std::string&) noexcept override
    {}

    void setMessageSender(cli::IMessageSenderPtr ms) override
    {
        _messageSender = ms;
    }

private:
    cli::IMessageSenderPtr _messageSender;
    std::string& _incomeMessage;
};

srv::IServerPtr setupServer(std::string& messageHolder)
{
    auto serverEventHandler = std::make_shared<ServerEventHandler>(messageHolder);
    auto serverContextBuilder = srv::ServerContextBuilder{};
    serverContextBuilder
        .setVersion(srv::ServerVersion::v1_Andromeda)
        .setPort(PORT)
        .setEventHandler(serverEventHandler)
        ;

    return srv::createServer(*serverContextBuilder.build());
}

cli::IClientPtr setupClient(std::string& messageHolder)
{
    auto clientEventHandler = std::make_shared<ClientEventHandler>(messageHolder);
    auto clientContextBuilder = cli::ClientContextBuilder{};
    clientContextBuilder
        .setVersion(cli::ClientVersion::v1_Amsterdam)
        .setAddress(ADDRESS)
        .setPort(PORT)
        .setEventHandler(clientEventHandler)
        ;

    return cli::createClient(*clientContextBuilder.build());
}

} // namespace

SCENARIO( "Clients sends 'hello world' to the server", "[hello_world]" )
{
    GIVEN( "Server and client" )
    {
        std::string actualMessageReceivedByServer;
        std::string actualMessageReceivedByClient;

        auto server = setupServer(actualMessageReceivedByServer);
        auto client = setupClient(actualMessageReceivedByClient);

        WHEN( "Client sends message to server" )
        {
            THEN( "Server receives message from client and sends message back" )
            {
                // waiting for the client and server exchange with the messages
                const auto timeout = 100U;
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

                REQUIRE(actualMessageReceivedByServer == HELLO_SERVER);
                REQUIRE(actualMessageReceivedByClient == HELLO_CLIENT);
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
