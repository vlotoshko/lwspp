/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Sep, 2023
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

const unsigned char BYTE = 0b11111111;
int DEFAULT_LWS_BUFFER_SIZE = 4096;

const std::vector<char> HELLO_SERVER_BINARY(DEFAULT_LWS_BUFFER_SIZE + 1024, BYTE);
const std::vector<char> HELLO_CLIENT_BINARY(DEFAULT_LWS_BUFFER_SIZE*2 + 1024, BYTE);


void setupServerBehavior(Mock<srv::IEventHandler>& eventHandler,
                         Mock<srv::IMessageSenderAcceptor>& messageSenderAcceptor,
                         srv::IMessageSenderPtr& messageSender,
                         std::vector<char>& incomeData)
{
    auto sendHelloToClient = [&](srv::SessionId, const std::vector<char>& data, size_t bytesRemains)
    {
        incomeData.insert(incomeData.end(), data.begin(), data.end());
        if (bytesRemains == 0)
        {
            messageSender->sendData(HELLO_CLIENT_BINARY);
        }
    };

    Fake(Method(eventHandler, onConnect), Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onDataReceive)).AlwaysDo(sendHelloToClient);

    When(Method(messageSenderAcceptor, acceptMessageSender))
        .Do([&messageSender](srv::IMessageSenderPtr ms){ messageSender = ms; });
}

void setupClientBehavior(Mock<cli::IEventHandler>& eventHandler,
                         Mock<cli::IMessageSenderAcceptor>& messageSenderAcceptor,
                         cli::IMessageSenderPtr& messageSender,
                         std::vector<char>& incomeData)
{
    auto sendHelloToServer = [&messageSender](cli::ISessionInfoPtr)
    {
        messageSender->sendData(HELLO_SERVER_BINARY);
    };

    auto onDataReceive = [&](const std::vector<char>& data, size_t /*bytesRemains*/)
    {
        incomeData.insert(incomeData.end(), data.begin(), data.end());
    };

    Fake(Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onConnect)).Do(sendHelloToServer);
    When(Method(eventHandler, onDataReceive)).AlwaysDo(onDataReceive);

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
        .setLwsLogLevel(DISABLE_LOG)
        ;

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
        .setLwsLogLevel(DISABLE_LOG)
        ;

    return clientBuilder.build();
}

} // namespace

SCENARIO( "Clients sends binary data to the server", "[data_transfer]" )
{
    auto srvEventHadler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHadler = MockedPtr<cli::IEventHandler>{};

    auto srvMessageSenderAcceptor = MockedPtr<srv::IMessageSenderAcceptor>{};
    auto cliMessageSenderAcceptor = MockedPtr<cli::IMessageSenderAcceptor>{};

    srv::IMessageSenderPtr srvMessageSender;
    cli::IMessageSenderPtr cliMessageSender;

    std::vector<char> actualServerIncomeData;
    std::vector<char> actualClientIncomeData;

    setupServerBehavior(srvEventHadler.mock(), srvMessageSenderAcceptor.mock(),
                        srvMessageSender, actualServerIncomeData);

    setupClientBehavior(cliEventHadler.mock(), cliMessageSenderAcceptor.mock(),
                        cliMessageSender, actualClientIncomeData);

    GIVEN( "Server and client" )
    {
        auto server = setupServer(srvEventHadler.ptr(), srvMessageSenderAcceptor.ptr());
        auto client = setupClient(cliEventHadler.ptr(), cliMessageSenderAcceptor.ptr());

        WHEN( "Client sends binary data to the  server" )
        {
            THEN( "Server receives binary data from client and sends binary data back" )
            {
                // waiting for the client and server exchange with the messages
                const auto timeout = 100U;
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

                server.reset();
                client.reset();

                Verify(Method(srvEventHadler.mock(), onConnect),
                       Method(srvEventHadler.mock(), onDisconnect)).Once();
                Verify(Method(cliEventHadler.mock(), onConnect),
                       Method(cliEventHadler.mock(), onDisconnect)).Once();

                Verify(Method(srvEventHadler.mock(), onDataReceive)).Exactly(2);
                Verify(Method(cliEventHadler.mock(), onDataReceive)).Exactly(3);

                VerifyNoOtherInvocations(srvEventHadler.mock());
                VerifyNoOtherInvocations(cliEventHadler.mock());

                REQUIRE(actualServerIncomeData == HELLO_SERVER_BINARY);
                REQUIRE(actualClientIncomeData == HELLO_CLIENT_BINARY);
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
