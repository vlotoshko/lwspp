/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <thread>

#include "catch2/catch.hpp"
#include "MockedPtr.hpp"

#include "easywebsocket/client/ClientBuilder.hpp"
#include "easywebsocket/client/IEventHandler.hpp"
#include "easywebsocket/client/IMessageSenderAcceptor.hpp"

#include "easywebsocket/server/IEventHandler.hpp"
#include "easywebsocket/server/IMessageSenderAcceptor.hpp"
#include "easywebsocket/server/ISessionInfo.hpp"
#include "easywebsocket/server/ServerBuilder.hpp"

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{
namespace
{

const srv::Port PORT = 9000;
const cli::Address ADDRESS = "localhost";
const std::string CUSTOM_PROTOCOL_NAME = "CUSTOM_PROTOCOL_NAME";
const std::string CUSTOM_PROTOCOL_NAME_2 = "CUSTOM_PROTOCOL_NAME_2";

const std::string SPECIFIC_PATH = "SPECIFIC_PATH";
const std::string SPECIFIC_PATH_2 = "SPECIFIC_PATH_2";

void waitForInitialization()
{
    const auto timeout = 100U;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
}

void setupServerBehavior(fakeit::Mock<srv::IEventHandler>& eventHandler,
                         fakeit::Mock<srv::IMessageSenderAcceptor>& messageSenderAcceptor,
                         srv::IMessageSenderPtr& messageSender)
{
    fakeit::Fake(Method(eventHandler, onConnect), Method(eventHandler, onDisconnect));

    fakeit::When(Method(messageSenderAcceptor, acceptMessageSender))
        .Do([&messageSender](srv::IMessageSenderPtr ms){ messageSender = ms; });
}

void setupClientBehavior(fakeit::Mock<cli::IEventHandler>& eventHandler,
                         fakeit::Mock<cli::IMessageSenderAcceptor>& messageSenderAcceptor,
                         cli::IMessageSenderPtr& messageSender)
{
    fakeit::Fake(Method(eventHandler, onConnect), Method(eventHandler, onError),
                 Method(eventHandler, onDisconnect));

    fakeit::When(Method(messageSenderAcceptor, acceptMessageSender))
        .Do([&messageSender](cli::IMessageSenderPtr ms){ messageSender = ms; });
}

} // namespace


SCENARIO( "Protocol name feature testing", "[protocol_name]" )
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
        auto serverBuilder = srv::ServerBuilder{};
        serverBuilder
            .setVersion(srv::ServerVersion::v1_Andromeda)
            .setPort(PORT)
            .setEventHandler(srvEventHadler.ptr())
            .setMessageSenderAcceptor(srvMessageSenderAcceptor.ptr());

        auto clientBuilder = cli::ClientBuilder{};
        clientBuilder
            .setVersion(cli::ClientVersion::v1_Amsterdam)
            .setAddress(ADDRESS)
            .setPort(PORT)
            .setEventHandler(cliEventHadler.ptr())
            .setMessageSenderAcceptor(cliMessageSenderAcceptor.ptr());

        WHEN( "Server uses default protocol name" )
        {
            auto server = serverBuilder.build();

            AND_WHEN("Client uses default protocol name")
            {
                auto client = clientBuilder.build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    fakeit::Verify(Method(srvEventHadler.mock(), onConnect),
                                   Method(srvEventHadler.mock(), onDisconnect)
                                   ).Once();

                    fakeit::Verify(Method(cliEventHadler.mock(), onConnect),
                                   Method(cliEventHadler.mock(), onDisconnect)
                                   ).Once();
                }
            }

            AND_WHEN("Client uses custom protocol name")
            {
                auto client = clientBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME).build();

                THEN( "Client cann't connect to the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    fakeit::VerifyNoOtherInvocations(srvEventHadler.mock());
                    fakeit::Verify(Method(cliEventHadler.mock(), onError)).Once();
                }
            }
        }

        WHEN( "Server uses custom protocol name" )
        {
            auto server = serverBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME).build();

            AND_WHEN("Client uses default protocol name")
            {
                auto client = clientBuilder.build();
                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    fakeit::Verify(Method(srvEventHadler.mock(), onConnect),
                                   Method(srvEventHadler.mock(), onDisconnect)).Once();

                    fakeit::Verify(Method(cliEventHadler.mock(), onConnect),
                                   Method(cliEventHadler.mock(), onDisconnect)).Once();

                    fakeit::VerifyNoOtherInvocations(srvEventHadler.mock());
                    fakeit::VerifyNoOtherInvocations(cliEventHadler.mock());
                }
            }

            AND_WHEN("Client uses a custom protocol name that differs from the servers protocol name")
            {
                auto client = clientBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME_2).build();
                THEN( "Client cann't connect to the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    fakeit::Verify(Method(cliEventHadler.mock(), onError)).Once();
                    fakeit::VerifyNoOtherInvocations(srvEventHadler.mock());
                    fakeit::VerifyNoOtherInvocations(cliEventHadler.mock());
                }
            }

            AND_WHEN("Client uses a custom protocol name that equals the servers protocol name")
            {
                clientBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME);
                auto client = clientBuilder.build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    fakeit::Verify(Method(srvEventHadler.mock(), onConnect),
                                   Method(srvEventHadler.mock(), onDisconnect)).Once();

                    fakeit::Verify(Method(cliEventHadler.mock(), onConnect),
                                   Method(cliEventHadler.mock(), onDisconnect)).Once();

                    fakeit::VerifyNoOtherInvocations(srvEventHadler.mock());
                    fakeit::VerifyNoOtherInvocations(cliEventHadler.mock());
                }
            }
        }
    } // GIVEN
} // SCENARIO

