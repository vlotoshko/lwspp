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
#include <thread>

#include "MockedPtr.hpp"

#include "lwspp/client/ClientBuilder.hpp"
#include "lwspp/client/IClientControl.hpp" // IWYU pragma: keep
#include "lwspp/client/contract/IClientControlAcceptor.hpp"
#include "lwspp/client/contract/IClientLogic.hpp"

#include "lwspp/server/IServerControl.hpp" // IWYU pragma: keep
#include "lwspp/server/ServerBuilder.hpp"
#include "lwspp/server/contract/IServerControlAcceptor.hpp"
#include "lwspp/server/contract/IServerLogic.hpp"

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace lwspp
{
namespace tests
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


void setupServerBehavior(Mock<srv::contract::IServerLogic>& serverLogic,
                         Mock<srv::contract::IServerControlAcceptor>& serverControlAcceptor,
                         srv::IServerControlPtr& serverControl,
                         std::vector<char>& incomeData,
                         size_t& incomeDataLength)
{
    auto onFirstDataPacket = [&](srv::ConnectionId, size_t messageLength)
    {
        incomeDataLength = messageLength;
    };

    auto sendHelloToClient = [&](srv::ConnectionId, const srv::DataPacket& dataPacket)
    {
        incomeData.reserve(incomeData.size() + dataPacket.length);
        incomeData.insert(incomeData.end(), dataPacket.data, dataPacket.data + dataPacket.length);
        if (dataPacket.remains == 0)
        {
            serverControl->sendBinaryData(HELLO_CLIENT_BINARY);
        }
    };

    Fake(Method(serverLogic, onConnect), Method(serverLogic, onDisconnect));
    When(Method(serverLogic, onFirstDataPacket)).AlwaysDo(onFirstDataPacket);
    When(Method(serverLogic, onBinaryDataReceive)).AlwaysDo(sendHelloToClient);
    
    When(Method(serverControlAcceptor, acceptServerControl))
        .Do([&serverControl](srv::IServerControlPtr c){ serverControl = c; });
}

void setupClientBehavior(Mock<cli::contract::IClientLogic>& clientLogic,
                         Mock<cli::contract::IClientControlAcceptor>& clientControlAcceptor,
                         cli::IClientControlPtr& clientControl,
                         std::vector<char>& incomeData,
                         size_t& incomeDataLength)
{
    auto sendHelloToServer = [&clientControl](cli::IConnectionInfoPtr)
    {
        clientControl->sendBinaryData(HELLO_SERVER_BINARY);
    };

    auto onFirstDataPacket = [&](size_t messageLength)
    {
        incomeDataLength = messageLength;
    };

    auto onBinaryDataReceive = [&](const cli::DataPacket& dataPacket)
    {
        incomeData.reserve(incomeData.size() + dataPacket.length);
        incomeData.insert(incomeData.end(), dataPacket.data, dataPacket.data + dataPacket.length);
    };

    Fake(Method(clientLogic, onDisconnect));
    When(Method(clientLogic, onConnect)).Do(sendHelloToServer);
    When(Method(clientLogic, onFirstDataPacket)).AlwaysDo(onFirstDataPacket);
    When(Method(clientLogic, onBinaryDataReceive)).AlwaysDo(onBinaryDataReceive);
    
    When(Method(clientControlAcceptor, acceptClientControl))
        .Do([&clientControl](cli::IClientControlPtr c){ clientControl = c; });
}

srv::IServerPtr setupServer(srv::contract::IServerLogicPtr serverLogic,
                            srv::contract::IServerControlAcceptorPtr serverControlAcceptor)
{
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setPort(PORT)
        .setServerLogic(serverLogic)
        .setServerControlAcceptor(serverControlAcceptor)
        .setLwsLogLevel(DISABLE_LOG)
        ;

    return serverBuilder.build();
}

cli::IClientPtr setupClient(cli::contract::IClientLogicPtr clientLogic,
                            cli::contract::IClientControlAcceptorPtr clientControlAcceptor)
{
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setAddress(ADDRESS)
        .setPort(PORT)
        .setClientLogic(clientLogic)
        .setClientControlAcceptor(clientControlAcceptor)
        .setLwsLogLevel(DISABLE_LOG)
        ;

    return clientBuilder.build();
}

} // namespace

SCENARIO( "Clients sends binary data to the server", "[data_transfer]" )
{
    auto srvLogic = MockedPtr<srv::contract::IServerLogic>{};
    auto cliLogic = MockedPtr<cli::contract::IClientLogic>{};
    
    auto srvControlAcceptor = MockedPtr<srv::contract::IServerControlAcceptor>{};
    auto cliControlAcceptor = MockedPtr<cli::contract::IClientControlAcceptor>{};

    srv::IServerControlPtr srvControl;
    cli::IClientControlPtr cliControl;

    std::vector<char> actualServerIncomeData;
    std::vector<char> actualClientIncomeData;

    size_t actualServerIncomeDataLength = 0;
    size_t actualClientIncomeDataLength = 0;

    setupServerBehavior(srvLogic.mock(), srvControlAcceptor.mock(),
                        srvControl, actualServerIncomeData, actualServerIncomeDataLength);

    setupClientBehavior(cliLogic.mock(), cliControlAcceptor.mock(),
                        cliControl, actualClientIncomeData, actualClientIncomeDataLength);

    GIVEN( "Server and client" )
    {
        auto server = setupServer(srvLogic.ptr(), srvControlAcceptor.ptr());
        auto client = setupClient(cliLogic.ptr(), cliControlAcceptor.ptr());

        WHEN( "Client sends binary data to the  server" )
        {
            THEN( "Server receives binary data from client and sends binary data back" )
            {
                // waiting for the client and server exchange with the messages
                const auto timeout = 100U;
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

                server.reset();
                client.reset();

                Verify(Method(srvLogic.mock(), onConnect),
                       Method(srvLogic.mock(), onFirstDataPacket),
                       Method(srvLogic.mock(), onDisconnect)).Once();
                Verify(Method(cliLogic.mock(), onConnect),
                       Method(cliLogic.mock(), onFirstDataPacket),
                       Method(cliLogic.mock(), onDisconnect)).Once();

                Verify(Method(srvLogic.mock(), onBinaryDataReceive)).AtLeastOnce();
                Verify(Method(cliLogic.mock(), onBinaryDataReceive)).AtLeastOnce();

                VerifyNoOtherInvocations(srvLogic.mock());
                VerifyNoOtherInvocations(cliLogic.mock());

                CHECK(actualServerIncomeData == HELLO_SERVER_BINARY);
                CHECK(actualClientIncomeData == HELLO_CLIENT_BINARY);

                CHECK(actualServerIncomeDataLength == HELLO_SERVER_BINARY.size());
                CHECK(actualClientIncomeDataLength == HELLO_CLIENT_BINARY.size());
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace tests
} // namespace lwspp
// NOLINTEND (readability-function-cognitive-complexity)
