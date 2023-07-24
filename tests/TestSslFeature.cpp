/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#include <thread>

#include "catch2/catch.hpp"
#include "easywebsocket/client/ClientBuilder.hpp"
#include "easywebsocket/client/IEventHandler.hpp"
#include "easywebsocket/client/IMessageSenderAcceptor.hpp"
#include "easywebsocket/client/SslSettingsBuilder.hpp"

#include "easywebsocket/server/IEventHandler.hpp"
#include "easywebsocket/server/IMessageSenderAcceptor.hpp"
#include "easywebsocket/server/ServerBuilder.hpp"
#include "easywebsocket/server/SslSettingsBuilder.hpp"

#include "MockedPtr.hpp"
#include "Utils.hpp"


// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{

using namespace fakeit;

namespace
{

#include <string>

const srv::Port PORT = 9000;
const cli::Address ADDRESS = "localhost";

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
                         Mock<srv::IMessageSenderAcceptor>& messageSenderAcceptor,
                         srv::IMessageSenderPtr& messageSender)
{
    Fake(Method(eventHandler, onConnect), Method(eventHandler, onDisconnect));

    When(Method(messageSenderAcceptor, acceptMessageSender))
        .Do([&messageSender](srv::IMessageSenderPtr ms){ messageSender = ms; });
}

void setupClientBehavior(Mock<cli::IEventHandler>& eventHandler,
                         Mock<cli::IMessageSenderAcceptor>& messageSenderAcceptor,
                         cli::IMessageSenderPtr& messageSender)
{
    Fake(Method(eventHandler, onConnect), Method(eventHandler, onError),
         Method(eventHandler, onDisconnect));

    When(Method(eventHandler, onError))
        .Do([](const std::string message){ std::cout << message << std::endl; });
    When(Method(messageSenderAcceptor, acceptMessageSender))
        .Do([&messageSender](cli::IMessageSenderPtr ms){ messageSender = ms; });
}

srv::ServerBuilder setupServerBuilder(srv::IEventHandlerPtr eventHandler,
                                      srv::IMessageSenderAcceptorPtr messageSenderAcceptor)
{
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setVersion(srv::ServerVersion::v1_Andromeda)
        .setPort(PORT)
        .setEventHandler(eventHandler)
        .setMessageSenderAcceptor(messageSenderAcceptor);

    return serverBuilder;
}

cli::ClientBuilder setupClientBuilder(cli::IEventHandlerPtr eventHandler,
                                      cli::IMessageSenderAcceptorPtr messageSenderAcceptor)
{
    auto builder = cli::ClientBuilder{};
    builder
        .setVersion(cli::ClientVersion::v1_Amsterdam)
        .setAddress(ADDRESS)
        .setPort(PORT)
        .setEventHandler(eventHandler)
        .setMessageSenderAcceptor(messageSenderAcceptor);

    return builder;
}

} // namespace

SCENARIO( "Test ssl enabling for server and client", "[ssl]" )
{
    auto srvEventHadler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHadler = MockedPtr<cli::IEventHandler>{};

    auto srvMessageSenderAcceptor = MockedPtr<srv::IMessageSenderAcceptor>{};
    auto cliMessageSenderAcceptor = MockedPtr<cli::IMessageSenderAcceptor>{};

    srv::IMessageSenderPtr srvMessageSender;
    cli::IMessageSenderPtr cliMessageSender;

    setupServerBehavior(srvEventHadler.mock(), srvMessageSenderAcceptor.mock(), srvMessageSender);
    setupClientBehavior(cliEventHadler.mock(), cliMessageSenderAcceptor.mock(), cliMessageSender);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = setupServerBuilder(srvEventHadler.ptr(), srvMessageSenderAcceptor.ptr());
        auto clientBuilder = setupClientBuilder(cliEventHadler.ptr(), cliMessageSenderAcceptor.ptr());

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

                    Verify(Method(cliEventHadler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(srvEventHadler.mock(), onConnect),
                           Method(srvEventHadler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHadler.mock(), onConnect),
                           Method(cliEventHadler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(cliEventHadler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
                }
            }
        }
    } // GIVEN
} // SCENARIO

