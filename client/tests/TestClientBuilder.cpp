/*
 * EasyWebsockets - C++ wrapper for the libwebsockets library
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

#include "catch2/catch.hpp"

#include "easywebsockets/client/ClientBuilder.hpp"
#include "easywebsockets/client/EventHandlerBase.hpp"
#include "easywebsockets/client/SslSettingsBuilder.hpp"
#include "ClientContext.hpp"
#include "SslSettings.hpp"

namespace ews::cli
{

class TestClientBuilder
{
public:
    explicit TestClientBuilder(const ClientBuilder& clientBuidler)
        : _clientBuilder(clientBuidler)
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

namespace
{

const Port PORT = 9000;
const Address ADDRESS = "localhost";
const std::string PROTOCOL_NAME = "PROTOCOL_NAME";
const Path PATH = "PATH";

const std::string CA_CERT_PATH     = "CA_CERT_PATH";
const std::string CLIENT_CERT_PATH = "CLIENT_CERT_PATH";
const std::string CLIENT_KEY_PATH  = "CLIENT_KEY_PATH";
const std::string PASSWORD  = "PASSWORD";
const std::string CIPHER_LIST  = "CIPHER_LIST";
const std::string CIPHER_LIST_TLS_13  = "CIPHER_LIST_TLS_13";

const int KEEPALIVE_TIMEOUT = 20;
const int KEEPALIVE_PROBES = 5;
const int KEEPALIVE_PROBES_INTERVAL = 10;
const int LWS_LOG_LEVEL = 9;
const int LWS_LOG_LEVEL_DISABLE = 0;

auto toString(CallbackVersion version) -> std::string
{
    switch (version)
    {
    case CallbackVersion::v1_Amsterdam:
        return "v1_Andromeda";
    default:
        return "Undefined";
    }
}

void compareClientContexts(const ClientContext& actual, const ClientContext& expected)
{
    // Mandatory parameters
    REQUIRE(toString(actual.callbackVersion) == toString(expected.callbackVersion));
    REQUIRE(actual.eventHandler == expected.eventHandler);
    REQUIRE(actual.address == expected.address);
    REQUIRE(actual.port == expected.port);

    // Non-mandatory parameters
    REQUIRE(actual.protocolName == expected.protocolName);
    REQUIRE(actual.path == expected.path);
    REQUIRE(actual.keepAliveTimeout == expected.keepAliveTimeout);
    REQUIRE(actual.keepAliveProbes == expected.keepAliveProbes);
    REQUIRE(actual.keepAliveProbesInterval == expected.keepAliveProbesInterval);
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

        REQUIRE(actual.ssl->allowSelfSignedServerCert == expected.ssl->allowSelfSignedServerCert);
        REQUIRE(actual.ssl->allowExpiredServerCert == expected.ssl->allowExpiredServerCert);
        REQUIRE(actual.ssl->skipServerCertHostnameCheck == expected.ssl->skipServerCertHostnameCheck);
        REQUIRE(actual.ssl->ignoreServerCaSert == expected.ssl->ignoreServerCaSert);
    }
}

} // namespace

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
                                   .setPrivateKeyPassword(PASSWORD)
                                   .setCiphersList(CIPHER_LIST)
                                   .setCiphersListTls13(CIPHER_LIST_TLS_13)
                                   .allowSelfSignedServerCert()
                                   .allowExpiredServerCert()
                                   .skipServerCertHostnameCheck()
                                   .ignoreServerCaSert()
                                   .build();
            clientBuilder
                .setCallbackVersion(CallbackVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT)
                .setEventHandler(handler)
                .setDataSenderAcceptor(handler)
                .setProtocolName(PROTOCOL_NAME)
                .setPath(PATH)
                .setKeepAliveTimeout(KEEPALIVE_TIMEOUT)
                .setKeepAliveProbes(KEEPALIVE_PROBES)
                .setKeepAliveProbesInterval(KEEPALIVE_PROBES_INTERVAL)
                .setLwsLogLevel(LWS_LOG_LEVEL)
                .setSslSettings(sslSettings);

            const ClientContext& actual = TestClientBuilder{clientBuilder}.getClientContext();

            THEN( "Client context has correct data" )
            {
                ClientContext expected;
                expected.callbackVersion = CallbackVersion::v1_Amsterdam;
                expected.address = ADDRESS;
                expected.port = PORT;
                expected.eventHandler = handler;
                expected.protocolName = PROTOCOL_NAME;
                expected.path = PATH;
                expected.ssl = std::make_shared<SslSettings>();
                expected.ssl->privateKeyPath = CLIENT_KEY_PATH;
                expected.ssl->certPath = CLIENT_CERT_PATH;
                expected.ssl->caCertPath = CA_CERT_PATH;
                expected.ssl->privateKeyPassword = PASSWORD;
                expected.ssl->ciphersList = CIPHER_LIST;
                expected.ssl->ciphersListTls13 = CIPHER_LIST_TLS_13;
                expected.ssl->allowSelfSignedServerCert = true;
                expected.ssl->allowExpiredServerCert = true;
                expected.ssl->skipServerCertHostnameCheck = true;
                expected.ssl->ignoreServerCaSert = true;
                expected.keepAliveTimeout = KEEPALIVE_TIMEOUT;
                expected.keepAliveProbes = KEEPALIVE_PROBES;
                expected.keepAliveProbesInterval = KEEPALIVE_PROBES_INTERVAL;
                expected.lwsLogLevel = LWS_LOG_LEVEL;

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
        clientBuilder.setLwsLogLevel(LWS_LOG_LEVEL_DISABLE);

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
                .setCallbackVersion(CallbackVersion::v1_Amsterdam)
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
                .setCallbackVersion(CallbackVersion::v1_Amsterdam)
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
                .setCallbackVersion(CallbackVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT);

            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: event handler");
            }
        }

        WHEN( "DataSenderAcceptor is not set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            clientBuilder
                .setCallbackVersion(CallbackVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT)
                .setEventHandler(handler);

            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: data sender acceptor");
            }
        }

        WHEN( "All mandatory parameters are set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            clientBuilder
                .setCallbackVersion(CallbackVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT)
                .setEventHandler(handler)
                .setDataSenderAcceptor(handler);

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

            AND_WHEN( "Dependent parameter 'keep alive probes' is not set" )
            {
                clientBuilder.setKeepAliveTimeout(KEEPALIVE_TIMEOUT);

                THEN( "Exception is thrown on client build" )
                {
                    REQUIRE_THROWS_WITH(clientBuilder.build(),
                                        "Required parameter is undefined: keep alive probes");
                }
            }

            AND_WHEN( "Dependent parameter 'keep alive probes interval' is not set" )
            {
                clientBuilder.setKeepAliveTimeout(KEEPALIVE_TIMEOUT);
                clientBuilder.setKeepAliveProbes(KEEPALIVE_PROBES);

                THEN( "Exception is thrown on client build" )
                {
                    REQUIRE_THROWS_WITH(clientBuilder.build(),
                                        "Required parameter is undefined: keep alive probes interval");
                }
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
