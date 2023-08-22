/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <condition_variable>
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

using namespace fakeit;

namespace
{

const srv::Port PORT = 9000;
const cli::Address ADDRESS = "localhost";
const int DISABLE_LOG = 0;

const std::string CUSTOM_PROTOCOL_NAME = "CUSTOM_PROTOCOL_NAME";
const std::string CUSTOM_PROTOCOL_NAME_2 = "CUSTOM_PROTOCOL_NAME_2";

const std::string SPECIFIC_PATH = "/SPECIFIC_PATH";
const std::string SPECIFIC_PATH_2 = "/SPECIFIC_PATH_2";

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

    When(Method(messageSenderAcceptor, acceptMessageSender))
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
            .setMessageSenderAcceptor(srvMessageSenderAcceptor.ptr())
            .setLwsLogLevel(DISABLE_LOG);

        auto clientBuilder = cli::ClientBuilder{};
        clientBuilder
            .setVersion(cli::ClientVersion::v1_Amsterdam)
            .setAddress(ADDRESS)
            .setPort(PORT)
            .setEventHandler(cliEventHadler.ptr())
            .setMessageSenderAcceptor(cliMessageSenderAcceptor.ptr())
            .setLwsLogLevel(DISABLE_LOG);

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

                    Verify(Method(srvEventHadler.mock(), onConnect),
                           Method(srvEventHadler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHadler.mock(), onConnect),
                           Method(cliEventHadler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(cliEventHadler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(srvEventHadler.mock(), onConnect),
                           Method(srvEventHadler.mock(), onDisconnect)).Once();
                    Verify(Method(cliEventHadler.mock(), onConnect),
                           Method(cliEventHadler.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

                    Verify(Method(cliEventHadler.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvEventHadler.mock());
                    VerifyNoOtherInvocations(cliEventHadler.mock());
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

        When(Method(srvEventHadler.mock(), onConnect)).Do(onConnect);

        WHEN( "Client uses default uri path" )
        {
            auto server = serverBuilder.build();
            auto client = clientBuilder.build();

            THEN("Server uses default behavoiur")
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

                Verify(Method(srvEventHadler.mock(), onConnect),
                       Method(srvEventHadler.mock(), onDisconnect)).Once();
                Verify(Method(cliEventHadler.mock(), onConnect),
                       Method(cliEventHadler.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(srvEventHadler.mock());
                VerifyNoOtherInvocations(cliEventHadler.mock());

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

                Verify(Method(srvEventHadler.mock(), onConnect),
                       Method(srvEventHadler.mock(), onDisconnect)).Once();
                Verify(Method(cliEventHadler.mock(), onConnect),
                       Method(cliEventHadler.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(srvEventHadler.mock());
                VerifyNoOtherInvocations(cliEventHadler.mock());

                const bool expectedUseSpecificBehaviour = false;
                REQUIRE(actualUseSpecificBehaviour == expectedUseSpecificBehaviour);
            }
        }
    } // GIVEN
} // SCENARIO

// NOTE: This test case is disabled. It was executed manually by disabling the network connection
// on the VM with the server.
SCENARIO( "Keep alive feature testing", "[.keep_alive]" )
{
    auto cliEventHadler = MockedPtr<cli::IEventHandler>{};
    auto cliMessageSenderAcceptor = MockedPtr<cli::IMessageSenderAcceptor>{};
    cli::IMessageSenderPtr cliMessageSender;

    std::mutex mutex;
    std::condition_variable condVar;
    bool clientStopped = false;

    auto stopClient = [&condVar, &clientStopped]()
    {
        clientStopped = true;
        condVar.notify_one();
    };

    Fake(Method(cliEventHadler.mock(), onConnect), Method(cliEventHadler.mock(), onError));
    When(Method(cliEventHadler.mock(), onDisconnect)).Do(stopClient);
    When(Method(cliMessageSenderAcceptor.mock(), acceptMessageSender))
        .Do([&cliMessageSender](cli::IMessageSenderPtr ms){ cliMessageSender = ms; });

    GIVEN( "Server and client" )
    {
        auto clientBuilder = cli::ClientBuilder{};
        clientBuilder
            .setVersion(cli::ClientVersion::v1_Amsterdam)
            .setAddress("192.168.1.7")
            .setPort(PORT)
            .setEventHandler(cliEventHadler.ptr())
            .setMessageSenderAcceptor(cliMessageSenderAcceptor.ptr());

        WHEN( "Client sets up keep alive feature" )
        {
            clientBuilder.setKeepAliveTimeout(10);
            clientBuilder.setKeepAliveProbes(3);
            clientBuilder.setKeepAliveProbesInterval(5);

            auto client = clientBuilder.build();

            std::unique_lock<std::mutex> guard(mutex);
            condVar.wait(guard, [&clientStopped]{ return clientStopped; });

            Verify(Method(cliEventHadler.mock(), onConnect),
                   Method(cliEventHadler.mock(), onDisconnect)).Once();
            VerifyNoOtherInvocations(cliEventHadler.mock());
        }
    } // GIVEN
}

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