SCENARIO( "Test client ssl features", "[ssl]" )
{
    auto srvEventHadler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHadler = MockedPtr<cli::IEventHandler>{};

    auto srvMessageSenderAcceptor = MockedPtr<srv::IMessageSenderAcceptor>{};
    auto cliMessageSenderAcceptor = MockedPtr<cli::IMessageSenderAcceptor>{};

    srv::IMessageSenderPtr srvMessageSender;
    cli::IMessageSenderPtr cliMessageSender;

    setupServerBehavior(srvEventHadler.mock(), srvMessageSenderAcceptor.mock(), srvMessageSender);
    setupClientBehavior(cliEventHadler.mock(), cliMessageSenderAcceptor.mock(), cliMessageSender);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = setupServerBuilder(srvEventHadler.ptr(), srvMessageSenderAcceptor.ptr());
        auto clientBuilder = setupClientBuilder(cliEventHadler.ptr(), cliMessageSenderAcceptor.ptr());

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

                    Verify(Method(cliEventHadler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(srvEventHadler.mock(), onConnect),
                           Method(srvEventHadler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHadler.mock(), onConnect),
                           Method(cliEventHadler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(cliEventHadler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(srvEventHadler.mock(), onConnect),
                           Method(srvEventHadler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHadler.mock(), onConnect),
                           Method(cliEventHadler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(cliEventHadler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(srvEventHadler.mock(), onConnect),
                           Method(srvEventHadler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHadler.mock(), onConnect),
                           Method(cliEventHadler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(cliEventHadler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(srvEventHadler.mock(), onConnect),
                           Method(srvEventHadler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHadler.mock(), onConnect),
                           Method(cliEventHadler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(srvEventHadler.mock(), onConnect),
                           Method(srvEventHadler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHadler.mock(), onConnect),
                           Method(cliEventHadler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
                }
            }
        }
    } // GIVEN
} // SCENARIO

SCENARIO( "Test server ssl features", "[ssl]" )
{
    auto srvEventHadler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHadler = MockedPtr<cli::IEventHandler>{};

    auto srvMessageSenderAcceptor = MockedPtr<srv::IMessageSenderAcceptor>{};
    auto cliMessageSenderAcceptor = MockedPtr<cli::IMessageSenderAcceptor>{};

    srv::IMessageSenderPtr srvMessageSender;
    cli::IMessageSenderPtr cliMessageSender;

    setupServerBehavior(srvEventHadler.mock(), srvMessageSenderAcceptor.mock(), srvMessageSender);
    setupClientBehavior(cliEventHadler.mock(), cliMessageSenderAcceptor.mock(), cliMessageSender);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = setupServerBuilder(srvEventHadler.ptr(), srvMessageSenderAcceptor.ptr());
        auto clientBuilder = setupClientBuilder(cliEventHadler.ptr(), cliMessageSenderAcceptor.ptr());

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

                    Verify(Method(cliEventHadler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(srvEventHadler.mock(), onConnect),
                           Method(srvEventHadler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHadler.mock(), onConnect),
                           Method(cliEventHadler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(srvEventHadler.mock(), onConnect),
                           Method(srvEventHadler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHadler.mock(), onConnect),
                           Method(cliEventHadler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
                }
            }
        }
    } // GIVEN
} // SCENARIO


// FIXME: Tests with ciphers lists do not work, server and client establish connect even
// they have no common ciphers set, the server's cipher is chosen
SCENARIO( "Test ssl ciphers list feature", "[.ssl]" )
{
    auto srvEventHadler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHadler = MockedPtr<cli::IEventHandler>{};

    auto srvMessageSenderAcceptor = MockedPtr<srv::IMessageSenderAcceptor>{};
    auto cliMessageSenderAcceptor = MockedPtr<cli::IMessageSenderAcceptor>{};

    srv::IMessageSenderPtr srvMessageSender;
    cli::IMessageSenderPtr cliMessageSender;

    setupServerBehavior(srvEventHadler.mock(), srvMessageSenderAcceptor.mock(), srvMessageSender);
    setupClientBehavior(cliEventHadler.mock(), cliMessageSenderAcceptor.mock(), cliMessageSender);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = setupServerBuilder(srvEventHadler.ptr(), srvMessageSenderAcceptor.ptr());
        auto clientBuilder = setupClientBuilder(cliEventHadler.ptr(), cliMessageSenderAcceptor.ptr());

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

                Verify(Method(srvEventHadler.mock(), onConnect),
                       Method(srvEventHadler.mock(), onDisconnect)).Once();
                Verify(Method(cliEventHadler.mock(), onConnect),
                       Method(cliEventHadler.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(srvEventHadler.mock());
                VerifyNoOtherInvocations(cliEventHadler.mock());
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

                Verify(Method(cliEventHadler.mock(), onError)).Once();
                VerifyNoOtherInvocations(srvEventHadler.mock());
                VerifyNoOtherInvocations(cliEventHadler.mock());
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
