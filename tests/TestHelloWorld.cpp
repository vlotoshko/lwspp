/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <thread>

#include "catch2/catch.hpp"
#include "MockedPtr.hpp"

#include "easywebsocket/client/ClientBuilder.hpp"
#include "easywebsocket/client/IEventHandler.hpp"
#include "easywebsocket/client/IMessageSender.hpp"
#include "easywebsocket/client/IMessageSenderAcceptor.hpp"

#include "easywebsocket/server/IEventHandler.hpp"
#include "easywebsocket/server/IMessageSender.hpp"
#include "easywebsocket/server/IMessageSenderAcceptor.hpp"
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

const std::string HELLO_SERVER = "hello server!";
const std::string HELLO_CLIENT = "hello client!";

void setupServerBehavior(Mock<srv::IEventHandler>& eventHandler,
                         Mock<srv::IMessageSenderAcceptor>& messageSenderAcceptor,
                         srv::IMessageSenderPtr& messageSender,
                         std::string& incomeMessage)
{
    auto sendHelloToClient = [&](srv::SessionId, const std::string& message)
    {
        incomeMessage = message;
        messageSender->sendMessage(HELLO_CLIENT);
    };

    Fake(Method(eventHandler, onConnect), Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onMessageReceive)).Do(sendHelloToClient);

    When(Method(messageSenderAcceptor, acceptMessageSender))
        .Do([&messageSender](srv::IMessageSenderPtr ms){ messageSender = ms; });
}

void setupClientBehavior(Mock<cli::IEventHandler>& eventHandler,
                         Mock<cli::IMessageSenderAcceptor>& messageSenderAcceptor,
                         cli::IMessageSenderPtr& messageSender,
                         std::string& incomeMessage)
{
    auto sendHelloToServer = [&messageSender](cli::ISessionInfoPtr)
    {
        messageSender->sendMessage(HELLO_SERVER);
    };

    auto onMessageReceive = [&](const std::string& message)
    {
        incomeMessage = message;
    };

    Fake(Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onConnect)).Do(sendHelloToServer);
    When(Method(eventHandler, onMessageReceive)).Do(onMessageReceive);

    When(Method(messageSenderAcceptor, acceptMessageSender))
        .Do([&messageSender](cli::IMessageSenderPtr ms){ messageSender = ms; });
}

srv::IServerPtr setupServer(srv::IEventHandlerPtr eventHandler,
                            srv::IMessageSenderAcceptorPtr messageSenderAcceptor)
{
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setVersion(srv::ServerVersion::v1_Andromeda)
        .setPort(PORT)
        .setEventHandler(eventHandler)
        .setMessageSenderAcceptor(messageSenderAcceptor)
        .setLwsLogLevel(DISABLE_LOG);

    return serverBuilder.build();
}

cli::IClientPtr setupClient(cli::IEventHandlerPtr eventHandler,
                            cli::IMessageSenderAcceptorPtr messageSenderAcceptor)
{
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setVersion(cli::ClientVersion::v1_Amsterdam)
        .setAddress(ADDRESS)
        .setPort(PORT)
        .setEventHandler(eventHandler)
        .setMessageSenderAcceptor(messageSenderAcceptor)
        .setLwsLogLevel(DISABLE_LOG);

    return clientBuilder.build();
}

} // namespace

SCENARIO( "Clients sends 'hello world' to the server", "[hello_world]" )
{
    auto srvEventHadler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHadler = MockedPtr<cli::IEventHandler>{};

    auto srvMessageSenderAcceptor = MockedPtr<srv::IMessageSenderAcceptor>{};
    auto cliMessageSenderAcceptor = MockedPtr<cli::IMessageSenderAcceptor>{};

    srv::IMessageSenderPtr srvMessageSender;
    cli::IMessageSenderPtr cliMessageSender;

    std::string actualServerIncomeMessage;
    std::string actualClientIncomeMessage;

    setupServerBehavior(srvEventHadler.mock(), srvMessageSenderAcceptor.mock(),
                        srvMessageSender, actualServerIncomeMessage);

    setupClientBehavior(cliEventHadler.mock(), cliMessageSenderAcceptor.mock(),
                        cliMessageSender, actualClientIncomeMessage);

    GIVEN( "Server and client" )
    {
        auto server = setupServer(srvEventHadler.ptr(), srvMessageSenderAcceptor.ptr());
        auto client = setupClient(cliEventHadler.ptr(), cliMessageSenderAcceptor.ptr());

        WHEN( "Client sends message to server" )
        {
            THEN( "Server receives message from client and sends message back" )
            {
                // waiting for the client and server exchange with the messages
                const auto timeout = 100U;
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

                server.reset();
                client.reset();

                Verify(Method(srvEventHadler.mock(), onConnect),
                       Method(srvEventHadler.mock(), onMessageReceive),
                       Method(srvEventHadler.mock(), onDisconnect)).Once();
                Verify(Method(cliEventHadler.mock(), onConnect),
                       Method(cliEventHadler.mock(), onMessageReceive),
                       Method(cliEventHadler.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(srvEventHadler.mock());
                VerifyNoOtherInvocations(cliEventHadler.mock());

                REQUIRE(actualServerIncomeMessage == HELLO_SERVER);
                REQUIRE(actualClientIncomeMessage == HELLO_CLIENT);
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
