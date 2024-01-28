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
#include <thread>

#include "lwspp/client/ClientBuilder.hpp"
#include "lwspp/client/IActorAcceptor.hpp"
#include "lwspp/client/IEventHandler.hpp"
#include "lwspp/client/SslSettingsBuilder.hpp"

#include "lwspp/server/IActorAcceptor.hpp"
#include "lwspp/server/IEventHandler.hpp"
#include "lwspp/server/ServerBuilder.hpp"
#include "lwspp/server/SslSettingsBuilder.hpp"

#include "MockedPtr.hpp"
#include "Utils.hpp"


// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace lwspp
{
namespace tests
{

using namespace fakeit;

namespace
{

#include <string>

const srv::Port PORT = 9000;
const cli::Address ADDRESS = "localhost";
const int DISABLE_LOG = 0;

const std::string EXE_PATH         = getExecDirectory();
const std::string CA_CERT_PATH     = EXE_PATH + "TestData/ssl/rootCA.crt";
const std::string SERVER_CERT_PATH = EXE_PATH + "TestData/ssl/self_signed/server.crt";
const std::string SERVER_KEY_PATH  = EXE_PATH + "TestData/ssl/self_signed/server.key";
const std::string CLIENT_CERT_PATH = EXE_PATH + "TestData/ssl/self_signed/client.crt";
const std::string CLIENT_KEY_PATH  = EXE_PATH + "TestData/ssl/self_signed/client.key";

const std::string SERVER_KEY_ENCRYPTED_PATH = EXE_PATH + "TestData/ssl/encrypted/server.key";
const std::string SERVER_CERT_ENCRYPTED_PATH = EXE_PATH + "TestData/ssl/encrypted/server.crt";
const std::string CLIENT_KEY_ENCRYPTED_PATH = EXE_PATH + "TestData/ssl/encrypted/client.key";
const std::string CLIENT_CERT_ENCRYPTED_PATH = EXE_PATH + "TestData/ssl/encrypted/client.crt";
const std::string PASSWORD = "test";

const std::string SERVER_KEY_EXPIRED_PATH = EXE_PATH + "TestData/ssl/expired/server.key";
const std::string SERVER_CERT_EXPIRED_PATH = EXE_PATH + "TestData/ssl/expired/server.crt";

const std::string SERVER_KEY_HOSTNAME_PATH = EXE_PATH + "TestData/ssl/wrong_hostname/server.key";
const std::string SERVER_CERT_HOSTNAME_PATH = EXE_PATH + "TestData/ssl/wrong_hostname/server.crt";

const std::string CIPHERS_LIST_TLS_13_1 = "TLS_CHACHA20_POLY1305_SHA256";
const std::string CIPHERS_LIST_TLS_13_2 = "TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256";
const std::string CIPHERS_LIST_TLS_13_3 = "TLS_AES_128_GCM_SHA256:TLS_AES_128_CCM_8_SHA256";

void waitForInitialization()
{
    const auto timeout = 100U;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
}

void setupServerBehavior(Mock<srv::IEventHandler>& eventHandler,
                         Mock<srv::IActorAcceptor>& actorAcceptor,
                         srv::IActorPtr& actor)
{
    Fake(Method(eventHandler, onConnect), Method(eventHandler, onDisconnect));

    When(Method(actorAcceptor, acceptActor))
        .Do([&actor](srv::IActorPtr a){ actor = a; });
}

void setupClientBehavior(Mock<cli::IEventHandler>& eventHandler,
                         Mock<cli::IActorAcceptor>& actorAcceptor,
                         cli::IActorPtr& actor)
{
    Fake(Method(eventHandler, onConnect), Method(eventHandler, onError),
         Method(eventHandler, onDisconnect));

    When(Method(eventHandler, onError))
        .Do([](const std::string message){ std::cout << message << std::endl; });
    When(Method(actorAcceptor, acceptActor))
        .Do([&actor](cli::IActorPtr a){ actor = a; });
}

srv::ServerBuilder setupServerBuilder(srv::IEventHandlerPtr eventHandler,
                                      srv::IActorAcceptorPtr actorAcceptor)
{
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setPort(PORT)
        .setEventHandler(eventHandler)
        .setActorAcceptor(actorAcceptor)
        .setLwsLogLevel(DISABLE_LOG);

    return serverBuilder;
}

cli::ClientBuilder setupClientBuilder(cli::IEventHandlerPtr eventHandler,
                                      cli::IActorAcceptorPtr actorAcceptor)
{
    auto builder = cli::ClientBuilder{};
    builder
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setAddress(ADDRESS)
        .setPort(PORT)
        .setEventHandler(eventHandler)
        .setActorAcceptor(actorAcceptor)
        .setLwsLogLevel(DISABLE_LOG);

    return builder;
}

} // namespace

SCENARIO( "Test ssl enabling for server and client", "[ssl]" )
{
    auto srvEventHandler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHandler = MockedPtr<cli::IEventHandler>{};
    
    auto srvActorAcceptor = MockedPtr<srv::IActorAcceptor>{};
    auto cliActorAcceptor = MockedPtr<cli::IActorAcceptor>{};

    srv::IActorPtr srvActor;
    cli::IActorPtr cliActor;

    setupServerBehavior(srvEventHandler.mock(), srvActorAcceptor.mock(), srvActor);
    setupClientBehavior(cliEventHandler.mock(), cliActorAcceptor.mock(), cliActor);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = setupServerBuilder(srvEventHandler.ptr(), srvActorAcceptor.ptr());
        auto clientBuilder = setupClientBuilder(cliEventHandler.ptr(), cliActorAcceptor.ptr());

        WHEN( "Server uses ssl" )
        {
            auto srvSslSettings =
                srv::SslSettingsBuilder{}
                    .setPrivateKeyFilepath(SERVER_KEY_PATH)
                    .setCertFilepath(SERVER_CERT_PATH)
                    .setCaCertFilepath(CA_CERT_PATH)
                    .build();
            auto server = serverBuilder.setSslSettings(srvSslSettings).build();

            AND_WHEN( "Client does not use ssl" )
            {
                auto client = clientBuilder.build();

                THEN( "Client cannot connect the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(cliEventHandler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }

            AND_WHEN( "Client uses ssl also" )
            {
                auto sslSettings =
                    cli::SslSettingsBuilder{}
                        // server uses self signed cert, had to allow it
                        .allowSelfSignedServerCert()
                        .build();

                auto client = clientBuilder.setSslSettings(sslSettings).build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvEventHandler.mock(), onConnect),
                           Method(srvEventHandler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHandler.mock(), onConnect),
                           Method(cliEventHandler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }
        }

        WHEN( "Server does not use ssl" )
        {
            auto server = serverBuilder.build();

            AND_WHEN( "Client uses ssl" )
            {
                auto client = clientBuilder
                                  .setSslSettings(cli::SslSettingsBuilder{}.build())
                                  .build();

                THEN( "Client cannot connect to the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(cliEventHandler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }
        }
    } // GIVEN
} // SCENARIO

SCENARIO( "Test client ssl features", "[ssl]" )
{
    auto srvEventHandler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHandler = MockedPtr<cli::IEventHandler>{};
    
    auto srvActorAcceptor = MockedPtr<srv::IActorAcceptor>{};
    auto cliActorAcceptor = MockedPtr<cli::IActorAcceptor>{};

    srv::IActorPtr srvActor;
    cli::IActorPtr cliActor;

    setupServerBehavior(srvEventHandler.mock(), srvActorAcceptor.mock(), srvActor);
    setupClientBehavior(cliEventHandler.mock(), cliActorAcceptor.mock(), cliActor);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = setupServerBuilder(srvEventHandler.ptr(), srvActorAcceptor.ptr());
        auto clientBuilder = setupClientBuilder(cliEventHandler.ptr(), cliActorAcceptor.ptr());

        WHEN( "Server uses self-signed certificate" )
        {
            auto srvSslSettings =
                srv::SslSettingsBuilder{}
                    .setPrivateKeyFilepath(SERVER_KEY_PATH)
                    .setCertFilepath(SERVER_CERT_PATH)
                    .setCaCertFilepath(CA_CERT_PATH)
                    .build();
            auto server = serverBuilder.setSslSettings(srvSslSettings).build();

            AND_WHEN( "Client does not allow self-signed certificates" )
            {
                auto sslSettings = cli::SslSettingsBuilder{}.build();
                auto client = clientBuilder.setSslSettings(sslSettings).build();

                THEN( "Client cannot connect the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(cliEventHandler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }

            AND_WHEN( "Client allows self-signed certificates" )
            {
                auto sslSettings = cli::SslSettingsBuilder{}
                                       .allowSelfSignedServerCert()
                                       .build();

                auto client = clientBuilder.setSslSettings(sslSettings).build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvEventHandler.mock(), onConnect),
                           Method(srvEventHandler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHandler.mock(), onConnect),
                           Method(cliEventHandler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }
        }

        WHEN( "Server uses expired certificate" )
        {
            auto srvSslSettings =
                srv::SslSettingsBuilder{}
                    .setPrivateKeyFilepath(SERVER_KEY_EXPIRED_PATH)
                    .setCertFilepath(SERVER_CERT_EXPIRED_PATH)
                    .setCaCertFilepath(CA_CERT_PATH)
                    .build();
            auto server = serverBuilder.setSslSettings(srvSslSettings).build();


            cli::SslSettingsBuilder cliSslSettingsBuilder;
            cliSslSettingsBuilder
                // have to allow self-signed certs because
                // server does not have cert signed by trusted CA
                .allowSelfSignedServerCert();

            AND_WHEN( "Client does not allow expired certificates" )
            {
                auto client = clientBuilder.setSslSettings(cliSslSettingsBuilder.build()).build();

                THEN( "Client cannot connect the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(cliEventHandler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }

            AND_WHEN( "Client allows expired certificates" )
            {
                cliSslSettingsBuilder.allowExpiredServerCert();
                auto client = clientBuilder.setSslSettings(cliSslSettingsBuilder.build()).build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvEventHandler.mock(), onConnect),
                           Method(srvEventHandler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHandler.mock(), onConnect),
                           Method(cliEventHandler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }
        }

        WHEN( "Server uses certificate with the wrong hostname" )
        {
            auto srvSslSettings =
                srv::SslSettingsBuilder{}
                    .setPrivateKeyFilepath(SERVER_KEY_HOSTNAME_PATH)
                    .setCertFilepath(SERVER_CERT_HOSTNAME_PATH)
                    .setCaCertFilepath(CA_CERT_PATH)
                    .build();
            auto server = serverBuilder.setSslSettings(srvSslSettings).build();

            cli::SslSettingsBuilder cliSslSettingsBuilder;
            cliSslSettingsBuilder
                // have to allow self-signed certs because
                // server does not have cert signed by trusted CA
                .allowSelfSignedServerCert();

            AND_WHEN( "Client does not skip hostname check" )
            {
                auto client = clientBuilder.setSslSettings(cliSslSettingsBuilder.build()).build();

                THEN( "Client cannot connect the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(cliEventHandler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }

            AND_WHEN( "Client skips hostname check" )
            {
                cliSslSettingsBuilder.skipServerCertHostnameCheck();
                auto client = clientBuilder.setSslSettings(cliSslSettingsBuilder.build()).build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvEventHandler.mock(), onConnect),
                           Method(srvEventHandler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHandler.mock(), onConnect),
                           Method(cliEventHandler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }
        }

        WHEN( "Server does not provide the CA certificate" )
        {
            auto srvSslSettings =
                srv::SslSettingsBuilder{}
                    .setPrivateKeyFilepath(SERVER_KEY_PATH)
                    .setCertFilepath(SERVER_CERT_PATH)
                    .build();
            auto server = serverBuilder.setSslSettings(srvSslSettings).build();

            cli::SslSettingsBuilder cliSslSettingsBuilder;
            cliSslSettingsBuilder
                // have to allow self-signed certs because
                // server does not have cert signed by trusted CA
                .allowSelfSignedServerCert();

            AND_WHEN( "Client does not ingore the CA certificate from server" )
            {
                auto client = clientBuilder.setSslSettings(cliSslSettingsBuilder.build()).build();

                THEN( "Client cannot connect the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(cliEventHandler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }

            AND_WHEN( "Client ignores the CA certificate from server" )
            {
                cliSslSettingsBuilder.ignoreServerCaSert();
                auto client = clientBuilder.setSslSettings(cliSslSettingsBuilder.build()).build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvEventHandler.mock(), onConnect),
                           Method(srvEventHandler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHandler.mock(), onConnect),
                           Method(cliEventHandler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }
        }

        WHEN( "Client uses encrypted key" )
        {
            cli::SslSettingsBuilder cliSslSettingsBuilder;
            cliSslSettingsBuilder
                .setPrivateKeyFilepath(CLIENT_KEY_ENCRYPTED_PATH)
                .setCertFilepath(CLIENT_CERT_ENCRYPTED_PATH)
                .setCaCertFilepath(CA_CERT_PATH)
                // have to allow self-signed certs because
                // server does not have cert signed by trusted CA
                .allowSelfSignedServerCert();

            AND_WHEN( "Key password was not set" )
            {
                clientBuilder.setSslSettings(cliSslSettingsBuilder.build());

                THEN( "Exception is thrown on client build" )
                {
                    REQUIRE_THROWS_WITH(clientBuilder.build(), "lws_context initialization failed");
                }
            }

            AND_WHEN( "Key password was set" )
            {
                cliSslSettingsBuilder.setPrivateKeyPassword(PASSWORD);

                THEN( "Client connects to the server successfully" )
                {
                    auto srvSslSettings =
                        srv::SslSettingsBuilder{}
                            .setPrivateKeyFilepath(SERVER_KEY_PATH)
                            .setCertFilepath(SERVER_CERT_PATH)
                            .setCaCertFilepath(CA_CERT_PATH)
                            .build();
                    auto server = serverBuilder.setSslSettings(srvSslSettings).build();
                    auto client = clientBuilder.setSslSettings(cliSslSettingsBuilder.build()).build();

                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvEventHandler.mock(), onConnect),
                           Method(srvEventHandler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHandler.mock(), onConnect),
                           Method(cliEventHandler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }
        }
    } // GIVEN
} // SCENARIO

SCENARIO( "Test server ssl features", "[ssl]" )
{
    auto srvEventHandler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHandler = MockedPtr<cli::IEventHandler>{};
    
    auto srvActorAcceptor = MockedPtr<srv::IActorAcceptor>{};
    auto cliActorAcceptor = MockedPtr<cli::IActorAcceptor>{};

    srv::IActorPtr srvActor;
    cli::IActorPtr cliActor;

    setupServerBehavior(srvEventHandler.mock(), srvActorAcceptor.mock(), srvActor);
    setupClientBehavior(cliEventHandler.mock(), cliActorAcceptor.mock(), cliActor);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = setupServerBuilder(srvEventHandler.ptr(), srvActorAcceptor.ptr());
        auto clientBuilder = setupClientBuilder(cliEventHandler.ptr(), cliActorAcceptor.ptr());

        WHEN( "Server requires valid cert from client" )
        {
            auto srvSslSettings =
                srv::SslSettingsBuilder{}
                    .setPrivateKeyFilepath(SERVER_KEY_PATH)
                    .setCertFilepath(SERVER_CERT_PATH)
                    .setCaCertFilepath(CA_CERT_PATH)
                    .build();
            auto server = serverBuilder.setSslSettings(srvSslSettings).build();

            AND_WHEN( "Client does not provide its certs" )
            {
                auto sslSettings = cli::SslSettingsBuilder{}.build();
                auto client = clientBuilder.setSslSettings(sslSettings).build();

                THEN( "Client cannot connect the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(cliEventHandler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }

            AND_WHEN( "Client provides its certs" )
            {
                auto sslSettings = cli::SslSettingsBuilder{}
                                       .setPrivateKeyFilepath(CLIENT_KEY_PATH)
                                       .setCertFilepath(CLIENT_CERT_PATH)
                                       .setCaCertFilepath(CA_CERT_PATH)
                                       .build();

                auto client = clientBuilder.setSslSettings(sslSettings).build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvEventHandler.mock(), onConnect),
                           Method(srvEventHandler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHandler.mock(), onConnect),
                           Method(cliEventHandler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }
        }

        WHEN( "Server uses encrypted key" )
        {
            srv::SslSettingsBuilder srvSslSettingsBuilder;
            srvSslSettingsBuilder
                .setPrivateKeyFilepath(SERVER_KEY_ENCRYPTED_PATH)
                .setCertFilepath(SERVER_CERT_ENCRYPTED_PATH)
                .setCaCertFilepath(CA_CERT_PATH);

            AND_WHEN( "Key password was not set" )
            {
                serverBuilder.setSslSettings(srvSslSettingsBuilder.build());

                THEN( "Exception is thrown on server build" )
                {
                    REQUIRE_THROWS_WITH(serverBuilder.build(), "lws_context initialization failed");
                }
            }

            AND_WHEN( "Key password was set" )
            {
                srvSslSettingsBuilder.setPrivateKeyPassword(PASSWORD);
                auto server = serverBuilder.setSslSettings(srvSslSettingsBuilder.build()).build();

                auto sslSettings =
                    cli::SslSettingsBuilder{}
                        // have to allow self-signed certs because
                        // server does not have cert signed by trusted CA
                        .allowSelfSignedServerCert()
                        .build();
                auto client = clientBuilder.setSslSettings(sslSettings).build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvEventHandler.mock(), onConnect),
                           Method(srvEventHandler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHandler.mock(), onConnect),
                           Method(cliEventHandler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHandler.mock());
                    VerifyNoOtherInvocations(cliEventHandler.mock());
                }
            }
        }
    } // GIVEN
} // SCENARIO


// FIXME: Tests with ciphers lists do not work, server and client establish connect even
// they have no common ciphers set, the server's cipher is chosen
SCENARIO( "Test ssl ciphers list feature", "[.ssl]" )
{
    auto srvEventHandler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHandler = MockedPtr<cli::IEventHandler>{};
    
    auto srvActorAcceptor = MockedPtr<srv::IActorAcceptor>{};
    auto cliActorAcceptor = MockedPtr<cli::IActorAcceptor>{};

    srv::IActorPtr srvActor;
    cli::IActorPtr cliActor;

    setupServerBehavior(srvEventHandler.mock(), srvActorAcceptor.mock(), srvActor);
    setupClientBehavior(cliEventHandler.mock(), cliActorAcceptor.mock(), cliActor);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = setupServerBuilder(srvEventHandler.ptr(), srvActorAcceptor.ptr());
        auto clientBuilder = setupClientBuilder(cliEventHandler.ptr(), cliActorAcceptor.ptr());

        srv::SslSettingsBuilder srvSslSettingsBuilder;
        srvSslSettingsBuilder
            .setPrivateKeyFilepath(SERVER_KEY_PATH)
            .setCertFilepath(SERVER_CERT_PATH)
            .setCaCertFilepath(CA_CERT_PATH);

        cli::SslSettingsBuilder cliSslSettingsBuilder;
        cliSslSettingsBuilder
            .setPrivateKeyFilepath(CLIENT_KEY_PATH)
            .setCertFilepath(CLIENT_CERT_PATH)
            .setCaCertFilepath(CA_CERT_PATH)
            // have to allow self-signed certs because
            // server does not have cert signed by trusted CA
            .allowSelfSignedServerCert();

        WHEN( "Server and client use the ciphers lists that have common ciphers" )
        {
            srvSslSettingsBuilder.setCiphersListTls13(CIPHERS_LIST_TLS_13_1);
            cliSslSettingsBuilder.setCiphersListTls13(CIPHERS_LIST_TLS_13_2);

            auto server = serverBuilder.setSslSettings(srvSslSettingsBuilder.build()).build();
            auto client = clientBuilder.setSslSettings(cliSslSettingsBuilder.build()).build();

            THEN( "Client connects to the server successfully" )
            {
                waitForInitialization();
                server.reset();
                client.reset();

                Verify(Method(srvEventHandler.mock(), onConnect),
                       Method(srvEventHandler.mock(), onDisconnect)).Once();
                Verify(Method(cliEventHandler.mock(), onConnect),
                       Method(cliEventHandler.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(srvEventHandler.mock());
                VerifyNoOtherInvocations(cliEventHandler.mock());
            }
        }

        WHEN( "Server and client use the ciphers lists that do not have common ciphers" )
        {
            srvSslSettingsBuilder.setCiphersListTls13(CIPHERS_LIST_TLS_13_2);
            cliSslSettingsBuilder.setCiphersListTls13(CIPHERS_LIST_TLS_13_3);

            auto server = serverBuilder.setSslSettings(srvSslSettingsBuilder.build()).build();
            auto client = clientBuilder.setSslSettings(cliSslSettingsBuilder.build()).build();

            THEN( "Client cannot connect the server" )
            {
                waitForInitialization();
                server.reset();
                client.reset();

                Verify(Method(cliEventHandler.mock(), onError)).Once();
                VerifyNoOtherInvocations(srvEventHandler.mock());
                VerifyNoOtherInvocations(cliEventHandler.mock());
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace tests
} // namespace lwspp
// NOLINTEND (readability-function-cognitive-complexity)
