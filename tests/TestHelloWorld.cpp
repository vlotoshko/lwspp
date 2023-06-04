/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <future>
#include <iostream>
#include <thread>

#include "catch2/catch.hpp"

#include "websocketpp/client/ClientContextBuilder.hpp"
#include "websocketpp/client/ClientFactory.hpp"
#include "websocketpp/client/IClient.hpp"
#include "websocketpp/client/IEventHandler.hpp"
#include "websocketpp/client/IMessageSender.hpp"

#include "websocketpp/server/IEventHandler.hpp"
#include "websocketpp/server/IMessageSender.hpp"
#include "websocketpp/server/IServer.hpp"
#include "websocketpp/server/ServerFactory.hpp"
#include "websocketpp/server/ServerContextBuilder.hpp"

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace wspp::tests
{

const srv::Port PORT = 9000;

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
    auto serverBuilder = srv::ServerContextBuilder{};
    serverBuilder
        .setVersion(srv::ServerVersion::v1_Andromeda)
        .setPort(PORT)
        .setEventHandler(serverEventHandler)
        ;

    return srv::createServer(*serverBuilder.build());
}

cli::IClientPtr setupClient(std::string& messageHolder)
{
    auto clientEventHandler = std::make_shared<ClientEventHandler>(messageHolder);
    auto clientBuilder = cli::ClientContextBuilder{};
    clientBuilder
        .setVersion(cli::ClientVersion::v1_Amsterdam)
        .setAddress("localhost")
        .setPort(PORT)
        .setEventHandler(clientEventHandler)
        ;

    return cli::createClient(*clientBuilder.build());
}

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
                auto asyncServerStart = std::async(std::launch::async, [&](){server->start();});
                auto asyncClientStart = std::async(std::launch::async, [&](){client->connect();});

                // waiting for the client and server exchange with the messages
                const auto timeout = 100U;
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

                client.reset();
                server.reset();

                asyncClientStart.wait();
                asyncServerStart.wait();

                REQUIRE(actualMessageReceivedByServer == HELLO_SERVER);
                REQUIRE(actualMessageReceivedByClient == HELLO_CLIENT);
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace wspp::tests
// NOLINTEND (readability-function-cognitive-complexity)
