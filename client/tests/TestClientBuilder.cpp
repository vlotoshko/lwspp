/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "catch2/catch.hpp"

#include "easywebsocket/client/ClientBuilder.hpp"
#include "easywebsocket/client/EventHandlerBase.hpp"
#include "easywebsocket/client/SslSettingsBuilder.hpp"
#include "ClientContext.hpp"
#include "SslSettings.hpp"

namespace ews::cli
{

class TestClientBuilder
{
public:
    explicit TestClientBuilder(const ClientBuilder& clientBuiler)
        : _clientBuilder(clientBuiler)
    {}

    auto getClientContext() const -> const ClientContext&
    {
        return *_clientBuilder._context;
    }

private:
    const ClientBuilder& _clientBuilder;
};

} // namespace ews::cli

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{
using namespace cli;

const Port PORT = 9000;
const Address ADDRESS = "localhost";
const std::string PROTOCOL_NAME = "PROTOCOL_NAME";
const Path PATH = "PATH";

const std::string CA_CERT_PATH = "/home/volodymyr/Projects/EasyWebsocket/tests/TestData/ssl/rootCA.crt";
const std::string CLIENT_CERT_PATH = "/home/volodymyr/Projects/EasyWebsocket/tests/TestData/ssl/client.crt";
const std::string CLIENT_KEY_PATH = "/home/volodymyr/Projects/EasyWebsocket/tests/TestData/ssl/client.key";

auto toString(ClientVersion version) -> std::string
{
    switch (version)
    {
    case ClientVersion::v1_Amsterdam:
        return "v1_Andromeda";
    default:
        return "Undefined";
    }
}

void compareClientContexts(const ClientContext& actual, const ClientContext& expected)
{
    // Mandatory parameters
    REQUIRE(toString(actual.clientVersion) == toString(expected.clientVersion));
    REQUIRE(actual.eventHandler == expected.eventHandler);
    REQUIRE(actual.address == expected.address);
    REQUIRE(actual.port == expected.port);

    // Non-mandatory parameters
    REQUIRE(actual.protocolName == expected.protocolName);
    REQUIRE(actual.path == expected.path);
    REQUIRE((actual.ssl != nullptr && expected.ssl != nullptr ||
             actual.ssl == nullptr && expected.ssl == nullptr));

    if (actual.ssl != nullptr && expected.ssl != nullptr)
    {
        REQUIRE(actual.ssl->privateKeyPath == expected.ssl->privateKeyPath);
        REQUIRE(actual.ssl->certPath == expected.ssl->certPath);
        REQUIRE(actual.ssl->caCertPath == expected.ssl->caCertPath);
    }
}


SCENARIO( "ClientContext setup", "[client_builder]" )
{
    GIVEN( "ClientContextBuilder" )
    {
        auto clientBuilder = ClientBuilder{};

        WHEN( "All parameters are set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            auto sslSettings = SslSettingsBuilder{}
                                   .setPrivateKeyFilepath(CLIENT_KEY_PATH)
                                   .setCertFilepath(CLIENT_CERT_PATH)
                                   .setCaCertFilepath(CA_CERT_PATH)
                                   .build();
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT)
                .setEventHandler(handler)
                .setMessageSenderAcceptor(handler)
                .setProtocolName(PROTOCOL_NAME)
                .setPath(PATH)
                .setSslSettings(sslSettings);

            const ClientContext& actual = TestClientBuilder{clientBuilder}.getClientContext();

            THEN( "Client context has correct data" )
            {
                ClientContext expected;
                expected.clientVersion = ClientVersion::v1_Amsterdam;
                expected.address = ADDRESS;
                expected.port = PORT;
                expected.eventHandler = handler;
                expected.protocolName = PROTOCOL_NAME;
                expected.path = PATH;
                expected.ssl = std::make_shared<SslSettings>();
                expected.ssl->privateKeyPath = CLIENT_KEY_PATH;
                expected.ssl->certPath = CLIENT_CERT_PATH;
                expected.ssl->caCertPath = CA_CERT_PATH;

                compareClientContexts(actual, expected);
            }
        }
    } // GIVEN
} // SCENARIO

SCENARIO( "Client construction", "[client_builder]" )
{
    GIVEN( "ClientBuilder" )
    {
        auto clientBuilder = ClientBuilder{};

        WHEN( "Client version is not set" )
        {
            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: client version");
            }
        }

        WHEN( "Address is not set" )
        {
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setPort(PORT);

            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: address");
            }
        }

        WHEN( "Port is not set" )
        {
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setAddress(ADDRESS);

            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: port");
            }
        }

        WHEN( "Event handler is not set" )
        {
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT);

            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: event handler");
            }
        }

        WHEN( "MessageSenderAcceptor is not set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT)
                .setEventHandler(handler);

            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: message sender acceptor");
            }
        }

        WHEN( "All mandatory parameters are set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT)
                .setEventHandler(handler)
                .setMessageSenderAcceptor(handler);

            auto expression = [&clientBuilder] {clientBuilder.build();};

            THEN( "Client builds successfully" )
            {
                REQUIRE_NOTHROW(expression());
            }

            AND_WHEN( "All non-mandatory parameters are set" )
            {
                clientBuilder
                    .setProtocolName(PROTOCOL_NAME)
                    .setPath(PATH)
                    .setSslSettings(std::make_shared<SslSettings>());

                THEN( "Client builds successfully" )
                {
                    REQUIRE_NOTHROW(expression());
                }
            }

            AND_WHEN( "Dependent parameter 'ssl certificate path' is not set" )
            {
                auto sslSettings =
                    SslSettingsBuilder{}
                        .setPrivateKeyFilepath(CLIENT_KEY_PATH)
                        .build();

                clientBuilder.setSslSettings(sslSettings);

                THEN( "Exception is thrown on client build" )
                {
                    REQUIRE_THROWS_WITH(clientBuilder.build(),
                                        "Required parameter is undefined: ssl certificate path");
                }
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
