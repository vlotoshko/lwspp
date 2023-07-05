/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <thread>

#include "catch2/catch.hpp"

#include "easywebsocket/client/ClientBuilder.hpp"
#include "easywebsocket/client/EventHandlerBase.hpp"
#include "easywebsocket/client/IMessageSender.hpp"

#include "easywebsocket/server/EventHandlerBase.hpp"
#include "easywebsocket/server/IMessageSender.hpp"
#include "easywebsocket/server/ServerBuilder.hpp"

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{
namespace
{

const srv::Port PORT = 9000;
const cli::Address ADDRESS = "localhost";

const std::string HELLO_SERVER = "hello server!";
const std::string HELLO_CLIENT = "hello client!";

class ServerEventHandler : public srv::EventHandlerBase
{
public:
    explicit ServerEventHandler(std::string& i)
        :_incomeMessage(i)
    {}

    void onMessageReceive(srv::SessionId sessionId, const std::string& message) noexcept override
    {
        _incomeMessage = message;
        _messageSender->sendMessage(sessionId, HELLO_CLIENT);
    }

private:
    std::string& _incomeMessage;
};

class ClientEventHandler : public cli::EventHandlerBase
{
public:
    explicit ClientEventHandler(std::string& i)
        :_incomeMessage(i)
    {}

    void onConnect(cli::ISessionInfoPtr) noexcept override
    {
        _messageSender->sendMessage(HELLO_SERVER);
    }
    void onMessageReceive(const std::string& message) noexcept override
    {
        _incomeMessage = message;
    }

private:
    std::string& _incomeMessage;
};

srv::IServerPtr setupServer(std::string& messageHolder)
{
    auto serverEventHandler = std::make_shared<ServerEventHandler>(messageHolder);
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setVersion(srv::ServerVersion::v1_Andromeda)
        .setPort(PORT)
        .setEventHandler(serverEventHandler)
        .setMessageSenderAcceptor(serverEventHandler)
        ;

    return serverBuilder.build();
}

cli::IClientPtr setupClient(std::string& messageHolder)
{
    auto clientEventHandler = std::make_shared<ClientEventHandler>(messageHolder);
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setVersion(cli::ClientVersion::v1_Amsterdam)
        .setAddress(ADDRESS)
        .setPort(PORT)
        .setEventHandler(clientEventHandler)
        .setMessageSenderAcceptor(clientEventHandler)
        ;

    return clientBuilder.build();
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
