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
    explicit TestServerBuilder(const ServerBuilder& serverBuilder)
        : _serverBuilder(serverBuilder)
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

namespace
{

const Port PORT = 9000;
const std::string PROTOCOL_NAME = "PROTOCOL_NAME";

const std::string CA_CERT_PATH     = "CA_CERT_PATH";
const std::string SERVER_CERT_PATH = "SERVER_CERT_PATH";
const std::string SERVER_KEY_PATH  = "SERVER_KEY_PATH";
const std::string PASSWORD  = "PASSWORD";
const std::string CIPHER_LIST  = "CIPHER_LIST";
const std::string CIPHER_LIST_TLS_13  = "CIPHER_LIST_TLS_13";
const std::string VHOST_NAME  = "VHOST_NAME";
const std::string SERVER_STRING = "SERVER_STRING";

const int KEEPALIVE_TIMEOUT = 20;
const int KEEPALIVE_PROBES = 5;
const int KEEPALIVE_PROBES_INTERVAL = 10;
const int LWS_LOG_LEVEL = 9;
const int LWS_LOG_LEVEL_DISABLE = 0;

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
    REQUIRE(actual.keepAliveTimeout == expected.keepAliveTimeout);
    REQUIRE(actual.keepAliveProbes == expected.keepAliveProbes);
    REQUIRE(actual.keepAliveProbesInterval == expected.keepAliveProbesInterval);
    REQUIRE(actual.vhostName == expected.vhostName);
    REQUIRE(actual.serverString == expected.serverString);
    REQUIRE(actual.lwsLogLevel == expected.lwsLogLevel);
    REQUIRE((actual.ssl != nullptr && expected.ssl != nullptr ||
             actual.ssl == nullptr && expected.ssl == nullptr));

    if (actual.ssl != nullptr && expected.ssl != nullptr)
    {
        REQUIRE(actual.ssl->privateKeyPath == expected.ssl->privateKeyPath);
        REQUIRE(actual.ssl->certPath == expected.ssl->certPath);
        REQUIRE(actual.ssl->caCertPath == expected.ssl->caCertPath);
        REQUIRE(actual.ssl->privateKeyPassword == expected.ssl->privateKeyPassword);
        REQUIRE(actual.ssl->ciphersList == expected.ssl->ciphersList);
        REQUIRE(actual.ssl->ciphersListTls13 == expected.ssl->ciphersListTls13);
        REQUIRE(actual.ssl->requireValidClientCert == expected.ssl->requireValidClientCert);
    }
}

} // namespace

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
                                   .setPrivateKeyPassword(PASSWORD)
                                   .setCiphersList(CIPHER_LIST)
                                   .setCiphersListTls13(CIPHER_LIST_TLS_13)
                                   .requireValidClientCert()
                                   .build();

            serverBuilder
                .setVersion(ServerVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(handler)
                .setMessageSenderAcceptor(handler)
                .setProtocolName(PROTOCOL_NAME)
                .setKeepAliveTimeout(KEEPALIVE_TIMEOUT)
                .setKeepAliveProbes(KEEPALIVE_PROBES)
                .setKeepAliveProbesInterval(KEEPALIVE_PROBES_INTERVAL)
                .setVhostName(VHOST_NAME)
                .setServerString(SERVER_STRING)
                .setLwsLogLevel(LWS_LOG_LEVEL)
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
                expected.ssl->privateKeyPassword = PASSWORD;
                expected.ssl->ciphersList = CIPHER_LIST;
                expected.ssl->ciphersListTls13 = CIPHER_LIST_TLS_13;
                expected.ssl->requireValidClientCert = true;
                expected.keepAliveTimeout = KEEPALIVE_TIMEOUT;
                expected.keepAliveProbes = KEEPALIVE_PROBES;
                expected.keepAliveProbesInterval = KEEPALIVE_PROBES_INTERVAL;
                expected.vhostName = VHOST_NAME;
                expected.serverString = SERVER_STRING;
                expected.lwsLogLevel = LWS_LOG_LEVEL;

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
        serverBuilder.setLwsLogLevel(LWS_LOG_LEVEL_DISABLE);

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

            AND_WHEN( "Dependent parameter 'keep alive probes' is not set" )
            {
                serverBuilder.setKeepAliveTimeout(KEEPALIVE_TIMEOUT);

                THEN( "Exception is thrown on server build" )
                {
                    REQUIRE_THROWS_WITH(serverBuilder.build(),
                                        "Required parameter is undefined: keep alive probes");
                }
            }

            AND_WHEN( "Dependent parameter 'keep alive probes interval' is not set" )
            {
                serverBuilder.setKeepAliveTimeout(KEEPALIVE_TIMEOUT);
                serverBuilder.setKeepAliveProbes(KEEPALIVE_PROBES);

                THEN( "Exception is thrown on server build" )
                {
                    REQUIRE_THROWS_WITH(serverBuilder.build(),
                                        "Required parameter is undefined: keep alive probes interval");
                }
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
