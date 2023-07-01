/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <thread>

#include "catch2/catch.hpp"

#include "easywebsocket/client/ClientContextBuilder.hpp"
#include "easywebsocket/client/ClientFactory.hpp"
#include "easywebsocket/client/EventHandlerBase.hpp"

#include "easywebsocket/server/EventHandlerBase.hpp"
#include "easywebsocket/server/ISessionInfo.hpp"
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

const std::string SPECIFIC_PATH_NAME = "SPECIFIC_PATH_NAME";
const std::string SPECIFIC_PATH_NAME_2 = "SPECIFIC_PATH_NAME_2";

class ServerEventHandlerProtocolName : public srv::EventHandlerBase
{
public:
    explicit ServerEventHandlerProtocolName(bool& b)
        :_connected(b)
    {}
    void onConnect(srv::ISessionInfoPtr) noexcept override
    {
        _connected = true;
    }
private:
    bool& _connected;
};

class ServerEventHandlerPath : public srv::EventHandlerBase
{
public:
    explicit ServerEventHandlerPath(bool& b)
        : _useSpecificBehaviour(b)
    {}
    void onConnect(srv::ISessionInfoPtr sessionInfo) noexcept override
    {
        if (sessionInfo != nullptr && sessionInfo->getPath() == SPECIFIC_PATH_NAME)
        {
            _useSpecificBehaviour = true;
        }
    }
private:
    bool& _useSpecificBehaviour;
};

class ClientEventHandlerProtocolName : public cli::EventHandlerBase
{
public:
    explicit ClientEventHandlerProtocolName(bool& b)
        :_connected(b)
    {}
    void onConnect(cli::ISessionInfoPtr) noexcept override
    {
        _connected = true;
    }
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

        auto serverEventHandler
            = std::make_shared<ServerEventHandlerProtocolName>(actualServerConnected);
        auto serverBuilder = srv::ServerContextBuilder{};
        serverBuilder
            .setVersion(srv::ServerVersion::v1_Andromeda)
            .setPort(PORT)
            .setEventHandler(serverEventHandler)
            .setMessageSenderAcceptor(serverEventHandler)
            ;

        auto clientEventHandler
            = std::make_shared<ClientEventHandlerProtocolName>(actualClientConnected);
        auto clientBuilder = cli::ClientContextBuilder{};
        clientBuilder
            .setVersion(cli::ClientVersion::v1_Amsterdam)
            .setAddress(ADDRESS)
            .setPort(PORT)
            .setEventHandler(clientEventHandler)
            .setMessageSenderAcceptor(clientEventHandler)
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


SCENARIO( "Path feature testing", "[path]" )
{
    GIVEN( "Server and client" )
    {
        bool actualUseSpecificBehaviour = false;
        auto serverEventHandler = std::make_shared<ServerEventHandlerPath>(actualUseSpecificBehaviour);
        auto serverBuilder = srv::ServerContextBuilder{};
        serverBuilder
            .setVersion(srv::ServerVersion::v1_Andromeda)
            .setPort(PORT)
            .setEventHandler(serverEventHandler)
            .setMessageSenderAcceptor(serverEventHandler)
            ;

        auto clientEventHandler = std::make_shared<cli::EventHandlerBase>();
        auto clientBuilder = cli::ClientContextBuilder{};
        clientBuilder
            .setVersion(cli::ClientVersion::v1_Amsterdam)
            .setAddress(ADDRESS)
            .setPort(PORT)
            .setEventHandler(clientEventHandler)
            .setMessageSenderAcceptor(clientEventHandler)
            ;

        WHEN( "Client uses default uri path" )
        {
            auto server = srv::createServer(*serverBuilder.build());
            auto client = cli::createClient(*clientBuilder.build());

            THEN("Server uses default behavoiur")
            {
                waitForInitialization();

                const bool expectedUseSpecificBehaviour = false;
                REQUIRE(actualUseSpecificBehaviour == expectedUseSpecificBehaviour);
            }
        }

        WHEN( "Client uses specific uri path" )
        {
            auto server = srv::createServer(*serverBuilder.build());
            clientBuilder.setPath(SPECIFIC_PATH_NAME);
            auto client = cli::createClient(*clientBuilder.build());

            AND_WHEN("Server uses specific behavoiur")
            {
                waitForInitialization();

                const bool expectedUseSpecificBehaviour = true;
                REQUIRE(actualUseSpecificBehaviour == expectedUseSpecificBehaviour);
            }
        }

        WHEN( "Client uses specific uri path, but other than specified by server" )
        {
            auto server = srv::createServer(*serverBuilder.build());
            clientBuilder.setPath(SPECIFIC_PATH_NAME_2);
            auto client = cli::createClient(*clientBuilder.build());

            AND_WHEN("Server uses default behavoiur")
            {
                waitForInitialization();

                const bool expectedUseSpecificBehaviour = false;
                REQUIRE(actualUseSpecificBehaviour == expectedUseSpecificBehaviour);
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
