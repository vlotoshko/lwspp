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
#include <future>

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
const std::chrono::milliseconds TIMEOUT{100};

const std::string HELLO_SERVER = "hello server!";
const std::string HELLO_CLIENT = "hello client!";

void setupServerBehavior(Mock<srv::IEventHandler>& eventHandler,
                         Mock<srv::IActorAcceptor>& actorAcceptor,
                         srv::IActorPtr& actor,
                         std::string& incomeMessage)
{
    auto sendHelloToClient = [&](srv::ConnectionId, const srv::DataPacket& dataPacket)
    {
        incomeMessage = std::string{dataPacket.data, dataPacket.length};
        actor->sendTextData(HELLO_CLIENT);
    };

    Fake(Method(eventHandler, onConnect), Method(eventHandler, onFirstDataPacket),
         Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onTextDataReceive)).Do(sendHelloToClient);
    
    When(Method(actorAcceptor, acceptActor)).Do([&actor](srv::IActorPtr a){ actor = a; });
}

void setupClientBehavior(Mock<cli::IEventHandler>& eventHandler,
                         Mock<cli::IActorAcceptor>& actorAcceptor,
                         cli::IActorPtr& actor,
                         std::promise<std::string>& incomeMessage)
{
    auto sendHelloToServer = [&actor](cli::IConnectionInfoPtr)
    {
        actor->sendTextData(HELLO_SERVER);
    };

    auto onTextDataReceive = [&](const cli::DataPacket& dataPacket)
    {
        incomeMessage.set_value(std::string{dataPacket.data, dataPacket.length});
    };

    Fake(Method(eventHandler, onFirstDataPacket), Method(eventHandler, onDisconnect));
    When(Method(eventHandler, onConnect)).Do(sendHelloToServer);
    When(Method(eventHandler, onTextDataReceive)).Do(onTextDataReceive);
    
    When(Method(actorAcceptor, acceptActor))
        .Do([&actor](cli::IActorPtr a){ actor = a; });
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
        .setLwsLogLevel(DISABLE_LOG);

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
        .setLwsLogLevel(DISABLE_LOG);

    return clientBuilder.build();
}

} // namespace

//clazy:excludeall=non-pod-global-static

SCENARIO( "Clients sends 'hello world' to the server", "[hello_world]" )
{
    std::promise<std::string> incomeMessage;
    auto waitForMessage = incomeMessage.get_future();

    auto srvEventHandler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHandler = MockedPtr<cli::IEventHandler>{};
    
    auto srvActorAcceptor = MockedPtr<srv::IActorAcceptor>{};
    auto cliActorAcceptor = MockedPtr<cli::IActorAcceptor>{};

    srv::IActorPtr srvActor;
    cli::IActorPtr cliActor;

    std::string actualServerIncomeMessage;
    std::string actualClientIncomeMessage;

    setupServerBehavior(srvEventHandler.mock(), srvActorAcceptor.mock(),
                        srvActor, actualServerIncomeMessage);

    setupClientBehavior(cliEventHandler.mock(), cliActorAcceptor.mock(),
                        cliActor, incomeMessage);

    GIVEN( "Server and client" )
    {
        auto server = setupServer(srvEventHandler.ptr(), srvActorAcceptor.ptr());
        auto client = setupClient(cliEventHandler.ptr(), cliActorAcceptor.ptr());

        WHEN( "Client sends message to server" )
        {
            THEN( "Server receives message from client and sends message back" )
            {
                REQUIRE(waitForMessage.wait_for(TIMEOUT) == std::future_status::ready);
                actualClientIncomeMessage = waitForMessage.get();

                server.reset();
                client.reset();

                Verify(Method(srvEventHandler.mock(), onConnect),
                       Method(srvEventHandler.mock(), onFirstDataPacket),
                       Method(srvEventHandler.mock(), onTextDataReceive),
                       Method(srvEventHandler.mock(), onDisconnect)).Once();
                Verify(Method(cliEventHandler.mock(), onConnect),
                       Method(cliEventHandler.mock(), onFirstDataPacket),
                       Method(cliEventHandler.mock(), onTextDataReceive),
                       Method(cliEventHandler.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(srvEventHandler.mock());
                VerifyNoOtherInvocations(cliEventHandler.mock());

                REQUIRE(actualServerIncomeMessage == HELLO_SERVER);
                REQUIRE(actualClientIncomeMessage == HELLO_CLIENT);
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace tests
} // namespace lwspp
// NOLINTEND (readability-function-cognitive-complexity)
