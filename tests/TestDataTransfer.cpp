/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Sep, 2023
 */

#include <thread>

#include "catch2/catch.hpp"
#include "MockedPtr.hpp"

#include "easywebsocket/client/ClientBuilder.hpp"
#include "easywebsocket/client/IDataSender.hpp"
#include "easywebsocket/client/IEventHandler.hpp"
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

const unsigned char BYTE = 0b11111111;
int DEFAULT_LWS_BUFFER_SIZE = 4096;

const std::vector<char> HELLO_SERVER_BINARY(DEFAULT_LWS_BUFFER_SIZE + 1024, BYTE);
const std::vector<char> HELLO_CLIENT_BINARY(DEFAULT_LWS_BUFFER_SIZE*2 + 1024, BYTE);


void setupServerBehavior(Mock<srv::IEventHandler>& eventHandler,
                         Mock<srv::IDataSenderAcceptor>& dataSenderAcceptor,
                         srv::IDataSenderPtr& dataSender,
                         std::vector<char>& incomeData)
{
    auto sendHelloToClient = [&](srv::SessionId, const std::vector<char>& data, size_t bytesRemains)
    {
        incomeData.insert(incomeData.end(), data.begin(), data.end());
        if (bytesRemains == 0)
        {
            dataSender->sendBinaryData(HELLO_CLIENT_BINARY);
        }
    };

    Fake(Method(eventHandler, onConnect), Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onBinaryDataReceive)).AlwaysDo(sendHelloToClient);

    When(Method(dataSenderAcceptor, acceptDataSender))
        .Do([&dataSender](srv::IDataSenderPtr ms){ dataSender = ms; });
}

void setupClientBehavior(Mock<cli::IEventHandler>& eventHandler,
                         Mock<cli::IDataSenderAcceptor>& dataSenderAcceptor,
                         cli::IDataSenderPtr& dataSender,
                         std::vector<char>& incomeData)
{
    auto sendHelloToServer = [&dataSender](cli::ISessionInfoPtr)
    {
        dataSender->sendBinaryData(HELLO_SERVER_BINARY);
    };

    auto onBinaryDataReceive = [&](const std::vector<char>& data, size_t /*bytesRemains*/)
    {
        incomeData.insert(incomeData.end(), data.begin(), data.end());
    };

    Fake(Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onConnect)).Do(sendHelloToServer);
    When(Method(eventHandler, onBinaryDataReceive)).AlwaysDo(onBinaryDataReceive);

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
        .setLwsLogLevel(DISABLE_LOG)
        ;

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
        .setLwsLogLevel(DISABLE_LOG)
        ;

    return clientBuilder.build();
}

} // namespace

SCENARIO( "Clients sends binary data to the server", "[data_transfer]" )
{
    auto srvEventHadler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHadler = MockedPtr<cli::IEventHandler>{};
    
    auto srvDataSenderAcceptor = MockedPtr<srv::IDataSenderAcceptor>{};
    auto cliDataSenderAcceptor = MockedPtr<cli::IDataSenderAcceptor>{};

    srv::IDataSenderPtr srvDataSender;
    cli::IDataSenderPtr cliDataSender;

    std::vector<char> actualServerIncomeData;
    std::vector<char> actualClientIncomeData;

    setupServerBehavior(srvEventHadler.mock(), srvDataSenderAcceptor.mock(),
                        srvDataSender, actualServerIncomeData);

    setupClientBehavior(cliEventHadler.mock(), cliDataSenderAcceptor.mock(),
                        cliDataSender, actualClientIncomeData);

    GIVEN( "Server and client" )
    {
        auto server = setupServer(srvEventHadler.ptr(), srvDataSenderAcceptor.ptr());
        auto client = setupClient(cliEventHadler.ptr(), cliDataSenderAcceptor.ptr());

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

                Verify(Method(srvEventHadler.mock(), onBinaryDataReceive)).Exactly(2);
                Verify(Method(cliEventHadler.mock(), onBinaryDataReceive)).Exactly(3);

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
