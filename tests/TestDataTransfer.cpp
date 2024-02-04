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
#include "lwspp/client/IActor.hpp"
#include "lwspp/client/IActorAcceptor.hpp"
#include "lwspp/client/IEventHandler.hpp"

#include "lwspp/server/IActor.hpp"
#include "lwspp/server/IActorAcceptor.hpp"
#include "lwspp/server/IEventHandler.hpp"
#include "lwspp/server/ServerBuilder.hpp"

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


void setupServerBehavior(Mock<srv::IEventHandler>& eventHandler,
                         Mock<srv::IActorAcceptor>& actorAcceptor,
                         srv::IActorPtr& actor,
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
            actor->sendBinaryData(HELLO_CLIENT_BINARY);
        }
    };

    Fake(Method(eventHandler, onConnect), Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onFirstDataPacket)).AlwaysDo(onFirstDataPacket);
    When(Method(eventHandler, onBinaryDataReceive)).AlwaysDo(sendHelloToClient);
    
    When(Method(actorAcceptor, acceptActor))
        .Do([&actor](srv::IActorPtr ms){ actor = ms; });
}

void setupClientBehavior(Mock<cli::IEventHandler>& eventHandler,
                         Mock<cli::IActorAcceptor>& actorAcceptor,
                         cli::IActorPtr& actor,
                         std::vector<char>& incomeData,
                         size_t& incomeDataLength)
{
    auto sendHelloToServer = [&actor](cli::IConnectionInfoPtr)
    {
        actor->sendBinaryData(HELLO_SERVER_BINARY);
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

    Fake(Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onConnect)).Do(sendHelloToServer);
    When(Method(eventHandler, onFirstDataPacket)).AlwaysDo(onFirstDataPacket);
    When(Method(eventHandler, onBinaryDataReceive)).AlwaysDo(onBinaryDataReceive);
    
    When(Method(actorAcceptor, acceptActor))
        .Do([&actor](cli::IActorPtr ms){ actor = ms; });
}

srv::IServerPtr setupServer(srv::IEventHandlerPtr eventHandler,
                            srv::IActorAcceptorPtr actorAcceptor)
{
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setPort(PORT)
        .setEventHandler(eventHandler)
        .setActorAcceptor(actorAcceptor)
        .setLwsLogLevel(DISABLE_LOG)
        ;

    return serverBuilder.build();
}

cli::IClientPtr setupClient(cli::IEventHandlerPtr eventHandler,
                            cli::IActorAcceptorPtr actorAcceptor)
{
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setAddress(ADDRESS)
        .setPort(PORT)
        .setEventHandler(eventHandler)
        .setActorAcceptor(actorAcceptor)
        .setLwsLogLevel(DISABLE_LOG)
        ;

    return clientBuilder.build();
}

} // namespace

SCENARIO( "Clients sends binary data to the server", "[data_transfer]" )
{
    auto srvEventHandler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHandler = MockedPtr<cli::IEventHandler>{};
    
    auto srvActorAcceptor = MockedPtr<srv::IActorAcceptor>{};
    auto cliActorAcceptor = MockedPtr<cli::IActorAcceptor>{};

    srv::IActorPtr srvActor;
    cli::IActorPtr cliActor;

    std::vector<char> actualServerIncomeData;
    std::vector<char> actualClientIncomeData;

    size_t actualServerIncomeDataLength = 0;
    size_t actualClientIncomeDataLength = 0;

    setupServerBehavior(srvEventHandler.mock(), srvActorAcceptor.mock(),
                        srvActor, actualServerIncomeData, actualServerIncomeDataLength);

    setupClientBehavior(cliEventHandler.mock(), cliActorAcceptor.mock(),
                        cliActor, actualClientIncomeData, actualClientIncomeDataLength);

    GIVEN( "Server and client" )
    {
        auto server = setupServer(srvEventHandler.ptr(), srvActorAcceptor.ptr());
        auto client = setupClient(cliEventHandler.ptr(), cliActorAcceptor.ptr());

        WHEN( "Client sends binary data to the  server" )
        {
            THEN( "Server receives binary data from client and sends binary data back" )
            {
                // waiting for the client and server exchange with the messages
                const auto timeout = 100U;
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

                server.reset();
                client.reset();

                Verify(Method(srvEventHandler.mock(), onConnect),
                       Method(srvEventHandler.mock(), onFirstDataPacket),
                       Method(srvEventHandler.mock(), onDisconnect)).Once();
                Verify(Method(cliEventHandler.mock(), onConnect),
                       Method(cliEventHandler.mock(), onFirstDataPacket),
                       Method(cliEventHandler.mock(), onDisconnect)).Once();

                Verify(Method(srvEventHandler.mock(), onBinaryDataReceive)).AtLeastOnce();
                Verify(Method(cliEventHandler.mock(), onBinaryDataReceive)).AtLeastOnce();

                VerifyNoOtherInvocations(srvEventHandler.mock());
                VerifyNoOtherInvocations(cliEventHandler.mock());

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
