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
#include "lwspp/server/IConnectionInfo.hpp"
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
const std::chrono::seconds TIMEOUT{1};

void setupServerBehavior(Mock<srv::IEventHandler>& eventHandler,
                         std::promise<srv::ConnectionId>& promiseConnected,
                         std::promise<void>& promiseDisconnected)
{
    auto onConnect = [&](srv::IConnectionInfoPtr connectionInfo)
    {
        promiseConnected.set_value(connectionInfo->getConnectionId());
    };

    auto onDisconnect = [&](srv::ConnectionId)
    {
        promiseDisconnected.set_value();
    };

    When(Method(eventHandler, onConnect)).Do(onConnect);
    When(Method(eventHandler, onDisconnect)).Do(onDisconnect);
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

SCENARIO( "Server disconnects client", "[disconnect_client]" )
{
    std::promise<srv::ConnectionId> promiseConnected;
    auto waitForConnection = promiseConnected.get_future();

    std::promise<void> promiseDisconnected;
    auto waitForDisconnection = promiseDisconnected.get_future();

    auto srvEventHandler = MockedPtr<srv::IEventHandler>{};
    auto cliEventHandler = MockedPtr<cli::IEventHandler>{};
    
    auto srvActorAcceptor = MockedPtr<srv::IActorAcceptor>{};
    auto cliActorAcceptor = MockedPtr<cli::IActorAcceptor>{};

    srv::IActorPtr srvActor;

    setupServerBehavior(srvEventHandler.mock(), promiseConnected, promiseDisconnected);
    When(Method(srvActorAcceptor.mock(), acceptActor)).Do([&srvActor](srv::IActorPtr a){ srvActor = a; });

    Fake(Method(cliEventHandler.mock(), onConnect), Method(cliEventHandler.mock(), onDisconnect),
         Method(cliEventHandler.mock(), onWarning),  Method(cliEventHandler.mock(), onError));

    Fake(Method(cliActorAcceptor.mock(), acceptActor));

    GIVEN( "Server and client are connected" )
    {
        auto server = setupServer(srvEventHandler.ptr(), srvActorAcceptor.ptr());
        auto client = setupClient(cliEventHandler.ptr(), cliActorAcceptor.ptr());

        REQUIRE(waitForConnection.wait_for(TIMEOUT) == std::future_status::ready);
        auto connectionId = waitForConnection.get();

        VerifyNoOtherInvocations(Method(srvEventHandler.mock(), onDisconnect));

        WHEN( "Server closes the connection with the client" )
        {
            srvActor->closeConnection(connectionId);

            THEN( "The connection is closed" )
            {
                REQUIRE(waitForDisconnection.wait_for(TIMEOUT*10) == std::future_status::ready);
                Verify(Method(srvEventHandler.mock(), onDisconnect)).Once();

                server.reset();
                client.reset();

                Verify(Method(srvEventHandler.mock(), onConnect)).Once();
                VerifyNoOtherInvocations(srvEventHandler.mock());

                Verify(Method(cliEventHandler.mock(), onConnect)).Once();
                Verify(Method(cliEventHandler.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(cliEventHandler.mock());
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace tests
} // namespace lwspp
// NOLINTEND (readability-function-cognitive-complexity)
