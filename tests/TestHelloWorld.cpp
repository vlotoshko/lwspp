/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <thread>

#include "catch2/catch.hpp"
#include "MockedPtr.hpp"

#include "easywebsocket/client/ClientBuilder.hpp"
#include "easywebsocket/client/IEventHandler.hpp"
#include "easywebsocket/client/IDataSender.hpp"
#include "easywebsocket/client/IDataSenderAcceptor.hpp"

#include "easywebsocket/server/IEventHandler.hpp"
#include "easywebsocket/server/IDataSender.hpp"
#include "easywebsocket/server/IDataSenderAcceptor.hpp"
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
                         Mock<srv::IDataSenderAcceptor>& dataSenderAcceptor,
                         srv::IDataSenderPtr& dataSender,
                         std::string& incomeMessage)
{
    auto sendHelloToClient = [&](srv::SessionId, const std::string& message, size_t /*bytesRemains*/)
    {
        incomeMessage = message;
        dataSender->sendTextData(HELLO_CLIENT);
    };

    Fake(Method(eventHandler, onConnect), Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onTextDataReceive)).Do(sendHelloToClient);

    When(Method(dataSenderAcceptor, acceptDataSender))
        .Do([&dataSender](srv::IDataSenderPtr ms){ dataSender = ms; });
}

void setupClientBehavior(Mock<cli::IEventHandler>& eventHandler,
                         Mock<cli::IDataSenderAcceptor>& dataSenderAcceptor,
                         cli::IDataSenderPtr& dataSender,
                         std::string& incomeMessage)
{
    auto sendHelloToServer = [&dataSender](cli::ISessionInfoPtr)
    {
        dataSender->sendTextData(HELLO_SERVER);
    };

    auto onTextDataReceive = [&](const std::string& message, size_t /*bytesRemains*/)
    {
        incomeMessage = message;
    };

    Fake(Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onConnect)).Do(sendHelloToServer);
    When(Method(eventHandler, onTextDataReceive)).Do(onTextDataReceive);

    When(Method(dataSenderAcceptor, acceptDataSender))
        .Do([&dataSender](cli::IDataSenderPtr ms){ dataSender = ms; });
}

srv::IServerPtr setupServer(srv::IEventHandlerPtr eventHandler,
                            srv::IDataSenderAcceptorPtr dataSenderAcceptor)
{
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setPort(PORT)
        .setEventHandler(eventHandler)
        .setDataSenderAcceptor(dataSenderAcceptor)
        .setLwsLogLevel(DISABLE_LOG);

    return serverBuilder.build();
}

cli::IClientPtr setupClient(cli::IEventHandlerPtr eventHandler,
                            cli::IDataSenderAcceptorPtr dataSenderAcceptor)
{
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setAddress(ADDRESS)
        .setPort(PORT)
        .setEventHandler(eventHandler)
        .setDataSenderAcceptor(dataSenderAcceptor)
        .setLwsLogLevel(DISABLE_LOG);

    return clientBuilder.build();
}

} // namespace

SCENARIO( "Clients sends 'hello world' to the server", "[hello_world]" )
{
    auto srvEventHadler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHadler = MockedPtr<cli::IEventHandler>{};
    
    auto srvDataSenderAcceptor = MockedPtr<srv::IDataSenderAcceptor>{};
    auto cliDataSenderAcceptor = MockedPtr<cli::IDataSenderAcceptor>{};

    srv::IDataSenderPtr srvDataSender;
    cli::IDataSenderPtr cliDataSender;

    std::string actualServerIncomeMessage;
    std::string actualClientIncomeMessage;

    setupServerBehavior(srvEventHadler.mock(), srvDataSenderAcceptor.mock(),
                        srvDataSender, actualServerIncomeMessage);

    setupClientBehavior(cliEventHadler.mock(), cliDataSenderAcceptor.mock(),
                        cliDataSender, actualClientIncomeMessage);

    GIVEN( "Server and client" )
    {
        auto server = setupServer(srvEventHadler.ptr(), srvDataSenderAcceptor.ptr());
        auto client = setupClient(cliEventHadler.ptr(), cliDataSenderAcceptor.ptr());

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
                       Method(srvEventHadler.mock(), onTextDataReceive),
                       Method(srvEventHadler.mock(), onDisconnect)).Once();
                Verify(Method(cliEventHadler.mock(), onConnect),
                       Method(cliEventHadler.mock(), onTextDataReceive),
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