SCENARIO( "Path feature testing", "[path]" )
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
        auto serverBuilder = srv::ServerBuilder{};
        serverBuilder
            .setVersion(srv::ServerVersion::v1_Andromeda)
            .setPort(PORT)
            .setEventHandler(srvEventHadler.ptr())
            .setMessageSenderAcceptor(srvMessageSenderAcceptor.ptr());

        auto clientBuilder = cli::ClientBuilder{};
        clientBuilder
            .setVersion(cli::ClientVersion::v1_Amsterdam)
            .setAddress(ADDRESS)
            .setPort(PORT)
            .setEventHandler(cliEventHadler.ptr())
            .setMessageSenderAcceptor(cliMessageSenderAcceptor.ptr());

        bool actualUseSpecificBehaviour = false;
        auto onConnect = [&](srv::ISessionInfoPtr sessionInfo)
        {
            if (sessionInfo != nullptr && sessionInfo->getPath() == SPECIFIC_PATH)
            {
                actualUseSpecificBehaviour = true;
            }
        };

        fakeit::When(Method(srvEventHadler.mock(), onConnect)).Do(onConnect);

        WHEN( "Client uses default uri path" )
        {
            auto server = serverBuilder.build();
            auto client = clientBuilder.build();

            THEN("Server uses default behavoiur")
            {
                waitForInitialization();
                server.reset();
                client.reset();

                fakeit::Verify(Method(srvEventHadler.mock(), onConnect),
                               Method(srvEventHadler.mock(), onDisconnect)).Once();

                fakeit::Verify(Method(cliEventHadler.mock(), onConnect),
                               Method(cliEventHadler.mock(), onDisconnect)).Once();

                fakeit::VerifyNoOtherInvocations(srvEventHadler.mock());
                fakeit::VerifyNoOtherInvocations(cliEventHadler.mock());

                const bool expectedUseSpecificBehaviour = false;
                REQUIRE(actualUseSpecificBehaviour == expectedUseSpecificBehaviour);
            }
        }

        WHEN( "Client uses specific uri path" )
        {
            auto server = serverBuilder.build();
            clientBuilder.setPath(SPECIFIC_PATH);
            auto client = clientBuilder.build();

            THEN("Server uses specific behavoiur")
            {
                waitForInitialization();
                server.reset();
                client.reset();

                fakeit::Verify(Method(srvEventHadler.mock(), onConnect),
                               Method(srvEventHadler.mock(), onDisconnect)).Once();

                fakeit::Verify(Method(cliEventHadler.mock(), onConnect),
                               Method(cliEventHadler.mock(), onDisconnect)).Once();

                fakeit::VerifyNoOtherInvocations(srvEventHadler.mock());
                fakeit::VerifyNoOtherInvocations(cliEventHadler.mock());

                const bool expectedUseSpecificBehaviour = true;
                REQUIRE(actualUseSpecificBehaviour == expectedUseSpecificBehaviour);
            }
        }

        WHEN( "Client uses specific uri path, but other than specified by server" )
        {
            auto server = serverBuilder.build();
            clientBuilder.setPath(SPECIFIC_PATH_2);
            auto client = clientBuilder.build();

            THEN("Server uses default behavoiur")
            {
                waitForInitialization();
                server.reset();
                client.reset();

                fakeit::Verify(Method(srvEventHadler.mock(), onConnect),
                               Method(srvEventHadler.mock(), onDisconnect)).Once();

                fakeit::Verify(Method(cliEventHadler.mock(), onConnect),
                               Method(cliEventHadler.mock(), onDisconnect)).Once();

                fakeit::VerifyNoOtherInvocations(srvEventHadler.mock());
                fakeit::VerifyNoOtherInvocations(cliEventHadler.mock());

                const bool expectedUseSpecificBehaviour = false;
                REQUIRE(actualUseSpecificBehaviour == expectedUseSpecificBehaviour);
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
