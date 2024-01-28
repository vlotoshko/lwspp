/*
 * lwspp - C++ wrapper for the libwebsockets library
 *
 * Copyright (C) 2023 - 2023 Volodymyr Lotoshko <vlotoshko@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include "ServerContext.hpp"
#include "SslSettings.hpp"
#include "lwspp/server/EventHandlerBase.hpp"
#include "lwspp/server/ServerBuilder.hpp"
#include "lwspp/server/SslSettingsBuilder.hpp"

namespace lwspp
{
namespace srv
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

} // namespace srv
} // namespace lwspp

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace lwspp
{
namespace tests
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

auto toString(CallbackVersion version) -> std::string
{
    switch (version)
    {
    case CallbackVersion::v1_Andromeda:
        return "v1_Andromeda";
    default:
        return "Undefined";
    }
}

void compareServerContexts(const ServerContext& actual, const ServerContext& expected)
{
    // Mandatory parameters
    REQUIRE(toString(actual.callbackVersion) == toString(expected.callbackVersion));
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
    REQUIRE(((actual.ssl != nullptr && expected.ssl != nullptr) ||
             (actual.ssl == nullptr && expected.ssl == nullptr)));

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
                .setCallbackVersion(CallbackVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(handler)
                .setActorAcceptor(handler)
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
                expected.callbackVersion = CallbackVersion::v1_Andromeda;
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
            serverBuilder.setCallbackVersion(CallbackVersion::v1_Andromeda);

            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: port");
            }
        }

        WHEN( "Event handler is not set" )
        {
            serverBuilder
                .setCallbackVersion(CallbackVersion::v1_Andromeda)
                .setPort(PORT);

            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: event handler");
            }
        }

        WHEN( "ActorAcceptor is not set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            serverBuilder
                .setCallbackVersion(CallbackVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(handler);

            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: data sender acceptor");
            }
        }

        WHEN( "All mandatory parameters are set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            serverBuilder
                .setCallbackVersion(CallbackVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(handler)
                .setActorAcceptor(handler);

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

} // namespace tests
} // namespace lwspp
// NOLINTEND (readability-function-cognitive-complexity)
