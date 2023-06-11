/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <future>
#include <iostream>
#include <thread>

#include "catch2/catch.hpp"

#include "easywebsocket/client/ClientContextBuilder.hpp"
#include "easywebsocket/client/ClientFactory.hpp"
//#include "easywebsocket/client/IClient.hpp"
#include "easywebsocket/client/IEventHandler.hpp"
//#include "easywebsocket/client/IMessageSender.hpp"

#include "easywebsocket/server/IEventHandler.hpp"
//#include "easywebsocket/server/IMessageSender.hpp"
//#include "easywebsocket/server/IServer.hpp"
#include "easywebsocket/server/ServerFactory.hpp"
#include "easywebsocket/server/ServerContextBuilder.hpp"

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{
namespace
{

const srv::Port PORT = 9000;
const cli::Address ADDRESS = "localhost";
const std::string CUSTOM_PROTOCOL_NAME = "CUSTOM_PROTOCOL_NAME";
const std::string CUSTOM_PROTOCOL_NAME_2 = "CUSTOM_PROTOCOL_NAME_2";

class ServerEventHandler : public srv::IEventHandler
{
public:
    explicit ServerEventHandler(bool& b)
        :_connected(b)
    {}
    void onConnect(srv::SessionId) noexcept override
    {
        _connected = true;
    }
    void onDisconnect(srv::SessionId) noexcept override
    {}
    void onMessageReceive(srv::SessionId, const std::string&) noexcept override
    {}
    void onError(srv::SessionId, const std::string&) noexcept override
    {}
    void onWarning(srv::SessionId, const std::string&) noexcept override
    {}
    void setMessageSender(srv::IMessageSenderPtr) override
    {}
private:
    bool& _connected;
};

class ClientEventHandler : public cli::IEventHandler
{
public:
    explicit ClientEventHandler(bool& b)
        :_connected(b)
    {}
    void onConnect() noexcept override
    {
        _connected = true;
    }
    void onDisconnect() noexcept override
    {}
    void onMessageReceive(const std::string&) noexcept override
    {}
    void onError(const std::string&) noexcept override
    {}
    void onWarning(const std::string&) noexcept override
    {}
    void setMessageSender(cli::IMessageSenderPtr) override
    {}
private:
    bool& _connected;
};

void waitForInitialization()
{
    const auto timeout = 100U;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
}

} // namespace


SCENARIO( "Protocol name feature testing", "[protocol_name]" )
{
    GIVEN( "Server and client" )
    {
        bool actualServerConnected = false;
        bool actualClientConnected = false;

        auto serverEventHandler = std::make_shared<ServerEventHandler>(actualServerConnected);
        auto serverBuilder = srv::ServerContextBuilder{};
        serverBuilder
            .setVersion(srv::ServerVersion::v1_Andromeda)
            .setPort(PORT)
            .setEventHandler(serverEventHandler)
            ;

        auto clientEventHandler = std::make_shared<ClientEventHandler>(actualClientConnected);
        auto clientBuilder = cli::ClientContextBuilder{};
        clientBuilder
            .setVersion(cli::ClientVersion::v1_Amsterdam)
            .setAddress(ADDRESS)
            .setPort(PORT)
            .setEventHandler(clientEventHandler)
            ;

        WHEN( "Server uses default protocol name" )
        {
            auto server = srv::createServer(*serverBuilder.build());

            AND_WHEN("Client uses default protocol name")
            {
                auto client = cli::createClient(*clientBuilder.build());

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();

                    const bool expectedServerConnected = true;
                    const bool expectedClientConnected = true;
                    REQUIRE(actualServerConnected == expectedServerConnected);
                    REQUIRE(actualClientConnected == expectedClientConnected);
                }
            }

            AND_WHEN("Client uses custom protocol name")
            {
                clientBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME);
                auto client = cli::createClient(*clientBuilder.build());

                THEN( "Client cann't connect to the server" )
                {
                    waitForInitialization();

                    const bool expectedServerConnected = false;
                    const bool expectedClientConnected = false;
                    REQUIRE(actualServerConnected == expectedServerConnected);
                    REQUIRE(actualClientConnected == expectedClientConnected);
                }
            }
        }

        WHEN( "Server uses custom protocol name" )
        {
            serverBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME);
            auto server = srv::createServer(*serverBuilder.build());

            AND_WHEN("Client uses default protocol name")
            {
                auto client = cli::createClient(*clientBuilder.build());
                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();

                    const bool expectedServerConnected = true;
                    const bool expectedClientConnected = true;
                    REQUIRE(actualServerConnected == expectedServerConnected);
                    REQUIRE(actualClientConnected == expectedClientConnected);
                }
            }

            AND_WHEN("Client uses a custom protocol name that differs from the servers protocol name")
            {
                clientBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME_2);
                auto client = cli::createClient(*clientBuilder.build());
                THEN( "Client cann't connect to the server" )
                {
                    waitForInitialization();

                    const bool expectedServerConnected = false;
                    const bool expectedClientConnected = false;
                    REQUIRE(actualServerConnected == expectedServerConnected);
                    REQUIRE(actualClientConnected == expectedClientConnected);
                }
            }

            AND_WHEN("Client uses a custom protocol name that equals the servers protocol name")
            {
                clientBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME);
                auto client = cli::createClient(*clientBuilder.build());

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();

                    const bool expectedServerConnected = true;
                    const bool expectedClientConnected = true;
                    REQUIRE(actualServerConnected == expectedServerConnected);
                    REQUIRE(actualClientConnected == expectedClientConnected);
                }
            }
        }

    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
