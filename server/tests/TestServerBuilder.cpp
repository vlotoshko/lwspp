/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "catch2/catch.hpp"

#include "easywebsocket/server/EventHandlerBase.hpp"
#include "easywebsocket/server/ServerBuilder.hpp"
#include "easywebsocket/server/SslSettingsBuilder.hpp"
#include "ServerContext.hpp"
#include "SslSettings.hpp"

namespace ews::srv
{

class TestServerBuilder
{
public:
    explicit TestServerBuilder(const ServerBuilder& serverBuiler)
        : _serverBuilder(serverBuiler)
    {}

    auto getServerContext() const -> const ServerContext&
    {
        return *_serverBuilder._context;
    }

private:
    const ServerBuilder& _serverBuilder;
};

} // namespace ews::srv

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{
using namespace srv;

const Port PORT = 9000;
const std::string PROTOCOL_NAME = "PROTOCOL_NAME";

const std::string CA_CERT_PATH = "./TestData/ssl/rootCA.crt";
const std::string SERVER_CERT_PATH = "./TestData/ssl/server.crt";
const std::string SERVER_KEY_PATH = "./TestData/ssl/server.key";

auto toString(ServerVersion version) -> std::string
{
    switch (version)
    {
    case ServerVersion::v1_Andromeda:
        return "v1_Andromeda";
    default:
        return "Undefined";
    }
}

void compareServerContexts(const ServerContext& actual, const ServerContext& expected)
{
    // Mandatory parameters
    REQUIRE(toString(actual.serverVersion) == toString(expected.serverVersion));
    REQUIRE(actual.eventHandler == expected.eventHandler);
    REQUIRE(actual.port == expected.port);

    // Non-mandatory parameters
    REQUIRE(actual.protocolName == expected.protocolName);
    REQUIRE((actual.ssl != nullptr && expected.ssl != nullptr ||
             actual.ssl == nullptr && expected.ssl == nullptr));

    if (actual.ssl != nullptr && expected.ssl != nullptr)
    {
        REQUIRE(actual.ssl->privateKeyPath == expected.ssl->privateKeyPath);
        REQUIRE(actual.ssl->certPath == expected.ssl->certPath);
        REQUIRE(actual.ssl->caCertPath == expected.ssl->caCertPath);
        REQUIRE(actual.ssl->requireValidClientCert == expected.ssl->requireValidClientCert);
    }
}


SCENARIO( "ServerContext setup", "[server_builder]" )
{
    GIVEN( "ServerBuilder" )
    {
        auto serverBuilder = ServerBuilder{};

        WHEN( "All parameters are set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            auto sslSettings = SslSettingsBuilder{}
                                   .setPrivateKeyFilepath(SERVER_KEY_PATH)
                                   .setCertFilepath(SERVER_CERT_PATH)
                                   .setCaCertFilepath(CA_CERT_PATH)
                                   .requireValidClientCert()
                                   .build();

            serverBuilder
                .setVersion(ServerVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(handler)
                .setMessageSenderAcceptor(handler)
                .setProtocolName(PROTOCOL_NAME)
                .setSslSettings(sslSettings);

            const ServerContext& actual = TestServerBuilder{serverBuilder}.getServerContext();

            THEN( "Server context has correct data" )
            {
                ServerContext expected;
                expected.serverVersion = ServerVersion::v1_Andromeda;
                expected.eventHandler = handler;
                expected.port = PORT;
                expected.protocolName = PROTOCOL_NAME;
                expected.ssl = std::make_shared<SslSettings>();
                expected.ssl->privateKeyPath = SERVER_KEY_PATH;
                expected.ssl->certPath = SERVER_CERT_PATH;
                expected.ssl->caCertPath = CA_CERT_PATH;

                compareServerContexts(actual, expected);
            }
        }
    } // GIVEN
} // SCENARIO

SCENARIO( "Server construction", "[server_builder]" )
{
    GIVEN( "ServerBuilder" )
    {
        auto serverBuilder = ServerBuilder{};

        WHEN( "Server version is not set" )
        {
            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: server version");
            }
        }

        WHEN( "Port is not set" )
        {
            serverBuilder.setVersion(ServerVersion::v1_Andromeda);

            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: port");
            }
        }

        WHEN( "Event handler is not set" )
        {
            serverBuilder
                .setVersion(ServerVersion::v1_Andromeda)
                .setPort(PORT);

            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: event handler");
            }
        }

        WHEN( "MessageSenderAcceptor is not set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            serverBuilder
                .setVersion(ServerVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(handler);

            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: message sender acceptor");
            }
        }

        WHEN( "All mandatory parameters are set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            serverBuilder
                .setVersion(ServerVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(handler)
                .setMessageSenderAcceptor(handler);

            auto expression = [&serverBuilder] {serverBuilder.build();};

            THEN( "Server builds successfully" )
            {
                REQUIRE_NOTHROW(expression());
            }

            AND_WHEN( "All non-mandatory parameters are set" )
            {
                serverBuilder
                    .setProtocolName(PROTOCOL_NAME)
                    .setSslSettings(std::make_shared<SslSettings>());

                THEN( "Server builds successfully" )
                {
                    REQUIRE_NOTHROW(expression());
                }
            }

            AND_WHEN( "Dependent parameter 'ssl certificate path' is not set" )
            {
                auto sslSettings =
                    SslSettingsBuilder{}
                        .setPrivateKeyFilepath(SERVER_KEY_PATH)
                        .build();

                serverBuilder.setSslSettings(sslSettings);

                THEN( "Exception is thrown on server build" )
                {
                    REQUIRE_THROWS_WITH(serverBuilder.build(),
                                        "Required parameter is undefined: ssl certificate path");
                }
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
